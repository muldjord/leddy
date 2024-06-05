/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            matrixada.h
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

#ifndef _MATRIXADA_H
#define _MATRIXADA_H

#include "matrixabstract.h"

#include "led-matrix.h"
//using rgb_matrix::RGBMatrix;
//using rgb_matrix::FrameCanvas;

class MatrixAda : public MatrixAbstract
{
  Q_OBJECT

public:
  MatrixAda(Settings &settings);
  ~MatrixAda() override;
  bool init() override;

public slots:
  void update(QImage buffer) override;

private:
  rgb_matrix::RGBMatrix *canvas = nullptr;
  rgb_matrix::FrameCanvas *offscreenBuffer = nullptr;
};

#endif // _MATRIXADA_H
