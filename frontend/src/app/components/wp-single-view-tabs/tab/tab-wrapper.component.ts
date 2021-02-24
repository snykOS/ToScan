// -- copyright
// OpenProject is an open source project management software.
// Copyright (C) 2012-2021 the OpenProject GmbH
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License version 3.
//
// OpenProject is a fork of ChiliProject, which is a fork of Redmine. The copyright follows:
// Copyright (C) 2006-2013 Jean-Philippe Lang
// Copyright (C) 2010-2013 the ChiliProject Team
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
// See docs/COPYRIGHT.rdoc for more details.
// ++

import {Transition} from '@uirouter/core';
import {Component, OnInit} from '@angular/core';
import {I18nService} from 'core-app/modules/common/i18n/i18n.service';
import {WorkPackageResource} from 'core-app/modules/hal/resources/work-package-resource';
import {HookService} from 'core-app/modules/plugins/hook-service';
import {UntilDestroyedMixin} from 'core-app/helpers/angular/until-destroyed.mixin';
import {APIV3Service} from 'core-app/modules/apiv3/api-v3.service';
import {Tab} from './tab';

@Component({
  templateUrl: './tab-wrapper.html',
  selector: 'wp-tab',
})
export class WorkPackageTabComponent extends UntilDestroyedMixin implements OnInit {
  public tab:Tab|undefined;
  public workPackage:WorkPackageResource;

  public constructor(readonly I18n:I18nService,
                     readonly $transition:Transition,
                     readonly apiV3Service:APIV3Service,
                     readonly hooks:HookService,) {
    super();
  }

  findTab() {
    const tabIdentifier = this.$transition.params("to").tabIdentifier;
    const registeredTabs = this.hooks.getWorkPackageTabs();
    const displayableTabs = _.filter(
      registeredTabs,
      (tab) => tab.displayable(this.workPackage)
    );
    this.tab = displayableTabs.find(({identifier: id}) => id === tabIdentifier);
  }

  workPackageId() {
    return(this.$transition.params("to").workPackageId);
  }

  ngOnInit() {
    this
      .apiV3Service
      .work_packages
      .id(this.workPackageId())
      .requireAndStream()
      .pipe(
        this.untilDestroyed()
      )
      .subscribe((wp) => {
        this.workPackage = wp;
        this.findTab();
      });
  }
}
