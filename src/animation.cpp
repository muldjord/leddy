/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            animation.h
 *
 *  Sun Aug 2 12:00:00 CEST 2020
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

#include "animation.h"

Animation::Animation(Settings &settings, const int &sceneTime) : Scene(settings, sceneTime)
{
}

void Animation::init(Scene *previousScene, Scene *nextScene)
{
  this->previousScene = previousScene;
  this->nextScene = nextScene;
  
  endScene = false;
  
  if(!running) {
    if(sceneTime != -1) {
      running = true;
    }
    currentFrame = 0;
    if(!frames.isEmpty()) {
      nextFrame();
    }
  }
}

void Animation::nextFrame()
{
  if(endScene) {
    running = false;
    emit sceneEnded();
    return;
  }

  frameTimer.setInterval(frames.at(currentFrame).first);

  if(currentFrame + 1 < frames.length()) {
    currentFrame++;
  } else {
    if(sceneTime == -1) {
      endScene = true;
    }
    currentFrame = 0;
  }
  frameTimer.start();
}

QImage Animation::getBuffer()
{
  buffer = frames.at(currentFrame).second;
  return buffer;
}
