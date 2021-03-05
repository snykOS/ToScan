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
# Takes user data coming from GitHub webhook data and stores
# them as a `GithubUser`.
# If the `GithubUser` already exists, it is updated.
#
# See: https://docs.github.com/en/developers/webhooks-and-events/webhook-events-and-payloads#pull_request
module GithubIntegration
  class UpsertUser < ::BaseServices::BaseCallable
    ##
    # Identifier of this dependency to include/exclude
    def self.identifier
      :upsert_github_user
    end

    protected

    def perform(params)
      begin
        upsert_user(params)
      rescue StandardError => e
        Rails.logger.error { "Failed to upsert GitHub User #{self.class.identifier}: #{e.message}" }
        result.success = false
        result.errors.add(:base, :could_not_be_upserted, dependency: self.class.human_name)
      end

      result
    end

    def upsert_user(params)
      gh_user_params = extract_gh_user_params(params)
      gh_user = find_or_initialize(gh_user_params.fetch(:github_id))
      gh_user.assign_values(gh_user_params)
      gh_user.save!
      gh_user
    end

    ##
    # Receives the input from the github webhook and translates them
    # to our internal representation.
    # See: https://docs.github.com/en/rest/reference/users
    def extract_gh_user_params(params)
      {
        github_id: params.fetch('id'),
        github_login: params.fetch('login'),
        github_url: params.fetch('html_url'),
        github_avatar_url: params.fetch('avatar_url')
      }
    end

    def find_or_initialize(github_id)
      GithubUser.find_or_initialize_by(github_id: github_id)
    end
  end
end
