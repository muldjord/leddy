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

#include <QImage>
#include <QSettings>

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

  if(!Loader::loadFonts(settings.fontPath, fonts)) {
    printf("ERROR: Error when loading some fonts!\n");
  }

  if(!Loader::loadTransitions(settings.transitionPath, transitions)) {
    printf("ERROR: Error when loading some transitions!\n");
  }

  QTimer::singleshot(2000, &Leddy::run);

  netComm = new NetComm(settings);
}

Leddy::~Leddy()
{
}

void Leddy::run()
{
  uniConn = new UniConn(settings);
  connect(uniConn, &UniConn::sceneReady, &sceneTimer, qOverload<>(&QTimer::start));
  if(uniConn->init()) {
    if(settings.clear) {
      QImage blackBuffer(16, 16, QImage::Format_ARGB32);
      blackBuffer.fill(QColor(Qt::black));
      uniConn->update(blackBuffer);
      exit(1);
    }
    nextScene();
  } else {
#ifndef WITHSIM
    exit(1);
#else
    nextScene();
#endif
  }
}

void Leddy::nextScene()
{
  // Disconnect everything, it should be a clean slate at this point
  if(transition != nullptr) {
    disconnect(transition, nullptr, nullptr, nullptr);
  }
  if(scene != nullptr) {
    disconnect(scene, nullptr, nullptr, nullptr);
  }
  
  scene = &(Scene&)scenes[nextScene];
  
  if(scene->type() == "transition") {

    transition = scene;
    if(transition == scene) {
      printf("WARNING: You seem to have the same scene or transition added to the rotation twice in a row. This can cause undefined behaviour.\n");
    }
    connect(transition, &Scene::frameReady, this, &UniConn::update);
    
    scene = &(Scene&)scenes[nextScene];
    connect(scene, &Scene::sceneEnded, this, &Leddy::nextScene);
    scene->init();

    transition->init(UniConn->latestBuffer, nextScene, uniConn);
    
  } else if(scene->type() == "scene") {

    connect(scene, &Scene::sceneEnded, this, &Leddy::nextScene);
    scene->init();

  }
  /* THOUGHTS:
     If everything is a Scene and every Scene has a frameReady, then I could potentially have an 'update(QImage buffer) inside Scene as well as in UniConn. If a transition is going on, it would then be connected to the Scene::update (which would update the internal toBuffer used for the transition), allowing both the transition and the scene to play at the same time...? And when the transition is then done, it will then re-connect the frameReady from the Scene to UniConn::update where the remainder of the scene would play out as usual.

     The LAST Scene's sceneEnded signal should always be the only one connected. We don't care if an earlier sceneEnded signal is emitted when a previous scene was longer than the current scene. In that case it will just run along in the background without rendering to anywhere. And if it's restarted in the meantime init() will be called on it, and it's restarted anyway so it should be fine.

     if(currentScene->type() == T_TRANSITION) {
       nextScene->init();
       connect(nextScene, &Scene::frameReady, currentScene, &Scene::update); // Will then render to transition buffer instead of UniConn buffer
       At one point the transition ends, and we need to connect the 'nextScene frameReady' to UniConn::update instead... how? It should happen when the transision emits sceneEnded()
     }

 So if we encountered a transition, we know we should just start the next Scene right away. The transition will start running, but the sceneEnded inside of it is not connected, so it will just stop when it's done.

     

  */

  /* QUESTIONS:
     What happens if a Scene ends before the transition is done?
     Should I have a 'Pause scene until transition has ended' setting for each Scene?
   */
  
  if(scene->type() == "transition") {
    transition = &(Transition&)transitions[scene->transitionName];
    connect(frameTimer, &QTimer::timeout, transition, &Transition::nextFrame);
    
    transition->init(uniConn->latestBuffer, scene->init());
  } else {
  }

  if(!scene->transitionName.isEmpty()) {
  } else {
    transition = nullptr;
    nextScene();
  }
}

void Leddy::transitionEnded
  disconnect(frameTimer, nullptr, nullptr, nullptr);
  connect(frameTimer, &QTimer::timeout, scene, &Scene::nextFrame);
  connect(scene, &Scene::frameReady, uniConn, &UniConn::update);
  connect(scene, &Scene::sceneDone, this, &Leddy::nextScene);

  

  printf("Switching to next scene!\n");
  if(eventIdx == 0) {
    uniConn->beginScene();
    QString timeStr = QTime::currentTime().toString("HH:mm");
    uniConn->drawText(0, 2, "small", timeStr.left(2), QColor(Qt::white), 0);
    uniConn->drawText(7, 2, "small", timeStr.mid(2, 1), QColor(Qt::white), 0);
    uniConn->drawText(9, 2, "small", timeStr.mid(3, 2), QColor(Qt::white), 0);
    QColor tempColor(Qt::white);
    if(settings.temperature < 0) {
      tempColor = QColor(0, 0, 255);
    } else if(settings.temperature < 5) {
      tempColor = QColor(0, 210, 255);
    } else if(settings.temperature < 10) {
      tempColor = QColor(0, 255, 204);
    } else if(settings.temperature < 15) {
      tempColor = QColor(0, 255, 145);
    } else if(settings.temperature < 20) {
      tempColor = QColor(0, 255, 69);
    } else if(settings.temperature < 25) {
      tempColor = QColor(143, 255, 0);
    } else if(settings.temperature < 30) {
      tempColor = QColor(255, 248, 0);
    } else if(settings.temperature < 35) {
      tempColor = QColor(255, 159, 0);
    } else if(settings.temperature < 40) {
      tempColor = QColor(255, 65, 0);
    }
    uniConn->drawText(0, 8, "medium",
                      QString::number((int)settings.temperature) + "C", tempColor, 1);
    sceneTimer.setInterval(10000);
    uniConn->showScene("random");
  } else if(eventIdx == 1) {
    uniConn->beginScene();
    uniConn->drawImage(0, 0, QImage(":" + settings.weatherType + ".png"));
    sceneTimer.setInterval(10000);
    uniConn->showScene("random");
  }
  eventIdx++;
  if(eventIdx > 1) {
    eventIdx = 0;
  }
}
