/*
  be_matter_module.c - implements the high level `matter` Berry module

  Copyright (C) 2023  Stephan Hadinger & Theo Arends

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/********************************************************************
 * Matter global module
 * 
 *******************************************************************/

#ifdef USE_BERRY_ANIMATION
#include "be_constobj.h"
#include "be_mapping.h"

#include "solidify/solidified_animation.h"
  #ifdef USE_BERRY_ANIMATION_DSL
  #include "solidify/solidified_animation_dsl.h"
  #endif
#endif