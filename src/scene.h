/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            scene.h
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

#ifndef _SCENE_H
#define _SCENE_H

#include "settings.h"

#include <QObject>
#include <QImage>
#include <QTimer>

class Scene : public QObject
{
  Q_OBJECT

public:
  Scene(Settings &settings, const int &sceneTime = -1);
  virtual ~Scene() {};
  virtual void init(Scene *previousScene = nullptr,
                    Scene *nextScene = nullptr);
  int getSceneTime();
  void addFrame(const QPair<int, QImage> &frame);
  virtual QImage getBuffer();
                   
public slots:
  virtual void nextFrame();

signals:
  void frameReady(QImage image);
  void sceneEnded();
  
protected:
  void drawText(const int x, const int y, const QString font, const QString text,
                const QColor color, const int spacing);

  Settings &settings;
  
  bool running = false;
  bool endScene = false;
  
  Scene *previousScene = nullptr;
  Scene *nextScene = nullptr;

  QTimer frameTimer;
  int currentFrame = 0;
  
  QList<QPair<int, QImage> > frames;
  
  QImage buffer = QImage(16, 16, QImage::Format_ARGB32);

private:
  int sceneTime = -1; // -1 equals a one shot scene

};

#endif // _SCENE_H
