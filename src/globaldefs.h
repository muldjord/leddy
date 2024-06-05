/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            globaldefs.h
 *
 *  Thu Aug 6 12:00:00 CEST 2020
 *  Copyright 2020 Lars Muldjord
 *  muldjordlars@gmail.com
 ****************************************************************************/
/*
 *  This file is part of Leddy.
 *
 *  Leddy is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Leddy is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Leddy; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */

#ifndef _GLOBALDEFS_H
#define _GLOBALDEFS_H

namespace DURATION {
  // Durations
  constexpr int ONESHOT = -1;
}

namespace COLOR {
  constexpr int UNSET = -1;

  constexpr int STATIC = 10;
  constexpr int RANDOM = 11;
  constexpr int COMPLIMENTARY = 12;
}

namespace SCENE {
  // Base types
  constexpr int SCENE = 10;
  constexpr int TRANSITION = 11;
  constexpr int ANIMATION = 12;

  // Specialized types
  constexpr int TIMEDATE = 20;
  constexpr int WEATHER = 21;
  constexpr int RSSSCROLL = 22;
  constexpr int GAMEOFLIFE = 23;
  constexpr int RUNCOMMAND = 24;
  constexpr int GALLERY = 25;
}

#endif // _GLOBALDEFS_H
