/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            matrixada.cpp
 *
 *  Tue May 21 18:34:00 CEST 2024
 *  Copyright 2024 Lars Muldjord
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

#include "matrixada.h"
#include "globaldefs.h"

MatrixAda::MatrixAda(Settings &settings)
  : MatrixAbstract(settings)
{
}

MatrixAda::~MatrixAda(){
}

bool MatrixAda::init()
{
  rgb_matrix::RGBMatrix::Options defaults;
  defaults.hardware_mapping = "adafruit-hat";  // Configure to use the AdaFruit bonnet pin configuration
  defaults.cols = settings.width;
  defaults.rows = settings.height;
  defaults.chain_length = 1;
  defaults.parallel = 1;
  defaults.show_refresh_rate = false;
  rgb_matrix::RuntimeOptions runtimeOptions;
  canvas = rgb_matrix::RGBMatrix::CreateFromOptions(defaults, runtimeOptions);
  
  offscreenBuffer = canvas->CreateFrameCanvas();

  return true;
}

void MatrixAda::update(QImage buffer)
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

  for(int y = 0; y < buffer.height(); ++y) {
    for(int x = 0; x < buffer.width(); ++x) {
      QRgb currentRgb = buffer.pixel(x, y);
      offscreenBuffer->SetPixel(x,
                                y,
                                qRed(currentRgb) / (100.0 / settings.brightness),
                                qGreen(currentRgb) / (100.0 / settings.brightness),
                                qBlue(currentRgb) / (100.0 / settings.brightness));
    }
  }
  offscreenBuffer = canvas->SwapOnVSync(offscreenBuffer);
  /*
  if(buffer.width() != settings.width || buffer.height() != settings.height) {
    buffer = buffer.scaled(settings.width, settings.height, Qt::IgnoreAspectRatio, Qt::FastTransformation);
  }
  if(buffer.format() != QImage::Format_RGB888) {
    buffer = buffer.convertToFormat(QImage::Format_RGB888);
  }
  */
  
  /*
  if(settings.rotation != 0) {
    QTransform rotator;
    rotator.rotate(settings.rotation, Qt::ZAxis);
    buffer = buffer.transformed(rotator, Qt::FastTransformation);
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
    tr.speed_hz = settings.speed;
    tr.bits_per_word = settings.bits;
    
    if(ioctl(fd, SPI_IOC_MESSAGE(1), &tr) < 1) {
      printf("ERROR: SPI write failed!\n");
    }
  }
  */
}
