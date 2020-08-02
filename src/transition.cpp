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

#include "transition.h"

Script::Script(const Script &script) : QObject()
{
  this->commands = script.commands;
  this->blocks = script.blocks;
}

void Script::operator=(const Script &script)
{
  this->commands = script.commands;
  this->blocks = script.blocks;
}

Script::Script()
{
}

Frame::Frame(const Frame &frame): QObject()
{
  this->sprite = frame.sprite;
  this->time = frame.time;
  this->dx = frame.dx;
  this->dy = frame.dy;
  this->soundBuffer = frame.soundBuffer;
  this->script = frame.script;
}

void Frame::operator=(const Frame &frame)
{
  this->sprite = frame.sprite;
  this->time = frame.time;
  this->dx = frame.dx;
  this->dy = frame.dy;
  this->soundBuffer = frame.soundBuffer;
  this->script = frame.script;
}

Frame::Frame()
{
}

Transition::Transition()
{
}

Transition::Transition(const Transition &transition) : QObject()
{
  this->file = transition.file;
  this->title = transition.title;
  this->category = transition.category;
  this->hyper = transition.hyper;
  this->health = transition.health;
  this->energy = transition.energy;
  this->hunger = transition.hunger;
  this->bladder = transition.bladder;
  this->social = transition.social;
  this->fun = transition.fun;
  this->hygiene = transition.hygiene;
  this->oneShot = transition.oneShot;
  this->doNotDisturb = transition.doNotDisturb;
  this->allowFlip = transition.allowFlip;
  this->pitchLock = transition.pitchLock;
  this->frames = transition.frames;
  this->labels = transition.labels;
  this->defines = transition.defines;
}

void Transition::operator=(const Transition &transition)
{
  this->file = transition.file;
  this->title = transition.title;
  this->category = transition.category;
  this->hyper = transition.hyper;
  this->health = transition.health;
  this->energy = transition.energy;
  this->hunger = transition.hunger;
  this->bladder = transition.bladder;
  this->social = transition.social;
  this->fun = transition.fun;
  this->hygiene = transition.hygiene;
  this->oneShot = transition.oneShot;
  this->doNotDisturb = transition.doNotDisturb;
  this->allowFlip = transition.allowFlip;
  this->pitchLock = transition.pitchLock;
  this->frames = transition.frames;
  this->labels = transition.labels;
  this->defines = transition.defines;
}
