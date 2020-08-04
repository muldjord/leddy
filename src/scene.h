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

#include "uniconn.h"

#include <QObject>
#include <QImage>

class Scene : public QObject
{
  Q_OBJECT

public:
  Scene(const Scene &scene);
  void operator=(const Scene &scene);
  Scene() {};
  Scene(const QString &type);
  virtual ~Scene() {};
  QImage init(const QImage &latestBuffer = QImage(16, 16, QImage::Format_ARGB32),
              Scene *nextScene = nullptr, UniConn *uniConn = nullptr);

public slots:
  void nextFrame();
  void update(QImage buffer);

signal:
  void frameReady(QImage image);
  void sceneEnded();
  
private:
  void drawText(const int x, const int y, const QString font, const QString text,
                const QColor color, const int spacing);

  QString type = "scene";
  
  Scene *nextScene = nullptr;
  UniConn *uniConn = nullptr;

  QTimer frameTimer;
  int currentFrame = 0;
  QList<QImage> frames;
  
  QImage oldBuffer = QImage(16, 16, QImage::Format_ARGB32);
  QImage newBuffer = QImage(16, 16, QImage::Format_ARGB32);
  
};

#endif // _SCENE_H
