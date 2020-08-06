/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            textscroll.h
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

#include "textscroll.h"

TextScroll::TextScroll(Settings &settings, const QString &text)
  : Scene(settings), text(text) 
{
}

void TextScroll::start()
{
  currentX = 17;

  if(settings.rssLines.isEmpty()) {
    text = "RSS feed URL from 'config.ini' didn't return any entries. Please check it and / or verify your network connection.";
  } else {
    int random = qrand() % settings.rssLines.length();
    text = settings.rssLines.at(random);
  }
  frameTimer.setInterval(40);
  nextFrame();
}

void TextScroll::nextFrame()
{
  buffer.fill(QColor(Qt::black));
  int textWidth = drawText(currentX, 2, "medium", text, QColor(Qt::white), 1);

  if(currentX < - textWidth) {
    running = false;
    emit sceneEnded();
    return;
  }
  currentX--;
  frameTimer.start();
}
