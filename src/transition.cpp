/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            transition.h
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

#include "transition.h"
#include "globaldefs.h"

Transition::Transition(Settings &settings) : Scene(settings, SCENE::TRANSITION)
{
}

void Transition::nextFrame()
{
  frameTimer.setInterval(frames.at(currentFrame).first);

  if(currentFrame + 1 < frames.length()) {
    currentFrame++;
  } else {
    running = false;
    emit sceneEnded();
    return;
  }
  frameTimer.start();
}

QImage Transition::getBuffer()
{
  if(!running && nextScene != nullptr) {
    buffer = nextScene->getBuffer();
  } else {
    buffer = frames.at(currentFrame).second;
  }

  QImage toBuffer(settings.width, settings.height, QImage::Format_ARGB32);
  if(nextScene != nullptr) {
    toBuffer = nextScene->getBuffer();
  }

  QImage fromBuffer(settings.width, settings.height, QImage::Format_ARGB32);
  if(previousScene != nullptr) {
    fromBuffer = previousScene->getBuffer();
  }

  const QRgb *fromBits = (const QRgb *)fromBuffer.constBits();
  QRgb *bits = (QRgb *)buffer.bits();
  const QRgb *toBits = (const QRgb *)toBuffer.constBits();
  for(int a = 0; a < buffer.width() * buffer.height(); ++a) {
    if(qRed(bits[a]) == 255 && qGreen(bits[a]) == 0 && qBlue(bits[a]) == 255) {
      bits[a] = fromBits[a];
    }
    if(qRed(bits[a]) == 0 && qGreen(bits[a]) == 255 && qBlue(bits[a]) == 255) {
      bits[a] = toBits[a];
    }
  }

  return buffer;
}
