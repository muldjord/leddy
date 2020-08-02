/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            transition.h
 *
 *  Fri Jul 24 12:00:00 CEST 2020
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

#ifndef _TRANSITION_H
#define _TRANSITION_H

#include "SFML/Audio.hpp"

#include <QObject>
#include <QPixmap>
#include <QMap>

// All single commands stored in the list with all children blocks replaced by '##1##', '##2##'
class Script: public QObject
{
public:
  Script(const Script &script);
  void operator=(const Script &script);
  Script();
  QList<QString> commands;
  QMap<QString, Script> blocks; // Contains map of '##1##', '##2##'... which contains a script with the commands of that block inside
};

class Frame: public QObject
{
  Q_OBJECT;

public:
  Frame(const Frame &frame);
  void operator=(const Frame &frame);
  Frame();
  QPixmap sprite;
  int time;
  int dx;
  int dy;
  sf::SoundBuffer *soundBuffer = nullptr;
  Script script;
};

class Transition: public QObject
{
  Q_OBJECT;

public:
  Transition(const Transition &transition);
  void operator=(const Transition &transition);
  Transition();
  QString file;
  QString title;
  QString category;
  int hyper;
  int health;
  int energy;
  int hunger;
  int bladder;
  int social;
  int fun;
  int hygiene;
  bool oneShot = false;
  bool doNotDisturb = false;
  bool allowFlip = false;
  bool pitchLock = false;
  QList<Frame> frames;
  QMap<QString, int> labels; // Used for scripts when goto'ing
  QMap<QString, Script> defines; // Used for defines of reusable scripting blocks
};

#endif // _TRANSITION_H
