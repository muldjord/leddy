/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            matrixuni.cpp
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

#include "matrixuni.h"
#include "globaldefs.h"

#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

MatrixUni::MatrixUni(Settings &settings) : MatrixAbstract(settings)
{
}

MatrixUni::~MatrixUni(){
  if(isOpen) {
    printf("Closing SPI connection.\n");
    close(fd);
  }
}

bool MatrixUni::init()
{
  fd = open(settings.uniSpiDevice.data(), O_RDWR);
  if(fd < 0) {
    printf("ERROR: Couldn't open SPI device '%s'\n", settings.uniSpiDevice.data());
    return false;
  }

  if(ioctl(fd, SPI_IOC_WR_MODE, &settings.uniSpiMode) == -1) {
    printf("ERROR: Couldn't set SPI mode to '%d'\n", settings.uniSpiMode);
    return false;
  }

  if(ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &settings.uniSpiBits) == -1) {
    printf("ERROR: Couldn't set bits per word to '%d'.\n", settings.uniSpiBits);
    return false;
  }

  if(ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &settings.uniSpiSpeed) == -1) {
    printf("ERROR: Couldn't set max speed to '%d' Hz\n", settings.uniSpiSpeed);
    return false;
  }

  printf("Successfully initialized SPI connection:\n");
  printf("  Device     : '%s'\n", settings.uniSpiDevice.data());
  printf("  Mode       : %d\n", settings.uniSpiMode);
  printf("  Bits       : %d\n", settings.uniSpiBits);
  printf("  Speed (kHz): %d\n", settings.uniSpiSpeed / 1000);
  
  isOpen = true;

  return true;
}

void MatrixUni::update(QImage buffer)
{
  if(settings.rotation != 0) {
    QTransform rotator;
    rotator.rotate(settings.rotation, Qt::ZAxis);
    buffer = buffer.transformed(rotator, Qt::FastTransformation);
  }

  if(buffer.width() != settings.width || buffer.height() != settings.height) {
    buffer = buffer.scaled(settings.width, settings.height, Qt::IgnoreAspectRatio, Qt::FastTransformation);
  }
  if(buffer.format() != QImage::Format_RGB888) {
    buffer = buffer.convertToFormat(QImage::Format_RGB888);
  }
  
  if(isOpen) {
    uint32_t len = 1 + (settings.width * settings.height * 3); // Start-byte + size of settings.width x settings.height RGB LED's, 3 bytes per pixel
    uint8_t tx[len] = { 0x72 };
    for(uint32_t a = 1; a < len; ++a) {
      tx[a] = (uint8_t)buffer.constBits()[a - 1] / (100.0 / settings.brightness);
    }
    
    struct spi_ioc_transfer tr;
    memset(&tr, 0, sizeof(spi_ioc_transfer)); // Init all to zero to avoid unknown behaviour
    tr.tx_buf = (unsigned long)tx;
    tr.len = len;
    //tr.delay_usecs = delay;
    tr.speed_hz = settings.uniSpiSpeed;
    tr.bits_per_word = settings.uniSpiBits;
    
    if(ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 1) {
      printf("ERROR: SPI write failed!\n");
    }
  }
}
