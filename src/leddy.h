/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            leddy.h
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

#ifndef LEDDY_H
#define LEDDY_H

#include "spiconn.h"
#include "settings.h"
#include "netcomm.h"

#include <QObject>
#include <QCommandLineParser>
#include <QTimer>

class Leddy : public QObject
{
  Q_OBJECT

public:
  Leddy(const QCommandLineParser &parser);
  ~Leddy();

public slots:
  void run();

signals:
  void finished();

private:
  NetComm *netComm;
  Settings settings;
  SPIConn *spiDev;
  int eventIdx = 0;
  QTimer eventTimer;

private slots:
  void nextEvent();
};

#endif // LEDDY_H
