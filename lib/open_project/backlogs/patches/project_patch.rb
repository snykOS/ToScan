require_dependency 'project'

module OpenProject::Backlogs::Patches::ProjectPatch
  def self.included(base)
    base.class_eval do
      unloadable

      has_and_belongs_to_many :done_statuses, :join_table => :done_statuses_for_project

      include InstanceMethods
    end
  end

  module InstanceMethods
    def rebuild_positions
      return unless backlogs_enabled?

      shared_versions.each { |v| v.rebuild_positions(self) }
      nil
    end

    def backlogs_enabled?
      module_enabled? "backlogs"
    end
  end
end

Project.send(:include, OpenProject::Backlogs::Patches::ProjectPatch)
