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

#include "rssscroll.h"
#include "netcomm.h"

#include <QDomDocument>
#include <QDomNodeList>

extern NetComm *netComm;

RssScroll::RssScroll(Settings &settings, const QString &rssUrl)
  : Scene(settings, SCENE::RSSSCROLL, -1), rssUrl(rssUrl)
{
  rssTimer.setInterval(60 * 30 * 1000); // Every half hour
  rssTimer.setSingleShot(true);
  connect(&rssTimer, &QTimer::timeout, this, &RssScroll::rssUpdate);
  rssUpdate();
}

void RssScroll::start()
{
  currentX = 17;

  if(rssLines.isEmpty()) {
    rssLine = "RSS feed URL from 'config.ini' didn't return any entries. Please check it and / or verify your network connection.";
  } else {
    int random = qrand() % rssLines.length();
    rssLine = rssLines.at(random);
  }
  frameTimer.setInterval(40);
  nextFrame();
}

void RssScroll::nextFrame()
{
  buffer.fill(bgColor);
  int textWidth = drawText(currentX, 4, "medium", rssLine, QColor(Qt::white), 1);

  if(currentX < - textWidth) {
    running = false;
    emit sceneEnded();
    return;
  }
  currentX--;
  frameTimer.start();
}

void RssScroll::rssUpdate()
{
  rssReply = netComm->get(QNetworkRequest(QUrl(rssUrl)));
  connect(rssReply, &QNetworkReply::finished, this, &RssScroll::rssReady);
}

void RssScroll::rssReady()
{
  printf("RSS feed titles updated from '%s':\n", rssUrl.toStdString().c_str());
  QDomDocument doc;
  doc.setContent(rssReply->readAll());
  QDomNodeList titles = doc.elementsByTagName("item");
  rssLines.clear();
  for(int a = 0; a < titles.length(); ++a) {
    rssLines.append(titles.at(a).firstChildElement("title").text().trimmed().
                    replace("&quot;", "\"").
                    replace("&amp;", "&").
                    replace("&apos;", "'").
                    replace("&lt;", "<").
                    replace("&gt;", ">"));
    printf("  Title: %s\n", rssLines.last().toStdString().c_str());
  }
  rssReply->close();
  rssReply->deleteLater();
  rssTimer.start();
}
