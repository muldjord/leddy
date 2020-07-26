/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            loader.cpp
 *
 *  Tue Nov 26 16:56:00 CEST 2013
 *  Copyright 2013 Lars Muldjord
 *  muldjordlars@gmail.com
 ****************************************************************************/

/*
 *  This file is part of Boris.
 *
 *  Boris is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Boris is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Boris; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */

#include "loader.h"

#include <stdio.h>

#include <QImage>

bool Loader::loadFont(QMap<QChar, QImage> &pfont)
{
  QImage fontSheet(":pfont.png");
  if(!fontSheet.isNull()) {
    QList<QChar> fontChars({'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', ' ', '.', ',', ':', ';', '!', '?', '\"', '\'', '_', '+', '-', '*', '/', '<', '>', '='});
    int w = fontSheet.width();
    int h = fontSheet.height();
    int x1 = 0;
    int x2 = 0;
    // Load all characters from png sprite sheet
    for(const auto &fontChar: fontChars) {
      // Look for first purple non-char pixel
      while(x2 < w && fontSheet.pixelIndex(x2, 0) != 2) {
        x2++;
      }
      pfont[fontChar] = fontSheet.copy(x1, 0, x2 - x1, h);
      // Move past purple non-char area to where next char begins
      while(x2 < w && fontSheet.pixelIndex(x2, 0) == 2) {
        x2++;
      }
      x1 = x2;
    }
  } else {
    return false;
  }
  return true;
}
