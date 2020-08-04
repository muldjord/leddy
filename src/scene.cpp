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

#include "scene.h"

#include <QPainter>

extern QMap<QString, PixelFont> fonts;

Scene::Scene(const Scene &scene) : QObject()
{
  this->type = scene.type;
  this->frameTime = scene.frameTime;
}

void Scene::operator=(const Scene &scene)
{
  this->type = scene.type;
  this->frameTime = scene.frameTime;
}

Scene::Scene(const QString &type, const int &frameTime)
{
  this->type = type;
  this->frameTime = frameTime;
  
  connect(&frameTimer, &QTimer::timeout, this, &Scene::nextFrame);
  frameTimer.setInterval(50);
  frameTimer.setSingleShot(true);
}

void Scene::init(const QImage &latestBuffer, Scene *nextScene, UniConn *uniConn)
{
  this->nextScene = nextScene;
  this->uniConn = uniConn;
  
  currentFrame = 0;
  // Draw initial frame into buffer
  emit frameReady(QImage(16, 16, QImage::Format_ARGB32));
  frameTimer.setInterval(10000);
  frameTimer.start();
}

void Scene::nextFrame()
{
  if(frames.isEmpty() || currentFrame >= frames.length()) {
    if(nextScene != nullptr && uniConn != nullptr) {
      disconnect(nextScene, &Scene::frameReady, nullptr, nullptr);
      connect(nextScene, &Scene::frameReady, uniConn, &UniConn::update);
    }
    nextScene = nullptr;
    uniConn = nullptr;
    emit sceneEnded();
  } else {
    emit frameReady(frames.at(currentFrame));
    currentFrame++;
    frameTimer.start();
  }
}

void Scene::update(QImage buffer) {
  this->newBuffer = buffer;
}

void Scene::addFrame(const QImage &frame)
{
  frames.append(frame);
}

void Scene::drawText(const int x, const int y, const QString font, const QString text,
                     const QColor color, const int spacing)
{
  QPainter painter;
  painter.begin(&mergedBuffer);
  painter.setRenderHint(QPainter::Antialiasing, false);

  int idx = x;
  for(const auto &character: text) {
    QImage charImage = fonts[font].getCharacter(character, color);
    painter.drawImage(idx, y, charImage);
    idx += charImage.width() + spacing;
  }
  painter.end();
}

QString Scene::getType()
{
  return type;
}
