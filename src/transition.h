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

#ifndef _TRANSITION_H
#define _TRANSITION_H

#include <QObject>
#include <QImage>

class Transition : public QObject
{
  Q_OBJECT

public:
  Transition(const Transition &transition);
  void operator=(const Transition &transition);
  Transition() {};
  Transition(const QString &name, const int &frameTime);
  virtual ~Transition() {};
  void addFrame(const QImage &frame);
  void startTransition(const QImage &from, const QImage &to);
  int getFrameTime();
  QImage getNextFrame();
  
private:
  QString name = "";
  int frameTime = 50;
  QList<QImage> frames;

  int currentFrame = 0;

  QImage fromBuffer = QImage(16, 16, QImage::Format_ARGB32);
  QImage toBuffer = QImage(16, 16, QImage::Format_ARGB32);

};

#endif // _TRANSITION_H
