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

#include <stdio.h>

#include <QImage>
#include <QDirIterator>

bool Loader::loadFonts(const QString &path, QMap<QString, PixelFont> &pixelFonts)
{
  QDirIterator dirIt(path,
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
        descriptorFile.close();
      } else {
        printf("ERROR: Font '%s' is missing '%s' descriptor, can't load!", dirIt.fileName().toStdString().c_str(), descriptorFileName.toStdString().c_str());
        return false;
      }
    }
  }
  
  return true;
}

bool Loader::loadTransitions(const QString &path)
{
  return true;
}
