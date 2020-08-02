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

Transition::Transition(const Transition &transition) : QObject()
{
  this->name = transition.name;
  this->frames = transition.frames;
  this->currentFrame = transition.currentFrame;
  this->frameTime = transition.frameTime;
}

void Transition::operator=(const Transition &transition)
{
  this->name = transition.name;
  this->frames = transition.frames;
  this->currentFrame = transition.currentFrame;
  this->frameTime = transition.frameTime;
}

Transition::Transition(const QString &name, const int &frameTime)
{
  this->name = name;
  this->frameTime = frameTime;
}

void Transition::addFrame(const QImage &frame)
{
  frames.append(frame);
}

void Transition::startTransition(const QImage &from, const QImage &to)
{
  fromBuffer = from;
  toBuffer = to;
  if(fromBuffer.format() != QImage::Format_ARGB32) {
    fromBuffer = fromBuffer.convertToFormat(QImage::Format_ARGB32);
  }
  if(toBuffer.format() != QImage::Format_ARGB32) {
    toBuffer = toBuffer.convertToFormat(QImage::Format_ARGB32);
  }
  currentFrame = 0;
}

int Transition::getFrameTime()
{
  return frameTime;
}

QImage Transition::getNextFrame()
{
  if(currentFrame >= frames.length()) {
    return QImage();
  }

  QImage mergedBuffer = frames.at(currentFrame);
  
  const QRgb *fromBits = (const QRgb *)fromBuffer.constBits();
  const QRgb *toBits = (const QRgb *)toBuffer.constBits();
  QRgb *mergedBits = (QRgb *)mergedBuffer.bits();
  for(int a = 0; a < mergedBuffer.width() * mergedBuffer.height(); ++a) {
    if(qRed(mergedBits[a]) == 255 && qGreen(mergedBits[a]) == 0 && qBlue(mergedBits[a]) == 255) {
      mergedBits[a] = fromBits[a];
    }
    if(qRed(mergedBits[a]) == 0 && qGreen(mergedBits[a]) == 255 && qBlue(mergedBits[a]) == 255) {
      mergedBits[a] = toBits[a];
    }
  }
  currentFrame++;

  return mergedBuffer;
}
