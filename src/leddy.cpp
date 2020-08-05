/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            leddy.cpp
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

#include "leddy.h"
#include "netcomm.h"
#include "loader.h"
#include "timetemp.h"
#include "weather.h"

#include <QImage>
#include <QSettings>
#include <QTimer>

QMap<QString, PixelFont> fonts;

Leddy::Leddy(const QCommandLineParser &parser)
{
  qsrand((uint)QTime::currentTime().msec());

  QSettings iniSettings("config.ini", QSettings::IniFormat);

  if(!iniSettings.contains("rss/url")) {
    iniSettings.setValue("rss/url", "http://rss.slashdot.org/Slashdot/slashdotMain");
  }
  settings.rssUrl = iniSettings.value("rss/url").toString();

  if(!iniSettings.contains("weather/city")) {
    iniSettings.setValue("weather/city", "Copenhagen");
  }
  settings.city = iniSettings.value("weather/city").toString();

  if(!iniSettings.contains("weather/key")) {
    iniSettings.setValue("weather/key", "fe9fe6cf47c03d2640d5063fbfa053a2");
  }
  settings.key = iniSettings.value("weather/key").toString();

  if(!iniSettings.contains("unicorn_hd/rotation")) {
    iniSettings.setValue("unicorn_hd/rotation", 180);
  }
  if(parser.isSet("r") && !parser.value("r").isEmpty()) {
    settings.rotation = parser.value("r").toInt();
  } else {
    settings.rotation = iniSettings.value("unicorn_hd/rotation").toInt();
  }

  if(!iniSettings.contains("unicorn_hd/brightness")) {
    iniSettings.setValue("unicorn_hd/brightness", 50);
  }
  if(parser.isSet("B") && !parser.value("B").isEmpty()) {
    settings.brightness = parser.value("B").toInt();
  } else {
    settings.brightness = iniSettings.value("unicorn_hd/brightness").toInt();
  }

  if(!iniSettings.contains("spi/device")) {
    iniSettings.setValue("spi/device", "/dev/spidev0.0");
  }
  if(parser.isSet("d") && !parser.value("d").isEmpty()) {
    settings.device = parser.value("d").toUtf8();
  } else {
    settings.device = iniSettings.value("spi/device").toByteArray();
  }

  if(!iniSettings.contains("spi/speed")) {
    iniSettings.setValue("spi/speed", 9000);
  }
  if(parser.isSet("s") && !parser.value("s").isEmpty()) {
    settings.speed = parser.value("s").toInt() * 1000;
  } else {
    settings.speed = iniSettings.value("spi/speed").toInt() * 1000;
  }

  if(!iniSettings.contains("spi/bits_per_word")) {
    iniSettings.setValue("spi/bits_per_word", 8);
  }
  if(parser.isSet("b") && !parser.value("b").isEmpty()) {
    settings.bits = parser.value("b").toInt();
  } else {
    settings.bits = iniSettings.value("spi/bits_per_word").toInt();
  }

  if(!iniSettings.contains("spi/mode")) {
    iniSettings.setValue("spi/mode", 0);
  }
  if(parser.isSet("m") && !parser.value("m").isEmpty()) {
    settings.mode = parser.value("m").toInt();
  } else {
    settings.mode = iniSettings.value("spi/mode").toInt();
  }

  if(!iniSettings.contains("data/font_path")) {
    iniSettings.setValue("data/font_path", "data/fonts");
  }
  settings.fontPath = iniSettings.value("data/font_path").toString();

  if(!iniSettings.contains("data/transition_path")) {
    iniSettings.setValue("data/transition_path", "data/transitions");
  }
  settings.transitionPath = iniSettings.value("data/transition_path").toString();

  if(parser.isSet("clear")) {
    settings.clear = true;
  }

  if(!Loader::loadFonts(settings, fonts)) {
    printf("ERROR: Error when loading some fonts!\n");
  }

  if(!Loader::loadTransitions(settings, transitions)) {
    printf("ERROR: Error when loading some transitions!\n");
  }

  sceneRotation.append(new TimeTemp(settings, 10000));
  sceneRotation.append(transitions["pacman"]);
  sceneRotation.append(new Weather(settings, 10000));
  sceneRotation.append(transitions["lemmings"]);
  sceneRotation.append(new TimeTemp(settings, 10000));
  sceneRotation.append(transitions["invaders"]);
  sceneRotation.append(new Weather(settings, 10000));
  sceneRotation.append(transitions["circular1"]);

  connect(&sceneTimer, &QTimer::timeout, this, &Leddy::sceneChange);
  sceneTimer.setSingleShot(true);

  netComm = new NetComm(settings);

  QTimer::singleShot(1000, this, &Leddy::run);
}

Leddy::~Leddy()
{
}

void Leddy::run()
{
  uniConn = new UniConn(settings);
  if(uniConn->init()) {
    if(settings.clear) {
      QImage blackBuffer(16, 16, QImage::Format_ARGB32);
      blackBuffer.fill(QColor(Qt::black));
      uniConn->update(blackBuffer);
      exit(1);
    }
    sceneChange();
  } else {
#ifndef WITHSIM
    exit(1);
#else
    sceneChange();
#endif
  }
}

Scene *Leddy::getNextScene()
{
  rotationIdx++;
  if(rotationIdx >= sceneRotation.length()) {
    rotationIdx = 0;
  }
  return sceneRotation.at(rotationIdx);
}

void Leddy::sceneChange()
{
  previousScene = currentScene;
  currentScene = nextScene;
  nextScene = getNextScene();
  nextScene->init();

  if(previousScene == nullptr) {
    sceneChange();
    return;
  }
  
  // Disconnect everything, it should be a clean slate at this point
  if(previousScene != nullptr) {
    disconnect(previousScene, &Scene::sceneEnded, this, &Leddy::sceneChange);
    disconnect(previousScene, &Scene::frameReady, uniConn, &UniConn::update);
  }

  if(currentScene == nextScene) {
    printf("WARNING: You seem to have the same scene added to the rotation twice in a row. This can cause undefined behaviour and might cause Leddy to crash.\n");
  }

  connect(currentScene, &Scene::sceneEnded, this, &Leddy::sceneChange);
  connect(currentScene, &Scene::frameReady, uniConn, &UniConn::update);
  currentScene->init(previousScene, nextScene);
  nextScene->init();
  if(currentScene->getSceneTime() != -1) {
    sceneTimer.setInterval(currentScene->getSceneTime());
    sceneTimer.start();
  }
}
