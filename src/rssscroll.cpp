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

#include <cmath>

#include <QPainter>
#include <QDomDocument>
#include <QDomNodeList>
#include <QRegularExpression>
#include <QRandomGenerator>

extern NetComm *netComm;

RssScroll::RssScroll(Settings &settings,
                     const QString &background,
                     const QString &bgColor,
                     const QString &rssUrl,
                     const QString &showSource,
                     const QString &font,
                     const QString &fontColor,
                     const QString &fontShadowColor,
                     const QString &textY,
                     const QString &waveHeight,
                     const QString &waveLength,
                     const QString &fps)
  : Scene(settings, SCENE::RSSSCROLL, QString(), background, bgColor, fontColor),
    rssUrl(rssUrl)
{
  if(!showSource.isNull() && showSource == "true") {
    this->showSource = true;
  }
  if(!font.isNull() && settings.fonts.contains(font)) {
    this->font = font;
  }
  // Font color is handled with fgColor
  if(!fontShadowColor.isNull()) {
    if(QRegularExpression("^#[0-9a-fA-F]{6}$").match(fontShadowColor).hasMatch()) {
      this->fontShadowColor = QColor(fontShadowColor.mid(1, 2).toInt(Q_NULLPTR, 16),
                                     fontShadowColor.mid(3, 2).toInt(Q_NULLPTR, 16),
                                     fontShadowColor.mid(5, 2).toInt(Q_NULLPTR, 16));
    } else if(QRegularExpression("^#[0-9a-fA-F]{8}$").match(fontShadowColor).hasMatch()) {
      this->fontShadowColor = QColor(fontShadowColor.mid(1, 2).toInt(Q_NULLPTR, 16),
                                     fontShadowColor.mid(3, 2).toInt(Q_NULLPTR, 16),
                                     fontShadowColor.mid(5, 2).toInt(Q_NULLPTR, 16),
                                     fontShadowColor.mid(7, 2).toInt(Q_NULLPTR, 16));
    }
  }
  if(!textY.isNull()) {
    this->textY = textY.toInt();
  }
  if(!waveHeight.isNull() &&
     waveHeight.toInt() > 0 &&
     waveHeight.toInt() <= 20) {
    this->waveHeight = waveHeight.toInt();
  }
  if(!waveLength.isNull() &&
     waveLength.toInt() > 0 &&
     waveLength.toInt() <= 200) {
    this->waveLength = 2.0 / waveLength.toDouble();
  }
  frameTimer.setInterval(1000 / (fps.toInt() <= 0 || fps.toInt() > 120?30:fps.toInt()));

  rssTimer.setInterval(60 * 30 * 1000); // Every half hour
  rssTimer.setSingleShot(true);
  connect(&rssTimer, &QTimer::timeout, this, &RssScroll::rssUpdate);
  rssUpdate();
}

void RssScroll::start()
{
  if(bgColorType == COLOR::RANDOM) {
    bgColor = QColor::fromHsl(QRandomGenerator::global()->bounded(360),
                              255,
                              50);
  }
  if(fgColorType == COLOR::RANDOM) {
    fgColor = QColor::fromHsl(QRandomGenerator::global()->bounded(360),
                              255,
                              200);
  } else if(fgColorType == COLOR::COMPLIMENTARY) {
    fgColor = QColor::fromHsl((bgColor.hslHue() + 180 > 359?180 + bgColor.hslHue() - 359:bgColor.hslHue() + 180),
                              bgColor.hslSaturation(),
                              200);
  }
  currentX = 64;

  if(rssLines.isEmpty()) {
    rssLine = "RSS feed didn't return any entries! Please check network and URL.";
  } else {
    int random = QRandomGenerator::global()->bounded(rssLines.length());
    rssLine = rssLines.at(random);
    if(showSource == true) {
      QList<QString> host = QUrl(rssUrl).host().toUpper().split('.');
      int hostIdx = 0;
      if(host.count() > 2) {
        hostIdx = 1;
      }
      rssLine.prepend(host.at(hostIdx) + ": ");
    }
  }
  nextFrame();
}

void RssScroll::nextFrame()
{
  if(!background.isNull()) {
    QPainter painter;
    painter.begin(&buffer);
    painter.drawImage(0, 0, background);
    painter.end();
  } else {
    buffer.fill(bgColor);
  }

  if(waveHeight > 0) {
    wavePhase += waveLength;
    if(wavePhase > 2.0) {
      wavePhase = 0.0;
    }
  }

  QRect textRect = drawText(currentX, textY - (settings.fonts[font].getHeight() / 2) + (sin(wavePhase * 3.14) * waveHeight), font, rssLine, fgColor, fontShadowColor);

  if(currentX < -textRect.width()) {
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
  rssReply->close();
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
  rssReply->deleteLater();
  rssTimer.start();
}
