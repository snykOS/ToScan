import {Component, Inject, OnInit, Output, EventEmitter, Optional} from '@angular/core';
import {InviteUserModalComponent} from "core-app/modules/common/invite-user-modal/invite-user.component";
import {OpEditingPortalSchemaToken} from "core-app/modules/fields/edit/edit-field.component";
import {IFieldSchema} from "core-app/modules/fields/field.base";
import {OpModalService} from "core-components/op-modals/op-modal.service";
import {CurrentProjectService} from "core-components/projects/current-project.service";
import {NgSelectComponent} from "@ng-select/ng-select";
import {SelectEditFieldComponent} from "core-app/modules/fields/edit/field-types/select-edit-field.component";
import {MultiSelectEditFieldComponent} from "core-app/modules/fields/edit/field-types/multi-select-edit-field.component";
import {I18nService} from "core-app/modules/common/i18n/i18n.service";
import {PermissionsService} from "core-app/core/services/permissions/permissions.service";
import {HalResource} from "core-app/modules/hal/resources/hal-resource";

@Component({
  selector: 'op-invite-user-button',
  templateUrl: './invite-user-button.component.html',
  styleUrls: ['./invite-user-button.component.sass']
})
export class InviteUserButtonComponent implements OnInit {
  @Output() invited = new EventEmitter<HalResource | HalResource[]>();

  get showButton() {
    const showButton = this.schema?.type === 'User' &&
      this.canInviteUsersToProject &&
      (this.selectEditFieldComponent || this.multiSelectEditFieldComponent);

    return showButton;
  }
  get parentIsMultiSelectEditFieldComponent() {
    return !!this.multiSelectEditFieldComponent;
  }
  get projectId() {
    // TODO: Should we default to currentProjectService.id?
    // Is it safe to rely on this.selectEditFieldComponent?.resource to get the projectId?
    return this.selectEditFieldComponent?.resource?.$source?._embedded?.project?.id ||
      this.multiSelectEditFieldComponent?.resource?.$source?._embedded?.project?.id ||
      this.currentProjectService.id;
  }

  text = {
    button: this.I18n.t('js.invite_user_modal.invite'),
  };
  canInviteUsersToProject:boolean;

  constructor(
    readonly I18n:I18nService,
    readonly opModalService:OpModalService,
    readonly currentProjectService:CurrentProjectService,
    readonly ngSelectComponent:NgSelectComponent,
    readonly permissionsService:PermissionsService,
    @Optional() readonly selectEditFieldComponent:SelectEditFieldComponent,
    @Optional() readonly multiSelectEditFieldComponent:MultiSelectEditFieldComponent,
    @Inject(OpEditingPortalSchemaToken) public schema:IFieldSchema,
  ) {}

  ngOnInit():void {
    this.permissionsService
      .canInviteUsersToProject$()
      .subscribe(canInviteUsersToProject => this.canInviteUsersToProject = canInviteUsersToProject);
  }

  onAddNewClick($event:Event) {
    this.ngSelectComponent.close();
    $event.stopPropagation();
    this.openInviteUserModal();
  }

  openInviteUserModal() {
    const inviteModal = this.opModalService.show(InviteUserModalComponent, 'global', {
      projectId: this.projectId,
    });

    inviteModal
      .closingEvent
      .subscribe((modal:any) => {
        let dataToEmit = null;

        if (modal.data) {
          // TODO: Remove this data formatting
          // The principal should be a UserResource instance to match
          // the interface used by the SelectEditFieldComponent's
          // availableOptions.
          dataToEmit = {
            ...modal.data,
            $href: modal.data.$source?._links?.self?.href,
          };

          if (this.parentIsMultiSelectEditFieldComponent) {
            dataToEmit = [dataToEmit];
          }
        }

        this.invited.emit(dataToEmit);
      });
  }
}
