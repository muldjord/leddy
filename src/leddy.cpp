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
#include "animation.h"
#include "timedate.h"
#include "weather.h"
#include "rssscroll.h"
#include "globaldefs.h"

#include <QImage>
#include <QSettings>
#include <QTimer>
#include <QDomDocument>

NetComm *netComm = nullptr;

Leddy::Leddy(const QCommandLineParser &parser)
{
  qsrand((uint)QTime::currentTime().msec());

  QSettings iniSettings("config.ini", QSettings::IniFormat);

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
  if(settings.rotation < 0) {
    settings.rotation = 0;
  }
  if(settings.rotation > 360) {
    settings.rotation = 360;
  }

  if(!iniSettings.contains("unicorn_hd/brightness")) {
    iniSettings.setValue("unicorn_hd/brightness", 50);
  }
  if(parser.isSet("B") && !parser.value("B").isEmpty()) {
    settings.brightness = parser.value("B").toInt();
  } else {
    settings.brightness = iniSettings.value("unicorn_hd/brightness").toInt();
  }
  if(settings.brightness < 0) {
    settings.brightness = 0;
  }
  if(settings.brightness > 100) {
    settings.brightness = 100;
  }

  if(!iniSettings.contains("unicorn_hd/framerate")) {
    iniSettings.setValue("unicorn_hd/framerate", 50);
  }
  if(parser.isSet("f") && !parser.value("f").isEmpty()) {
    settings.framerate = parser.value("f").toInt();
  } else {
    settings.framerate = iniSettings.value("unicorn_hd/framerate").toInt();
  }
  if(settings.framerate < 1) {
    settings.framerate = 1;
  }
  if(settings.framerate > 60) {
    settings.framerate = 60;
  }

  printf("Unicorn Hat HD configuration:\n");
  printf("  Framerate  : %d\n", settings.framerate);
  printf("  Brightness : %d\n", settings.brightness);
  printf("  Rotation   : %d\n", settings.rotation);

  settings.framerate = 1000 / settings.framerate; // ms per frame

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

  if(!iniSettings.contains("theme/path")) {
    iniSettings.setValue("theme/path", "themes/default");
  }
  settings.themePath = iniSettings.value("theme/path").toString();

  settings.fontPath = settings.themePath +
    (settings.themePath.right(1) == "/"?"":"/") + "fonts";
  settings.animationPath = settings.themePath +
    (settings.themePath.right(1) == "/"?"":"/") + "animations";
  settings.transitionPath = settings.themePath +
    (settings.themePath.right(1) == "/"?"":"/") + "transitions";
  settings.backgroundPath = settings.themePath +
    (settings.themePath.right(1) == "/"?"":"/") + "backgrounds";
  settings.weatherIconPath = settings.themePath +
    (settings.themePath.right(1) == "/"?"":"/") + "weather";

  if(parser.isSet("clear")) {
    settings.clear = true;
  }

  if(!Loader::loadFonts(settings)) {
    printf("ERROR: Error when loading some fonts!\n");
  }

  if(!Loader::loadAnimations(settings, animations)) {
    printf("ERROR: Error when loading some animations!\n");
  }

  if(!Loader::loadTransitions(settings, transitions)) {
    printf("ERROR: Error when loading some transitions!\n");
  }

  if(!Loader::loadBackgrounds(settings)) {
    printf("ERROR: Error when loading some backgrounds!\n");
  }

  if(!Loader::loadWeatherIcons(settings)) {
    printf("ERROR: Error when loading some weather icons!\n");
  }

  netComm = new NetComm(settings);

  loadRotation(); // Load all scenes into the rotation from XML
  
  connect(&sceneTimer, &QTimer::timeout, this, &Leddy::sceneChange);
  sceneTimer.setSingleShot(true);

  connect(&uniTimer, &QTimer::timeout, this, &Leddy::pushBuffer);
  uniTimer.setInterval(settings.framerate);
  uniTimer.setSingleShot(true);

  QTimer::singleShot(1000, this, &Leddy::run);
}

Leddy::~Leddy()
{
}

Scene *Leddy::getTransition(const QString &name)
{
  if(transitions.contains(name)) {
    return transitions[name];
  }
  if(name == "random") {
    int random = qrand() % transitions.count();
    return transitions[transitions.keys().at(random)];
  }
  return nullptr;
}

Scene *Leddy::getAnimation(const QString &name)
{
  if(animations.contains(name)) {
    return animations[name];
  }
  if(name == "random") {
    int random = qrand() % animations.count();
    return animations[animations.keys().at(random)];
  }
  return nullptr;
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
    uniTimer.start();
    sceneChange();
  } else {
#ifndef WITHSIM
    exit(1);
#else
    uniTimer.start();
    sceneChange();
#endif
  }
}

void Leddy::pushBuffer()
{
  // Only update if buffer has changed since last update
  if(currentScene != nullptr && prevBuffer != currentScene->getBuffer()) {
    uniConn->update(currentScene->getBuffer());
    prevBuffer = currentScene->getBuffer();
  }
  uniTimer.start();
}

Scene *Leddy::getNextScene()
{
  rotationIdx++;
  if(rotationIdx >= sceneRotation.length()) {
    rotationIdx = 0;
  }
  if(sceneRotation.at(rotationIdx)->scene->getType() == SCENE::TRANSITION &&
     sceneRotation.at(rotationIdx)->random) {
    sceneRotation.at(rotationIdx)->scene = getTransition("random");
  }
  if(sceneRotation.at(rotationIdx)->scene->getType() == SCENE::ANIMATION &&
     sceneRotation.at(rotationIdx)->random) {
    sceneRotation.at(rotationIdx)->scene = getAnimation("random");
  }
  return sceneRotation.at(rotationIdx)->scene;
}

void Leddy::sceneChange()
{
  previousScene = currentScene;
  currentScene = nextScene;
  nextScene = getNextScene();
  if(nextScene->getDuration() != DURATION::ONESHOT) {
    nextScene->init();
  }
  
  // Fill all initial scene pointers before moving on
  if(currentScene == nullptr) {
    sceneChange();
    return;
  }

  if(currentScene == nextScene) {
    printf("WARNING: You seem to have the same scene added to the rotation twice in a row. This can cause undefined behaviour.\n");
  }

  // Disconnect everything, it should be a clean slate at this point
  if(previousScene != nullptr) {
    disconnect(previousScene, &Scene::sceneEnded, this, &Leddy::sceneChange);
  }

  connect(currentScene, &Scene::sceneEnded, this, &Leddy::sceneChange);
  currentScene->init(previousScene, nextScene);
  if(currentScene->getType() == SCENE::TRANSITION) {
    nextScene->init();
  }
  if(currentScene->getDuration() != DURATION::ONESHOT) {
    sceneTimer.setInterval(currentScene->getDuration());
    sceneTimer.start();
  } else {
    sceneTimer.stop();
  }
}

void Leddy::loadRotation()
{
  QString themeFileStr = settings.themePath +
    (settings.themePath.right(1) == "/"?"":"/") + "theme.xml";

  printf("Loading theme from '%s':\n", themeFileStr.toStdString().c_str());

  QFile themeFile(themeFileStr);
  QDomDocument themeXml;
  if(themeFile.open(QIODevice::ReadOnly) &&
     themeXml.setContent(themeFile.readAll())) {
    themeFile.close();
  } else {
    printf("ERROR: Couldn't load theme XML definitions from '%s'\n",
           themeFileStr.toStdString().c_str());
  }
  
  QDomNodeList scenes =
    themeXml.documentElement().elementsByTagName("rotation").at(0).childNodes();

  for(int a = 0; a < scenes.length(); ++a) {
    QDomElement scene = scenes.at(a).toElement();
    if(scene.tagName() == "animation") {
      sceneRotation.append(new SceneDesc(getAnimation(scene.attribute("name")), SCENE::ANIMATION, (scene.attribute("name") == "random"?true:false)));
    } else if(scene.tagName() == "transition") {
      sceneRotation.append(new SceneDesc(getTransition(scene.attribute("name")), SCENE::TRANSITION, (scene.attribute("name") == "random"?true:false)));
    } else if(scene.tagName() == "rss") {
      sceneRotation.append(new SceneDesc(new RssScroll(settings,
                                                       scene.attribute("background"),
                                                       scene.attribute("url"),
                                                       scene.attribute("showsource"),
                                                       scene.attribute("font"),
                                                       scene.attribute("waveheight"),
                                                       scene.attribute("wavelength"))));
    } else if(scene.tagName() == "weather") {
      sceneRotation.append(new SceneDesc(new Weather(settings,
                                                     scene.attribute("duration"))));
    } else if(scene.tagName() == "timedate") {
      sceneRotation.append(new SceneDesc(new TimeDate(settings,
                                                      scene.attribute("duration"),
                                                      scene.attribute("background"),
                                                      scene.attribute("timefont"),
                                                      scene.attribute("timecolor"),
                                                      scene.attribute("timeformat"),
                                                      scene.attribute("timex"),
                                                      scene.attribute("timey"),
                                                      scene.attribute("timespacing"),
                                                      scene.attribute("datefont"),
                                                      scene.attribute("datecolor"),
                                                      scene.attribute("dateformat"),
                                                      scene.attribute("datex"),
                                                      scene.attribute("datey"),
                                                      scene.attribute("datespacing"))));
    }
  }
}
