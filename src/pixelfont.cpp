/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            pixelfont.h
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

#include "pixelfont.h"

PixelFont::PixelFont(const PixelFont &font) : QObject()
{
  this->name = font.name;
  this->characters = font.characters;
}

void PixelFont::operator=(const PixelFont &font)
{
  this->name = font.name;
  this->characters = font.characters;
}

PixelFont::PixelFont(const QString &name)
{
  this->name = name;
}

int PixelFont::getHeight()
{
  if(!characters.isEmpty()) {
    return characters.first().height();
  }
  return 0;
}

void PixelFont::addCharacter(const QChar &character, const QImage &image)
{
  characters[character] = image;
}

bool PixelFont::hasCharacter(const QChar &character)
{
  if(characters.contains(character)) {
    return true;
  }

  return false;
}

QImage PixelFont::getCharacter(const QChar &character, const QColor &color)
{
  QImage image;

  if(characters.contains(character)) {
    image = characters[character];
  } else if(!characters.isEmpty()) {
    QImage noCharacter(characters.first().width(),
                       characters.first().height(),
                       QImage::Format_ARGB32);
    noCharacter.fill(Qt::red);
    
    image = noCharacter;
  }
  
  // Change the color of all non-alpha channels to the requested color
  QRgb* bits = (QRgb *)image.bits();
  for(int a = 0; a < image.width() * image.height(); ++a) {
    int alpha = qAlpha(bits[a]) - (255 - color.alpha());
    if(alpha < 0) {
      alpha = 0;
    }
    bits[a] = qRgba(color.red(),
                    color.green(),
                    color.blue(),
                    alpha);
  }

  return image;
}

int PixelFont::getCharacterWidth(const QChar &character)
{
  if(characters.contains(character)) {
    return characters[character].width();
  }
  return characters.first().width();
}
