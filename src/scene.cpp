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
#include <QRegularExpression>

Scene::Scene(Settings &settings,
             const int &type,
             const QString &duration,
             const QString &background,
             const QString &foreground)
  : settings(settings), type(type)
{
  if(!duration.isNull() && duration.toInt() >= 500 && duration.toInt() <= 360000) {
    this->duration = duration.toInt();
  }
  if(!background.isNull()) {
    if(background == "random") {
      bgColorType = COLOR::RANDOM;
    } else if(settings.backgrounds.contains(background)) {
      this->background = settings.backgrounds[background];
    } else if(QRegularExpression("^#[0-9a-fA-F]{6}$").match(background).hasMatch()) {
      this->bgColor = QColor(background.mid(1, 2).toInt(Q_NULLPTR, 16),
                             background.mid(3, 2).toInt(Q_NULLPTR, 16),
                             background.mid(5, 2).toInt(Q_NULLPTR, 16));
    }
  }
  if(!foreground.isNull()) {
    if(foreground == "random") {
      fgColorType = COLOR::RANDOM;
    } else if(foreground == "complimentary") {
      fgColorType = COLOR::COMPLIMENTARY;
    } else if(QRegularExpression("^#[0-9a-fA-F]{6}$").match(foreground).hasMatch()) {
      this->fgColor = QColor(foreground.mid(1, 2).toInt(Q_NULLPTR, 16),
                             foreground.mid(3, 2).toInt(Q_NULLPTR, 16),
                             foreground.mid(5, 2).toInt(Q_NULLPTR, 16));
    }
  }
  buffer.fill(bgColor);

  connect(&frameTimer, &QTimer::timeout, this, &Scene::nextFrame);
  frameTimer.setSingleShot(true);
}

int Scene::getDuration()
{
  return duration;
}

void Scene::init(Scene *previousScene, Scene *nextScene)
{
  this->previousScene = previousScene;
  this->nextScene = nextScene;
  
  if(!running) {
    if(bgColorType == COLOR::RANDOM) {
      bgColor.setHsv(qrand() % 256,
                     (qrand() % 100) + 156,
                     50);
    }
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

QRect Scene::drawText(const int x, const int y, const QString font, const QString text,
                      const QColor color, const QList<int> spacing)
{
  QPainter painter;
  painter.begin(&buffer);
  painter.setRenderHint(QPainter::Antialiasing, false);

  QRect boundingRect(0, 0, 0, 0);
  int spacingIdx = 0;
  for(const auto &character: text) {
    QImage charImage = settings.fonts[font].getCharacter(character, color);
    painter.drawImage(x + boundingRect.width(), y, charImage);
    boundingRect.setWidth(boundingRect.width() + charImage.width() + spacing.value(spacingIdx, spacing.first()));
    if(charImage.height() > boundingRect.height()) {
      boundingRect.setHeight(charImage.height());
    }
    spacingIdx++;
  }
  painter.end();
  boundingRect.setWidth(boundingRect.width() - spacing.last());

  return boundingRect;
}

QImage Scene::getBuffer()
{
  return buffer;
}

int Scene::getType()
{
  return type;
}
