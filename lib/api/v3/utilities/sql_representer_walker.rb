#-- encoding: UTF-8

#-- copyright
# OpenProject is an open source project management software.
# Copyright (C) 2012-2020 the OpenProject GmbH
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License version 3.
#
# OpenProject is a fork of ChiliProject, which is a fork of Redmine. The copyright follows:
# Copyright (C) 2006-2017 Jean-Philippe Lang
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

module API
  module V3
    module Utilities
      class SqlRepresenterWalker
        include API::Utilities::PageSizeHelper

        def initialize(scope,
                       current_user:,
                       page_size: nil,
                       offset: nil,
                       embed: {},
                       select: {})
          self.scope = scope
          self.current_user = current_user
          self.embed = embed
          self.select = select
          self.page_size = page_size
          self.offset = offset
        end

        def walk(start)
          result = SqlWalkerResults.new(scope,
                                        page_size: resulting_page_size(page_size),
                                        offset: (to_i_or_nil(offset) || 1) - 1)

          result.selects = embedded_depth_first([], start) do |map, stack, current_representer|
            current_representer.select_sql(map, select_for(stack), result)
          end

          embedded_depth_first([], start) do |_, stack, current_representer|
            result.scope = current_representer.joins(select_for(stack), result.scope)
          end

          embedded_depth_first([], start) do |_, _, current_representer|
            result.ctes.merge!(current_representer.ctes(result))
          end

          self.sql = start.to_sql(result)

          self
        end

        def to_json(*)
          ActiveRecord::Base.connection.select_one(sql)['json']
        end

        protected

        attr_accessor :scope,
                      :current_user,
                      :embed,
                      :select,
                      :sql,
                      :page_size,
                      :offset

        def embedded_depth_first(stack, current_representer, &block)
          up_map = {}

          embed_for(stack).each_key do |key|
            representer = current_representer
                          .embed_map[key]

            up_map[key] = embedded_depth_first(stack.dup << key, representer, &block)
          end

          yield up_map, stack, current_representer
        end

        def select_for(stack)
          stack.any? ? select.dig(*stack) : select
        end

        def embed_for(stack)
          stack.any? ? embed.dig(*stack) : embed
        end
      end
    end
  end
end