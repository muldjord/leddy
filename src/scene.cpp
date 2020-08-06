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

Scene::Scene(Settings &settings, const int &type) : settings(settings)
{
  this->type = type;
  
  buffer.fill(bgColor);

  connect(&frameTimer, &QTimer::timeout, this, &Scene::nextFrame);
  frameTimer.setSingleShot(true);
}

void Scene::setDuration(const int &duration) {
  this->duration = duration;
}

int Scene::getDuration()
{
  return duration;
}

void Scene::init(Scene *previousScene, Scene *nextScene)
{
  this->previousScene = previousScene;
  this->nextScene = nextScene;
  
  bgColor = QColor(qrand() % 100, qrand() % 100, qrand() % 100);
  
  if(!running) {
    running = true;
    currentFrame = 0;
    start();
  }
}

void Scene::start()
{
  nextFrame();
}

void Scene::nextFrame()
{
  if(currentFrame >= frames.length()) {
    running = false;
    emit sceneEnded();
    return;
  }

  buffer = frames.at(currentFrame).second;

  frameTimer.setInterval(frames.at(currentFrame).first);
  currentFrame++;
  frameTimer.start();
}

void Scene::addFrame(const QPair<int, QImage> &frame)
{
  frames.append(frame);
}

int Scene::drawText(const int x, const int y, const QString font, const QString text,
                     const QColor color, const int spacing)
{
  QPainter painter;
  painter.begin(&buffer);
  painter.setRenderHint(QPainter::Antialiasing, false);

  int idx = x;
  for(const auto &character: text) {
    QImage charImage = settings.fonts[font].getCharacter(character, color);
    painter.drawImage(idx, y, charImage);
    idx += charImage.width() + spacing;
  }
  painter.end();

  return idx - x;
}

QImage Scene::getBuffer()
{
  return buffer;
}

int Scene::getType()
{
  return type;
}
