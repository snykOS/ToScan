import { ComponentFixture, TestBed } from '@angular/core/testing';

import { WpTabsComponent } from './wp-tabs.component';

describe('WpTabsComponent', () => {
  let component: WpTabsComponent;
  let fixture: ComponentFixture<WpTabsComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ WpTabsComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(WpTabsComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
