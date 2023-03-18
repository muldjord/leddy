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

#include "gallery.h"

#include <cmath>

#include <QDir>
#include <QFileInfo>
#include <QRandomGenerator>

Gallery::Gallery(Settings &settings,
                 const QString &duration,
                 const QString &input,
                 const QString &order,
                 const QString &hVel,
                 const QString &vVel,
                 const QString &scale)
  : Scene(settings, SCENE::GALLERY, duration), order(order), durationStr(duration), hVelStr(hVel), vVelStr(vVel)
{
  QFileInfo inputInfo(input);
  if(!inputInfo.exists()) {
    inputInfo.setFile(settings.themePath + "/" + input);
  }
  if(inputInfo.isDir()) {
    QDir inputDir(inputInfo.absoluteFilePath(), "*.png *.jpg");
    if(order == "name") {
      inputDir.setSorting(QDir::Name | QDir::IgnoreCase);
    } else if(order == "time") {
      inputDir.setSorting(QDir::Time);
    }
    imageFiles = inputDir.entryInfoList();
  } else if(inputInfo.isFile()) {
    QFile inputFile(inputInfo.absoluteFilePath());
    if(inputFile.open(QIODevice::ReadOnly)) {
      while(!inputFile.atEnd()) {
        QFileInfo imageInfo(QString::fromUtf8(inputFile.readLine().trimmed()));
        if(imageInfo.exists()) {
          imageFiles.append(imageInfo);
        }
        inputFile.close();
      }
    }
  }

  if(!hVel.isNull() && hVel != "rand") {
    this->hVel = hVel.toDouble();
  }
  if(!vVel.isNull() && vVel != "rand") {
    this->vVel = vVel.toDouble();
  }
  if(!scale.isNull()) {
    this->scale = scale.toDouble();
  }

  galleryTimer.setSingleShot(true);
  connect(&galleryTimer, &QTimer::timeout, this, &Gallery::nextImage);

  frameTimer.setInterval(1000 / 30);
}

void Gallery::start()
{
  if(durationStr == "rand" ||
     durationStr.isNull() ||
     durationStr.isEmpty() ||
     duration == DURATION::ONESHOT) {
    setDuration(QRandomGenerator::global()->bounded(5000) + 5000);
  }

  if(hVelStr == "rand") {
    this->hVel = (QRandomGenerator::global()->bounded(6000) - 3000) / 1000.0;
  }
  if(vVelStr == "rand") {
    this->vVel = (QRandomGenerator::global()->bounded(6000) - 3000) / 1000.0;
  }

  if(!imageFiles.isEmpty()) {
    if(order == "rand") {
      printf("Choosing random image\n");
      imageIdx = QRandomGenerator::global()->bounded(imageFiles.count());
    } else {
      imageIdx++;
      if(imageIdx >= imageFiles.count()) {
        imageIdx = 0;
      }
    }
    image = QImage(imageFiles.at(imageIdx).absoluteFilePath());
  } else {
    image = QImage(MATRIX::WIDTH, MATRIX::HEIGHT, QImage::Format_RGB32);
    image.fill(Qt::black);
  }

  image = image.convertToFormat(QImage::Format_RGB32);
  
  if(scale != 1.0) {
    image = image.scaledToWidth(image.width() * scale);
  }

  if(image.width() < MATRIX::WIDTH) {
    image = image.scaledToWidth(MATRIX::WIDTH);
  }
  if(image.height() < MATRIX::HEIGHT) {
    image = image.scaledToHeight(MATRIX::HEIGHT);
  }

  x = QRandomGenerator::global()->bounded((image.width() - MATRIX::WIDTH + 1));
  y = QRandomGenerator::global()->bounded((image.height() - MATRIX::HEIGHT + 1));

  galleryTimer.setInterval(getDuration());
  galleryTimer.start();
  nextFrame();
}

void Gallery::nextFrame()
{
  buffer = image.copy(x, y, MATRIX::WIDTH, MATRIX::HEIGHT);

  x += hVel / (1000.0 / (double)frameTimer.interval());
  y += vVel / (1000.0 / (double)frameTimer.interval());

  if(x + MATRIX::WIDTH > image.width()) {
    x = 0;
  } else if(x < 0) {
    x = image.width() - MATRIX::WIDTH;
  }

  if(y + MATRIX::HEIGHT > image.height()) {
    y = 0;
  } else if(y < 0) {
    y = image.height() - MATRIX::HEIGHT;
  }

  frameTimer.start();
}

void Gallery::nextImage()
{
  running = false;
  emit sceneEnded();
}
