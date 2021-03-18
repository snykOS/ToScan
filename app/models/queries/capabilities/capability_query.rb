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

class Queries::Capabilities::CapabilityQuery < Queries::BaseQuery
  def self.model
    Capability
  end

  def results
    super
    #.includes(:context, :principal)
      .reorder('permission_map ASC', 'principal_id ASC', 'capabilities.context_id ASC')
  end

  def default_scope
    Capability
      .default
  end

  private

  #def apply_orders(scope)
  #  orders.each do |order|
  #    scope = scope.merge(order.scope)
  #  end

  #  scope

  #  # To get deterministic results, especially when paginating (limit + offset)
  #  # an order needs to be prepended that is ensured to be
  #  # different between all elements.
  #  # Without such a criteria, results can occur on multiple pages.
  #  #already_ordered_by_id?(scope) ? scope : scope.order(id: :desc)
  #end
end
