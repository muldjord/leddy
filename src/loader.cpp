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

bool Loader::loadFonts(Settings &settings, QMap<QString, PixelFont> &pixelFonts)
{
  printf("Loading fonts from '%s':\n", settings.fontPath.toStdString().c_str());
  QDirIterator dirIt(settings.fontPath,
                     QStringList({"*.png"}),
                     QDir::Files | QDir::NoDotAndDotDot,
                     QDirIterator::NoIteratorFlags);
  while(dirIt.hasNext()) {
    dirIt.next();
    PixelFont pixelFont(dirIt.fileInfo().baseName());
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
          pixelFont.addCharacter(character, spriteSheet.copy(x1, 0, x2 - x1, h));
          // Move past purple non-char area to where next char begins
          while(x2 < w  && qRed(scanLine[x2]) == 255 && qBlue(scanLine[x2]) == 255) {
            x2++;
          }
          x1 = x2;
        }
        pixelFonts[dirIt.fileInfo().baseName()] = pixelFont;
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
                     QStringList({"*.png"}),
                     QDir::Files | QDir::NoDotAndDotDot,
                     QDirIterator::NoIteratorFlags);
  while(dirIt.hasNext()) {
    dirIt.next();
    QString baseName = dirIt.fileInfo().baseName();
    QString animationName = baseName.left(baseName.indexOf("-"));
    int frameTime = 50;
    int duration = -1;
    int type = SC::LOOP;
    if(baseName.split("-").length() > 1) {
      frameTime = baseName.split("-").at(1).toInt(); 
    }
    if(baseName.split("-").length() > 2) {
      duration = baseName.split("-").at(2).toInt();
      printf("Set duration to %d\n", duration);
    }
    if(duration == -1) {
      type = SC::ONESHOT;
    }
  if(frameTime == -1) {
      frameTime = 50;
    }
    if(frameTime < 10) {
      frameTime = 10;
    }
    QImage spriteSheet(dirIt.filePath());
    if(spriteSheet.format() != QImage::Format_ARGB32) {
      spriteSheet = spriteSheet.convertToFormat(QImage::Format_ARGB32);
    }
    if(spriteSheet.width() % 16 != 0) {
      printf("WARNING: Animation sprite sheet '%s' does not adhere to 16 pixel width per sprite!\n", baseName.toStdString().c_str());
    }
    Animation *animation = new Animation(settings, type);
    if(duration != -1) {
      animation->setDuration(duration);
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
        animation->addFrame(frame);
      }
      printf("  Loaded '%s' (frame time %d)\n", animationName.toStdString().c_str(), frameTime);
      animations[animationName] = animation;
    }
  }
  
  return true;
}

bool Loader::loadTransitions(Settings &settings, QMap<QString, Transition *> &transitions)
{
  printf("Loading transitions from '%s':\n", settings.transitionPath.toStdString().c_str());
  QDirIterator dirIt(settings.transitionPath,
                     QStringList({"*.png"}),
                     QDir::Files | QDir::NoDotAndDotDot,
                     QDirIterator::NoIteratorFlags);
  while(dirIt.hasNext()) {
    dirIt.next();
    QString baseName = dirIt.fileInfo().baseName();
    QString transitionName = baseName.left(baseName.indexOf("-"));
    int frameTime = baseName.mid(baseName.indexOf("-") + 1).toInt();
    if(frameTime == -1) {
      frameTime = 50;
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
    Transition *transition = new Transition(settings);
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
      printf("  Loaded '%s' (frame time %d)\n", transitionName.toStdString().c_str(), frameTime);
      transitions[transitionName] = transition;
    }
  }
  
  return true;
}
