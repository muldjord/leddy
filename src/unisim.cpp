/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            unisim.cpp
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

#include "unisim.h"

UniSim::UniSim()
{
  scene = new QGraphicsScene;
  scene->setSceneRect(0, 0, 16, 16);
  pixmap = scene->addPixmap(QPixmap(16, 16));
  setScene(scene);
  scale(4.0, 4.0);
}

UniSim::~UniSim(){
}

void UniSim::setImage(const QImage &scene)
{
  pixmap->setPixmap(QPixmap::fromImage(scene));
}

void UniSim::wheelEvent(QWheelEvent * event)
{
  if(event->orientation() == Qt::Vertical && event->delta() > 0) {
    scale(1.5, 1.5);
    event->accept();
    return;
  }

  if(event->orientation() == Qt::Vertical && event->delta() < 0) {
    scale(1 / 1.5, 1 / 1.5);
    event->accept();
    return;
  }

  // Always ignore event if not used, to allow propagation to parent
  event->ignore();
}
