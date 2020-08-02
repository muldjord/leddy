/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            uniconn.h
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

#ifndef _UNICONN_H
#define _UNICONN_H

#include "settings.h"
#include "pixelfont.h"

#include <stdint.h>

#include <QObject>
#include <QImage>
#include <QTimer>
#include <QEventLoop>

class UniConn : public QObject
{
  Q_OBJECT

public:
  UniConn(Settings &settings);
  ~UniConn();
  bool init();

  void beginScene(const QColor color = QColor(Qt::black));
  void showScene(const int transition);
  
  void drawImage(const int x, const int y, const QImage image);
  void drawPixel(const int x, const int y, const QColor color);
  void drawText(const int x, const int y, const QString font, const QString text,
                const QColor color = QColor(Qt::white), const int spacing = 0);

signals:
  void sceneReady();
  
private:
  Settings &settings;
  QMap<QString, PixelFont> fonts;

  /*
  QTimer limitTimer;
  QEventLoop limiter;
  */
  QImage currentScene = QImage(16, 16, QImage::Format_RGB888);
  QImage nextScene = QImage(16, 16, QImage::Format_RGB888);
  
  bool isOpen = false;
  int fd;
};

#endif // _UNICONN_H
