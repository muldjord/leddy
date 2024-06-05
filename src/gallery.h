/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            gallery.h
 *
 *  Fri Sep 9 12:00:00 CEST 2022
 *  Copyright 2022 Lars Muldjord
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

#ifndef _GALLERY_H
#define _GALLERY_H

#include "scene.h"
#include "settings.h"
#include "globaldefs.h"

#include <QFileInfo>

class Gallery : public Scene
{
  Q_OBJECT

public:
  Gallery(Settings &settings,
          const QString &duration = QString(),
          const QString &input = QString(),
          const QString &order = QString(),
          const QString &hVel = QString(),
          const QString &vVel = QString(),
          const QString &scale = QString());
  void start() override;
                             
public slots:
  void nextFrame() override;

private slots:

private:
  void nextImage();
  QTimer galleryTimer;
  QImage image = QImage(settings.width, settings.height, QImage::Format_RGB32);
  QList<QFileInfo> imageFiles;
  int imageIdx = 0;
  QString order = "rand";
  double x = 0.0;
  double y = 0.0;
  const QString durationStr;
  const QString hVelStr;
  const QString vVelStr;
  double hVel = 0.0;
  double vVel = 0.0;
  double scale = 1.0;
  
};

#endif // _GALLERY_H
