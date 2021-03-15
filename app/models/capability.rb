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

# Turn it to something independent of a member
# and possibly prevent instantiating somehow
class Capability < Member#ApplicationRecord
  #include ActiveModel::Model
  #include ActiveRecord::Associations
  #extend ActiveRecord::AttributeMethods::ClassMethods
  #include ActiveRecord::AttributeMethods
  #include ActiveRecord::ModelSchema

  #self.pluralize_table_names = true
  #self.abstract_class = true

  belongs_to :context, class_name: 'Project'
  belongs_to :principal#, foreign_key: :principal_id

  self.table_name = 'members'

  attr_accessor :id,
                :permission

  #alias_method :context, :project

  #def context_id
  #  project_id
  #end

  #def principal_id
  #  user_id
  #end
end