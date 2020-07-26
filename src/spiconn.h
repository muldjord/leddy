/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            spiconn.h
 *
 *  Fri Jul 24 12:00:00 CEST 2020
 *  Copyright 2020 Lars Muldjord
 *  muldjordlars@gmail.com
 ****************************************************************************/
/*
 *  This file is part of leddy.
 *
 *  leddy is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  leddy is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with leddy; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */

#ifndef SPICONN_H
#define SPICONN_H

#include <stdint.h>
#include <QObject>
#include <QImage>
#include <QTimer>
#include <QEventLoop>

class SPIConn : public QObject
{
  Q_OBJECT

public:
  SPIConn(QByteArray device, uint32_t speed, uint8_t mode, uint8_t bits,
	  int brightness = 50, int rotation = 0);
  ~SPIConn();
  bool init();
  bool refresh();
  void clear(const QColor color = QColor(Qt::black));
  void setFromImage(const QImage image);
  void setPixel(const int x, const int y, const QColor color);
  void drawText(const int x, const int y,
		const QString text, const int spacing = 0, const QColor color = QColor(Qt::white));
  
private:
  QTimer limitTimer;
  QEventLoop limiter;
  QImage buffer = QImage(16, 16, QImage::Format_RGB888);
  QByteArray device = "/dev/spidev0.0";
  uint8_t bits = 8;
  uint8_t mode = 0;
  uint32_t speed = 9000000;
  int rotation = 0;
  int brightness = 50;
  
  bool isOpen = false;
  int fd;
};

#endif // SPICONN_H
