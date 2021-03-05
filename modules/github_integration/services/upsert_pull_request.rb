#-- encoding: UTF-8

#-- copyright
# OpenProject is an open source project management software.
# Copyright (C) 2012-2021 the OpenProject GmbH
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License version 3.
#
# OpenProject is a fork of ChiliProject, which is a fork of Redmine. The copyright follows:
# Copyright (C) 2006-2013 Jean-Philippe Lang
# Copyright (C) 2010-2013 the ChiliProject Team
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#
# See docs/COPYRIGHT.rdoc for more details.
#++

##
# Takes pull request data coming from GitHub webhook data and stores
# them as a `GithubPullRequest`.
# If the `GithubPullRequest` already exists, it is updated.
#
# See: https://docs.github.com/en/developers/webhooks-and-events/webhook-events-and-payloads#pull_request
module GithubIntegration
  class UpsertPullRequest < ::BaseServices::BaseCallable
    ##
    # Identifier of this dependency to include/exclude
    def self.identifier
      :upsert_github_pull_request
    end

    protected

    def perform(params)
      begin
        upsert_pr(params)
      rescue StandardError => e
        Rails.logger.error { "Failed to upsert GitHub PR #{self.class.identifier}: #{e.message}" }
        result.success = false
        result.errors.add(:base, :could_not_be_upserted, dependency: self.class.human_name)
      end

      result
    end

    def upsert_pr(params)
      pr_params = extract_pr_params(params)
      pr = find_or_initialize(pr_params.fetch(:github_id))
      pr.assign_values(pr_params)
      pr.save!
      pr
    end

    ##
    # Receives the input from the github webhook and translates them
    # to our internal representation.
    # See: https://docs.github.com/en/rest/reference/pulls
    def extract_pr_params(params)
      {
        github_id: params.fetch('id'),
        number: params.fetch('number'),
        github_html_url: params.fetch('html_url'),
        github_updated_at: params.fetch('updated_at'),
        state: params.fetch('state'),
        title: params.fetch('title'),
        body: params.fetch('body'),
        draft: bool(params.fetch('draft')),
        merged: bool(params.fetch('merged')),
        merged_by: params['merged_by'],
        merged_at: params['merged_at'],
        comments_count: params.fetch('comments'),
        review_comments_count: params.fetch('review_comments'),
        additions_count: params.fetch('additions'),
        deletions_count: params.fetch('deletions'),
        changed_files_count: params.fetch('changed_files'),
        labels: params.fetch('labels').map(|values| extract_label_values(values))
        user: user(params.fetch('user'))
      }
    end

    def find_or_initialize(github_id)
      GithubPullRequest.find_or_initialize_by(github_id: github_id)
    end

    def bool(value)
      ActiveRecord::Type::Boolean.new.deserialize(value)
    end

    def extract_label_values(params)
      {
        name: params.fetch(:name),
        color: params.fetch(:color),
      }
    end

    def user(params)
      GithubIntegration::UpsertUser.new.call(params)
    end
  end
end