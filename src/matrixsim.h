/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            matrixsim.h
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

#ifndef _MATRIXSIM_H
#define _MATRIXSIM_H

#include "matrixabstract.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QWheelEvent>

class GraphicsView : public QGraphicsView
{
  Q_OBJECT

public:
  GraphicsView(Settings &settings);
  double getCurrentScale();
  
protected:
  void wheelEvent(QWheelEvent *event);

private:
  int scaleIdx = 2;
  QList<double> scaleList = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0};
  Settings &settings;
};

class MatrixSim : public MatrixAbstract
{
  Q_OBJECT

public:
  MatrixSim(Settings &settings);
  ~MatrixSim();
  bool init() override;

public slots:
  void update(QImage image) override;

private:
  GraphicsView *view = nullptr;
  QGraphicsScene *scene = nullptr;
  QGraphicsPixmapItem *pixmap = nullptr;
};

#endif // _MATRIXSIM_H
