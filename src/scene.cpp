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
#include <QRandomGenerator>

Scene::Scene(Settings &settings,
             const int &type,
             const QString &duration,
             const QString &background,
             const QString &bgColor,
             const QString &fgColor)
  : settings(settings), type(type)
{
  if(!duration.isNull() && duration.toInt() != 0) {
    setDuration(duration.toInt());
  }
  if(!background.isNull()) {
    this->background = settings.backgrounds.getBackground(background);
  }
  if(!bgColor.isNull() && background.isNull()) {
    if(bgColor == "random") {
      bgColorType = COLOR::RANDOM;
    } else if(QRegularExpression("^#[0-9a-fA-F]{6}$").match(bgColor).hasMatch()) {
      bgColorType = COLOR::STATIC;
      this->bgColor = QColor(bgColor.mid(1, 2).toInt(Q_NULLPTR, 16),
                             bgColor.mid(3, 2).toInt(Q_NULLPTR, 16),
                             bgColor.mid(5, 2).toInt(Q_NULLPTR, 16));
    }
  }
  if(!fgColor.isNull()) {
    if(fgColor == "random") {
      fgColorType = COLOR::RANDOM;
    } else if(fgColor == "complimentary") {
      fgColorType = COLOR::COMPLIMENTARY;
    } else if(QRegularExpression("^#[0-9a-fA-F]{6}$").match(fgColor).hasMatch()) {
      fgColorType = COLOR::STATIC;
      this->fgColor = QColor(fgColor.mid(1, 2).toInt(Q_NULLPTR, 16),
                             fgColor.mid(3, 2).toInt(Q_NULLPTR, 16),
                             fgColor.mid(5, 2).toInt(Q_NULLPTR, 16));
    } else if(QRegularExpression("^#[0-9a-fA-F]{8}$").match(fgColor).hasMatch()) {
      fgColorType = COLOR::STATIC;
      this->fgColor = QColor(fgColor.mid(1, 2).toInt(Q_NULLPTR, 16),
                             fgColor.mid(3, 2).toInt(Q_NULLPTR, 16),
                             fgColor.mid(5, 2).toInt(Q_NULLPTR, 16),
                             fgColor.mid(7, 2).toInt(Q_NULLPTR, 16));
    }
  }
  buffer.fill(bgColor);

  connect(&frameTimer, &QTimer::timeout, this, &Scene::nextFrame);
  frameTimer.setSingleShot(true);
}

void Scene::setDuration(const int &duration)
{
  this->duration = duration;
  if(duration < 500 && duration != DURATION::ONESHOT) {
    this->duration = 500;
  } else if(duration > 360000) {
    this->duration = 360000;
  }
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
      bgColor = QColor::fromHsl(QRandomGenerator::global()->bounded(360),
                                255,
                                50);
    }
    if(fgColorType == COLOR::RANDOM) {
      fgColor = QColor::fromHsl(QRandomGenerator::global()->bounded(360),
                                255,
                                200);
    } else if(fgColorType == COLOR::COMPLIMENTARY) {
      fgColor = QColor::fromHsl((bgColor.hslHue() + 180 > 359?180 + bgColor.hslHue() - 359:bgColor.hslHue() + 180),
                                bgColor.hslSaturation(),
                                200);
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

void Scene::checkResult(const QString, const QString)
{
}

void Scene::addFrame(const QPair<int, QImage> &frame)
{
  frames.append(frame);
}

QRect Scene::drawText(const int x, const int y, const QString font, const QString text,
                      const QColor color, const QColor shadowColor, const int vAlign, const QList<int> spacing)
{
  // First figure out the bounding rectangle of the text
  QRect boundingRect(0, 0, 0, 0);
  int spacingIdx = 0;
  for(const auto &character: text) {
    boundingRect.setWidth(boundingRect.width() + settings.fonts[font].getCharacterWidth(character) + spacing.value(spacingIdx, spacing.first()));
    if(settings.fonts[font].getHeight() > boundingRect.height()) {
      boundingRect.setHeight(settings.fonts[font].getHeight());
    }
    spacingIdx++;
  }
  boundingRect.setWidth(boundingRect.width() - spacing.last());
  if(shadowColor.alpha()) {
    boundingRect.setWidth(boundingRect.width() + 1);
    boundingRect.setHeight(boundingRect.height() + 1);
  }

  // Now draw the text on the buffer in the requested location
  int alignedX = x;
  if(vAlign == VALIGN::CENTER) {
    alignedX -= boundingRect.width() / 2;
  } else if(vAlign == VALIGN::RIGHT) {
    alignedX -= boundingRect.width();
  }
  QPainter painter;
  painter.begin(&buffer);
  for(const auto &character: text) {
    if(shadowColor.alpha()) {
      QImage shadowCharImage = settings.fonts[font].getCharacter(character, shadowColor);
      painter.drawImage(alignedX + 1, y + 1, shadowCharImage);
    }
    QImage charImage = settings.fonts[font].getCharacter(character, color);
    painter.drawImage(alignedX, y, charImage);
    alignedX += charImage.width() + spacing.value(spacingIdx, spacing.first());
    spacingIdx++;
  }
  painter.setRenderHint(QPainter::Antialiasing, false);

  painter.end();

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
