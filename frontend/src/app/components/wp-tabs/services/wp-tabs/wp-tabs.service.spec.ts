import { TestBed } from '@angular/core/testing';

import { WpTabsService } from './wp-tabs.service';

describe('WpTabsService', () => {
  let service: WpTabsService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(WpTabsService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
