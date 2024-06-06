/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            matrixsim.cpp
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

#include "matrixsim.h"
#include "globaldefs.h"

GraphicsView::GraphicsView(Settings &settings)
  : settings(settings)
{
}

void GraphicsView::wheelEvent(QWheelEvent * event)
{
  if(event->angleDelta().y() > 0) {
    if(scaleIdx + 1 < scaleList.length()) {
      scaleIdx++;
    }
    resetTransform();
    scale(scaleList[scaleIdx], scaleList[scaleIdx]);
    setFixedSize((transform().m11() * settings.width) + 5, (transform().m22() * settings.height) + 5);
    event->accept();
    return;
  }

  if(event->angleDelta().y() < 0) {
    if(scaleIdx - 1 >= 0) {
      scaleIdx--;
    }
    resetTransform();
    scale(scaleList[scaleIdx], scaleList[scaleIdx]);
    setFixedSize((transform().m11() * settings.width) + 5, (transform().m22() * settings.height) + 5);
    event->accept();
    return;
  }

  // Always ignore event if not used, to allow propagation to parent
  event->ignore();
}

double GraphicsView::getCurrentScale()
{
  return scaleList[scaleIdx];
}

MatrixSim::MatrixSim(Settings &settings)
  : MatrixAbstract(settings)
{
}

MatrixSim::~MatrixSim()
{
  delete scene;
  delete view;
}

bool MatrixSim::init()
{
  view = new GraphicsView(settings);
  scene = new QGraphicsScene;
  scene->setSceneRect(0, 0, settings.width, settings.height);
  QPixmap blank(settings.width, settings.height);
  blank.fill(QColor(Qt::black));
  pixmap = scene->addPixmap(blank);
  view->setScene(scene);
  view->scale(view->getCurrentScale(), view->getCurrentScale());
  view->setFixedSize((view->transform().m11() * settings.width) + 5, (view->transform().m22() * settings.height) + 5);
  view->show();

  return true;
}

void MatrixSim::update(QImage buffer)
{
  pixmap->setPixmap(QPixmap::fromImage(buffer));
}
