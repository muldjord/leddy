/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            spiconn.cpp
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

#include "spiconn.h"

#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <string.h>
#include <linux/spi/spidev.h>

#include <QPainter>
#include <QTransform>
#include <QMap>

extern QMap<QChar, QImage> pfont;

SPIConn::SPIConn(QByteArray device, uint32_t speed, uint8_t mode, uint8_t bits,
		 int brightness, int rotation)
{
  this->device = device;
  this->speed = speed;
  this->mode = mode;
  this->bits = bits;
  this->rotation = rotation;
  this->brightness = brightness;
  
  buffer.fill(Qt::black);

  connect(&limitTimer, &QTimer::timeout, &limiter, &QEventLoop::quit);
  limitTimer.setInterval(10);
  limitTimer.setSingleShot(false);
  limitTimer.start();
}

SPIConn::~SPIConn(){
  if(isOpen) {
    printf("Closing SPI connection.\n");
    close(fd);
  }
}

bool SPIConn::init()
{
  fd = open(device.data(), O_RDWR);
  if(fd < 0) {
    printf("ERROR: Couldn't open SPI device '%s'\n", device.data());
    return false;
  }

  if(ioctl(fd, SPI_IOC_WR_MODE, &mode) == -1) {
    printf("ERROR: Couldn't set SPI mode to '%d'\n", mode);
    return false;
  }

  if(ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) == -1) {
    printf("ERROR: Couldn't set bits per word to '%d'.\n", bits);
    return false;
  }

  if(ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) == -1) {
    printf("ERROR: Couldn't set max speed to '%d' Hz\n", speed);
    return false;
  }

  printf("Successfully initialized SPI connection:\n");
  printf("  Device     : '%s'\n", device.data());
  printf("  Mode       : %d\n", mode);
  printf("  Bits       : %d\n", bits);
  printf("  Speed (kHz): %d\n", speed / 1000);
  
  isOpen = true;

  return true;
}

void SPIConn::clear(const QColor color)
{
  buffer.fill(color);
}

void SPIConn::setFromImage(const QImage image)
{
  buffer = image;
}

void SPIConn::setPixel(const int x, const int y, const QColor color)
{
  buffer.setPixelColor(x, y, color);
}

void SPIConn::drawText(const int x, const int y,
		       const QString text, const int spacing, const QColor color)
{
  QPainter painter;
  painter.begin(&buffer);
  painter.setRenderHint(QPainter::Antialiasing, false);

  int idx = x;
  for(const auto &textChar: text) {
    QImage charImage;
    if(pfont.contains(textChar)) {
      charImage = pfont[textChar];
    } else {
      charImage = QImage(5, 4, QImage::Format_RGB888);
      charImage.fill(Qt::red);
    }
    QColor randColor(qrand() % 255, qrand() % 255, qrand() % 255);
    painter.setPen(QPen(randColor));
    charImage.setColor(1, painter.pen().color().rgb());

    painter.drawImage(idx, y, charImage);
    idx += charImage.width() + spacing;
  }
  painter.end();
}

bool SPIConn::refresh()
{
  if(buffer.width() != 16 || buffer.height() != 16) {
    buffer = buffer.scaled(16, 16, Qt::IgnoreAspectRatio, Qt::FastTransformation);
  }
  if(buffer.format() != QImage::Format_RGB888) {
    buffer = buffer.convertToFormat(QImage::Format_RGB888);
  }
  
  if(rotation != 0) {
    QTransform rotator;
    rotator.rotate(rotation, Qt::ZAxis);
    buffer = buffer.transformed(rotator, Qt::FastTransformation);
  }
  uint32_t len = 1 + (16 * 16 * 3); // Start-byte + size of 16x16 RGB LED's
  uint8_t tx[len] = { 0x72 };
  for(uint32_t a = 1; a < len; ++a) {
    tx[a] = (uint8_t)buffer.constBits()[a - 1] / (100.0 / brightness);
  }
  
  struct spi_ioc_transfer tr;
  memset(&tr, 0, sizeof(spi_ioc_transfer)); // Init all to zero to avoid unknown behaviour
  tr.tx_buf = (unsigned long)tx;
  tr.len = len;
  //tr.delay_usecs = delay;
  tr.speed_hz = speed;
  tr.bits_per_word = bits;

  if(ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 1) {
    printf("ERROR: SPI write failed!\n");
    return false;
  }

  limiter.exec(); // Wait while the LEDs are updated before allowing a new refresh
  
  return true;
}
