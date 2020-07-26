/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            netcomm.h
 *
 *  Tue Nov 26 16:56:00 CEST 2013
 *  Copyright 2013 Lars Muldjord
 *  muldjordlars@gmail.com
 ****************************************************************************/

/*
 *  This file is part of Boris.
 *
 *  Boris is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  Boris is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Boris; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA.
 */
#ifndef _NETCOMM_H
#define _NETCOMM_H

#include "settings.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QTimer>

class NetComm : public QNetworkAccessManager
{
  Q_OBJECT;
public:
  NetComm(Settings *settings);
  ~NetComm();

public slots:
  void updateAll();
  
signals:
  void weatherUpdated();
  void rssUpdated();

private slots:
  void netReply(QNetworkReply *r);
  
private:
  Settings *settings;
  
  QTimer netTimer;
  QNetworkRequest weatherRequest;
  QNetworkRequest rssRequest;
};

#endif // _NETCOMM_H
