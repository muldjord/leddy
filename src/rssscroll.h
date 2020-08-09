/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            rssscroll.h
 *
 *  Sun Aug 2 12:00:00 CEST 2020
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

#ifndef _RSSSCROLL_H
#define _RSSSCROLL_H

#include "scene.h"
#include "settings.h"

#include <QNetworkRequest>
#include <QNetworkReply>

class RssScroll : public Scene
{
  Q_OBJECT

public:
  RssScroll(Settings &settings,
            const QString &background = QString(),
            const QString &rssUrl = QString(),
            const QString &showSource = QString(),
            const QString &font = QString(),
            const QString &fontColor = QString(),
            const QString &waveHeight = QString(),
            const QString &waveLength = QString());
  void start() override;
                             
public slots:
  void nextFrame() override;

private slots:
  void rssUpdate();
  void rssReady();

private:
  QNetworkReply *rssReply = nullptr;
  QTimer rssTimer;
  QString rssUrl = "";
  QString font = "medium";
  QColor fontColor = QColor(Qt::white);
  bool randFontColor = false;
  bool showSource = false;
  double wavePhase = 0.0;
  double waveHeight = -1;
  double waveLength = 0;
  int currentX = 17;
  QList<QString> rssLines;
  QString rssLine = "";
  
};

#endif // _RSSSCROLL_H
