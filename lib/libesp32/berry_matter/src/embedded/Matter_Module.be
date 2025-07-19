#
# Matter_Module.be - implements the global `matter` module from a solidified module that can be dynamically enriched
#
# Copyright (C) 2023  Stephan Hadinger & Theo Arends
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

# matter module for extensibility
#
# this allows the module `matter` to be extended

var matter = module("matter")

def setmember(k, v)
  import global
  if !global.contains(".matter")    global.(".matter") = {} end
  global.(".matter")[k] = v
end

def member(k)
  import global
  import undefined

  if global.contains(".matter") && global.(".matter").contains(k)
    return global.(".matter")[k]
  else
    return undefined
  end
end

matter.setmember = setmember
matter.member = member
#@ solidify:matter.setmember,weak
#@ solidify:matter.member,weak
