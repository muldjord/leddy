/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            loader.cpp
 *
 *  Fri Jul 24 12:00:00 CEST 2020
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

#include "loader.h"
#include "globaldefs.h"

#include <stdio.h>

#include <QImage>
#include <QDirIterator>
#include <QMovie>

bool Loader::loadFonts(Settings &settings)
{
  printf("Loading fonts from '%s':\n", settings.fontPath.toStdString().c_str());
  QDirIterator dirIt(settings.fontPath,
                     QStringList({"*.png"}),
                     QDir::Files | QDir::NoDotAndDotDot,
                     QDirIterator::NoIteratorFlags);
  while(dirIt.hasNext()) {
    dirIt.next();
    PixelFont font(dirIt.fileInfo().baseName());
    QImage spriteSheet(dirIt.filePath());
    spriteSheet = spriteSheet.convertToFormat(QImage::Format_ARGB32);
    QString descriptorFileName = dirIt.fileInfo().baseName() + ".txt";
    if(!spriteSheet.isNull()) {
      QFile descriptorFile(dirIt.fileInfo().absolutePath() + "/" + descriptorFileName);
      if(descriptorFile.open(QIODevice::ReadOnly) && !descriptorFile.atEnd()) {
        QString characterString = QString::fromUtf8(descriptorFile.readLine()).simplified();
        QList<QChar> characterList;
        for(int a = 0; a < characterString.length(); ++a) {
          characterList.append(characterString.at(a));
        }
        int w = spriteSheet.width();
        int h = spriteSheet.height();
        int x1 = 0;
        int x2 = 0;
        // Load all characters from png sprite spriteSheet
        for(const auto &character: characterList) {
          // Look for first purple non-char pixel
          const QRgb *scanLine = (QRgb *)spriteSheet.constScanLine(0);
          while(x2 < w && qRed(scanLine[x2]) != 255 && qBlue(scanLine[x2]) != 255) {
            x2++;
          }
          font.addCharacter(character, spriteSheet.copy(x1, 0, x2 - x1, h));
          // Move past purple non-char area to where next char begins
          while(x2 < w  && qRed(scanLine[x2]) == 255 && qBlue(scanLine[x2]) == 255) {
            x2++;
          }
          x1 = x2;
        }
        settings.fonts[dirIt.fileInfo().baseName()] = font;
        printf("  Loaded '%s'\n", dirIt.fileInfo().baseName().toStdString().c_str());
        descriptorFile.close();
      } else {
        printf("ERROR: Font '%s' is missing '%s' descriptor, can't load!", dirIt.fileName().toStdString().c_str(), descriptorFileName.toStdString().c_str());
        return false;
      }
    }
  }
  
  return true;
}

bool Loader::loadAnimations(Settings &settings, QMap<QString, Animation *> &animations)
{
  printf("Loading animations from '%s':\n", settings.animationPath.toStdString().c_str());
  QDirIterator dirIt(settings.animationPath,
                     QStringList({"*.png", "*.gif"}),
                     QDir::Files | QDir::NoDotAndDotDot,
                     QDirIterator::NoIteratorFlags);
  while(dirIt.hasNext()) {
    dirIt.next();
    QString extension = dirIt.fileInfo().suffix();
    QString baseName = dirIt.fileInfo().baseName();
    QString animationName = baseName.left(baseName.indexOf("-"));
    Animation *animation = new Animation(settings);
    if(extension == "gif") {
      QMovie gifFile(dirIt.filePath());
      for(int a = 0; a < gifFile.frameCount(); ++a) {
        gifFile.jumpToFrame(a);
        QImage sprite(gifFile.currentImage());
        sprite = sprite.convertToFormat(QImage::Format_ARGB32);
        if(sprite.width() != 16 || sprite.height() != 16) {
          sprite = sprite.scaled(16, 16, Qt::IgnoreAspectRatio, Qt::FastTransformation);
        }
        QPair<int, QImage> frame;
        frame.first = gifFile.nextFrameDelay();
        frame.second = sprite;
        animation->addFrame(frame);
      }
    } else {
      QImage spriteSheet(dirIt.filePath());
      if(!spriteSheet.isNull()) {
        if(spriteSheet.format() != QImage::Format_ARGB32) {
          spriteSheet = spriteSheet.convertToFormat(QImage::Format_ARGB32);
        }
        if(spriteSheet.width() % 16 != 0) {
          printf("WARNING: Animation sprite sheet '%s' does not adhere to 16 pixel width per sprite!\n", baseName.toStdString().c_str());
        }
        for(int a = 0; a < spriteSheet.width(); a = a + 16) {
          QImage sprite = spriteSheet.copy(a, 0, 16, 16);
          if(sprite.width() != 16 || sprite.height() != 16) {
            sprite = sprite.scaled(16, 16, Qt::IgnoreAspectRatio, Qt::FastTransformation);
          }
          int frameTime = 50;
          if(baseName.split("-").length() > 1) {
            frameTime = baseName.split("-").at(1).toInt(); 
          }
          if(frameTime < 10) {
            frameTime = 10;
          }
          QPair<int, QImage> frame;
          frame.first = frameTime;
          frame.second = sprite;
          animation->addFrame(frame);
        }
      }
    }
    printf("  Loaded '%s'\n", animationName.toStdString().c_str());
    animations[animationName] = animation;
  }
  
  return true;
}

bool Loader::loadTransitions(Settings &settings, QMap<QString, Transition *> &transitions)
{
  printf("Loading transitions from '%s':\n", settings.transitionPath.toStdString().c_str());
  QDirIterator dirIt(settings.transitionPath,
                     QStringList({"*.png", "*.gif"}),
                     QDir::Files | QDir::NoDotAndDotDot,
                     QDirIterator::NoIteratorFlags);
  while(dirIt.hasNext()) {
    dirIt.next();
    QString extension = dirIt.fileInfo().suffix();
    QString baseName = dirIt.fileInfo().baseName();
    QString transitionName = baseName.left(baseName.indexOf("-"));
    Transition *transition = new Transition(settings);
    if(extension == "gif") {
      QMovie gifFile(dirIt.filePath());
      for(int a = 0; a < gifFile.frameCount(); ++a) {
        gifFile.jumpToFrame(a);
        QImage sprite(gifFile.currentImage());
        sprite = sprite.convertToFormat(QImage::Format_ARGB32);
        if(sprite.width() != 16 || sprite.height() != 16) {
          sprite = sprite.scaled(16, 16, Qt::IgnoreAspectRatio, Qt::FastTransformation);
        }
        QPair<int, QImage> frame;
        frame.first = gifFile.nextFrameDelay();
        frame.second = sprite;
        transition->addFrame(frame);
      }
    } else {
      int frameTime = 50;
      if(baseName.split("-").length() > 1) {
        frameTime = baseName.split("-").at(1).toInt(); 
      }
      if(frameTime < 10) {
        frameTime = 10;
      }
      QImage spriteSheet(dirIt.filePath());
      if(spriteSheet.format() != QImage::Format_ARGB32) {
        spriteSheet = spriteSheet.convertToFormat(QImage::Format_ARGB32);
      }
      if(spriteSheet.width() % 16 != 0) {
        printf("WARNING: Transition sprite sheet '%s' does not adhere to 16 pixel width per sprite!\n", baseName.toStdString().c_str());
      }
      if(!spriteSheet.isNull()) {
        for(int a = 0; a < spriteSheet.width(); a = a + 16) {
          QImage sprite = spriteSheet.copy(a, 0, 16, 16);
          if(sprite.format() != QImage::Format_ARGB32) {
            sprite = sprite.convertToFormat(QImage::Format_ARGB32);
          }
          QPair<int, QImage> frame;
          frame.first = frameTime;
          frame.second = sprite;
          transition->addFrame(frame);
        }
      }
    }
    printf("  Loaded '%s'\n", transitionName.toStdString().c_str());
    transitions[transitionName] = transition;
  }
  
  return true;
}

bool Loader::loadBackgrounds(Settings &settings)
{
  printf("Loading backgrounds from '%s':\n", settings.backgroundPath.toStdString().c_str());
  QDirIterator dirIt(settings.backgroundPath,
                     QStringList({"*.png", "*.gif"}),
                     QDir::Files | QDir::NoDotAndDotDot,
                     QDirIterator::NoIteratorFlags);
  while(dirIt.hasNext()) {
    dirIt.next();
    QString backgroundName = dirIt.fileInfo().baseName();
    QImage background(dirIt.filePath());
    if(background.format() != QImage::Format_ARGB32) {
      background = background.convertToFormat(QImage::Format_ARGB32);
    }
    if(background.width() != 16 || background.height() != 16) {
      background = background.scaled(16, 16, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    }
    if(!background.isNull()) {
      printf("  Loaded '%s'\n", backgroundName.toStdString().c_str());
      settings.backgrounds.setBackground(backgroundName, background);
    }
  }
  
  return true;
}

bool Loader::loadWeatherIcons(Settings &settings)
{
  printf("Loading weather icons from '%s':\n", settings.weatherIconPath.toStdString().c_str());
  QDirIterator dirIt(settings.weatherIconPath,
                     QStringList({"*.png"}),
                     QDir::Files | QDir::NoDotAndDotDot,
                     QDirIterator::NoIteratorFlags);
  while(dirIt.hasNext()) {
    dirIt.next();
    QString weatherIconName = dirIt.fileInfo().baseName();
    QImage weatherIcon(dirIt.filePath());
    if(weatherIcon.format() != QImage::Format_ARGB32) {
      weatherIcon = weatherIcon.convertToFormat(QImage::Format_ARGB32);
    }
    if(weatherIcon.width() != 16 || weatherIcon.height() != 16) {
      weatherIcon = weatherIcon.scaled(16, 16, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    }
    if(!weatherIcon.isNull()) {
      printf("  Loaded '%s'\n", weatherIconName.toStdString().c_str());
      settings.icons[weatherIconName] = weatherIcon;
    }
  }
  
  return true;
}
