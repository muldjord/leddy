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
#include "pixelfont.h"

#include <QPainter>

extern QMap<QString, PixelFont> fonts;

Scene::Scene(Settings &settings, const int &sceneTime) : settings(settings)
{
  this->sceneTime = sceneTime;
  
  connect(&frameTimer, &QTimer::timeout, this, &Scene::nextFrame);
  frameTimer.setSingleShot(true);
}

int Scene::getSceneTime()
{
  return sceneTime;
}

void Scene::init(Scene *previousScene, Scene *nextScene)
{
  this->previousScene = previousScene;
  this->nextScene = nextScene;
  
  // TODO: Draw initial frame into buffer here
  emit frameReady(buffer);

  if(!running) {
    running = true;
    currentFrame = 0;
    if(!frames.isEmpty()) {
      frameTimer.setInterval(frames.at(currentFrame).first);
      frameTimer.start();
    }
  }
}

void Scene::nextFrame()
{
  if(frames.isEmpty() || currentFrame >= frames.length()) {
    running = false;
    emit sceneEnded();
  } else {
    emit frameReady(frames.at(currentFrame).second);
    currentFrame++;
    frameTimer.setInterval(frames.at(currentFrame).first);
    frameTimer.start();
  }
}

void Scene::addFrame(const QPair<int, QImage> &frame)
{
  frames.append(frame);
}

void Scene::drawText(const int x, const int y, const QString font, const QString text,
                     const QColor color, const int spacing)
{
  QPainter painter;
  painter.begin(&buffer);
  painter.setRenderHint(QPainter::Antialiasing, false);

  int idx = x;
  for(const auto &character: text) {
    QImage charImage = fonts[font].getCharacter(character, color);
    painter.drawImage(idx, y, charImage);
    idx += charImage.width() + spacing;
  }
  painter.end();
}

QImage Scene::getBuffer()
{
  return buffer;
}