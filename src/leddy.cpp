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
#include "gameoflife.h"
#include "rssscroll.h"
#include "gallery.h"
#include "snowfall.h"
#include "runcommand.h"

#ifdef WITHSIM
#include "matrixsim.h"
#endif
#ifdef WITHUNI
#include "matrixuni.h"
#endif
#ifdef WITHADA
#include "matrixada.h"
#endif

#include <QFile>
#include <QImage>
#include <QSettings>
#include <QTimer>
#include <QDomDocument>
#include <QRandomGenerator>
#include <QDir>

NetComm *netComm = nullptr;

Leddy::Leddy(const QCommandLineParser &parser)
{
  QSettings iniSettings("config.ini", QSettings::IniFormat);

  // Genereal matrix settings
  if(!iniSettings.contains("matrix/rotation")) {
    iniSettings.setValue("matrix/rotation", 180);
  }
  if(parser.isSet("r") && !parser.value("r").isEmpty()) {
    settings.rotation = parser.value("r").toInt();
  } else {
    settings.rotation = iniSettings.value("matrix/rotation").toInt();
  }
  if(settings.rotation < 0) {
    settings.rotation = 0;
  }
  if(settings.rotation > 360) {
    settings.rotation = 360;
  }

  if(!iniSettings.contains("matrix/brightness")) {
    iniSettings.setValue("matrix/brightness", 50);
  }
  if(parser.isSet("b") && !parser.value("b").isEmpty()) {
    settings.brightness = parser.value("b").toInt();
  } else {
    settings.brightness = iniSettings.value("matrix/brightness").toInt();
  }
  if(settings.brightness < 0) {
    settings.brightness = 0;
  }
  if(settings.brightness > 100) {
    settings.brightness = 100;
  }

  if(!iniSettings.contains("matrix/framerate")) {
    iniSettings.setValue("matrix/framerate", 30);
  }
  if(parser.isSet("f") && !parser.value("f").isEmpty()) {
    settings.framerate = parser.value("f").toInt();
  } else {
    settings.framerate = iniSettings.value("matrix/framerate").toInt();
  }
  if(settings.framerate < 1) {
    settings.framerate = 1;
  }
  if(settings.framerate > 60) {
    settings.framerate = 60;
  }

  printf("Matrix configuration:\n");
  printf("  Framerate  : %d\n", settings.framerate);
  printf("  Brightness : %d\n", settings.brightness);
  printf("  Rotation   : %d\n", settings.rotation);

  settings.framerate = 1000 / settings.framerate; // ms per frame

  // Unicorn Hat HD & Ubercorn specific settings
  if(!iniSettings.contains("matrixuni/device")) {
    iniSettings.setValue("matrixuni/device", "/dev/spidev0.0");
  }
  settings.uniSpiDevice = iniSettings.value("matrixuni/device").toByteArray();

  if(!iniSettings.contains("matrixuni/speed")) {
    iniSettings.setValue("matrixuni/speed", 9000);
  }
  settings.uniSpiSpeed = iniSettings.value("matrixuni/speed").toInt() * 1000;

  if(!iniSettings.contains("matrixuni/bitsPerWord")) {
    iniSettings.setValue("matrixuni/bitsPerWord", 8);
  }
  settings.uniSpiBits = iniSettings.value("matrixuni/bitsPerWord").toInt();

  if(!iniSettings.contains("matrixuni/mode")) {
    iniSettings.setValue("matrixuni/mode", 0);
  }
  settings.uniSpiMode = iniSettings.value("matrixuni/mode").toInt();

  // Adafruit Matrix Bonnet specific settings
  if(!iniSettings.contains("matrixada/chainLength")) {
    iniSettings.setValue("matrixada/chainLength", 1);
  }
  settings.adaChainLength = iniSettings.value("matrixada/chainLength").toInt();

  if(!iniSettings.contains("matrixada/parallel")) {
    iniSettings.setValue("matrixada/parallel", 1);
  }
  settings.adaParallel = iniSettings.value("matrixada/parallel").toInt();

  if(!iniSettings.contains("matrixada/showRefreshRate")) {
    iniSettings.setValue("matrixada/showRefreshRate", true);
  }
  settings.adaShowRefreshRate = iniSettings.value("matrixada/showRefreshRate").toBool();

  if(!iniSettings.contains("matrixada/gpioSlowdown")) {
    iniSettings.setValue("matrixada/gpioSlowdown", 4);
  }
  settings.adaGpioSlowdown = iniSettings.value("matrixada/gpioSlowdown").toInt();
  
  // Theme related
  if(!iniSettings.contains("theme/xml")) {
    iniSettings.setValue("theme/xml", "themes/16x16/theme.xml");
  }
  iniSettings.sync();
  settings.themeXmlFile = iniSettings.value("theme/xml").toString();

  QFile themeFile(settings.themeXmlFile);
  QDomDocument themeXml;
  if(themeFile.open(QIODevice::ReadOnly) &&
     themeXml.setContent(themeFile.readAll())) {
    themeFile.close();
  } else {
    printf("ERROR: Couldn't load theme XML definitions from '%s'\n", qPrintable(settings.themeXmlFile));
    exit(1);
  }

  if(themeXml.documentElement().hasAttribute("themepath")) {
    QDir themeDir(themeXml.documentElement().attribute("themepath"));
    if(themeDir.isRelative()) {
      settings.themePath = QFileInfo(settings.themeXmlFile).absolutePath() + "/" + themeXml.documentElement().attribute("themepath");
    } else {
      settings.themePath = themeXml.documentElement().attribute("themepath");
    }
  } else {
    settings.themePath = QFileInfo(settings.themeXmlFile).absolutePath();
  }

  if(themeXml.documentElement().hasAttribute("width")) {
    settings.width = themeXml.documentElement().attribute("width").toInt();
  }

  if(themeXml.documentElement().hasAttribute("height")) {
    settings.height = themeXml.documentElement().attribute("height").toInt();
  }

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

  // Create shared 'runcommand' command queue
  settings.commandQueue = QSharedPointer<CommandQueue>(new CommandQueue());
  commandHandler = new CommandHandler(settings);
  commandHandler->moveToThread(&workerThread);
  //connect(&workerThread, SIGNAL(finished()), worker, SLOT(deleteLater()));
  //connect(worker, &CommandThread::resultReady, this, &CommandCreator::handleResult);
  workerThread.start();

  loadTheme();
  
  connect(&sceneTimer, &QTimer::timeout, this, &Leddy::sceneChange);
  sceneTimer.setSingleShot(true);

  matrixTimer.start(settings.framerate, Qt::PreciseTimer, this);

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
    int random = QRandomGenerator::global()->bounded(transitions.count());
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
    int random = QRandomGenerator::global()->bounded(animations.count());
    return animations[animations.keys().at(random)];
  }
  return nullptr;
}

void Leddy::run()
{
  checkActions(true);

#ifdef WITHSIM
  matrix = new MatrixSim(settings);
#endif
#ifdef WITHUNI
  matrix = new MatrixUni(settings);
#endif
#ifdef WITHADA
  matrix = new MatrixAda(settings);
#endif

  if(matrix->init()) {
    if(settings.clear) {
      QImage blackBuffer(settings.width, settings.height, QImage::Format_ARGB32);
      blackBuffer.fill(QColor(Qt::black));
      matrix->update(blackBuffer);
      exit(1);
    }
    sceneChange();
  } else {
    exit(1);
  }
}

void Leddy::timerEvent(QTimerEvent *)
{
  checkActions();

  // Only update if buffer has changed since last update
  if(currentScene != nullptr && prevBuffer != currentScene->getBuffer()) {
    matrix->update(currentScene->getBuffer());
    prevBuffer = currentScene->getBuffer();
  }
}

void Leddy::checkActions(const bool &init)
{
  // Run actions if any matches the current time
  QString currentTime = QTime::currentTime().toString("HH:mm");
  for(const auto &action: actions) {
    bool doAction = false;
    if(init) { // Do all actions up to current time on init
      if(QTime::currentTime() >= QTime::fromString(action.time, "HH:mm")) {
        doAction = true;
      }
    } else { // Run action if current time matches any action time
      if(prevTime != currentTime &&
         currentTime == action.time) {
        prevTime = currentTime;
        doAction = true;
      }
    }
    if(doAction) {
      if(action.parameter == "brightness") {
        settings.brightness = action.value;
        printf("Changed brightness to %d\n", settings.brightness);
      }
    }
  }
}

Scene *Leddy::getNextScene()
{
  rotationIdx++;
  if(rotationIdx >= sceneRotation.length()) {
    rotationIdx = 0;
  }
  if(sceneRotation.at(rotationIdx)->type == SCENE::TRANSITION &&
     sceneRotation.at(rotationIdx)->random) {
    sceneRotation.at(rotationIdx)->scene = getTransition("random");
  }
  if(sceneRotation.at(rotationIdx)->type == SCENE::ANIMATION &&
     sceneRotation.at(rotationIdx)->random) {
    sceneRotation.at(rotationIdx)->scene = getAnimation("random");
    sceneRotation.at(rotationIdx)->scene->setDuration(sceneRotation.at(rotationIdx)->duration);
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

void Leddy::loadTheme()
{
  QFile themeFile(settings.themeXmlFile);
  printf("Loading theme from '%s':\n", qPrintable(settings.themeXmlFile));
  QDomDocument themeXml;
  if(themeFile.open(QIODevice::ReadOnly) &&
     themeXml.setContent(themeFile.readAll())) {
    themeFile.close();
  } else {
    printf("ERROR: Couldn't load theme XML definitions from '%s'\n", qPrintable(settings.themeXmlFile));
  }

  QDomNodeList scenes = themeXml.documentElement().elementsByTagName("rotation").at(0).childNodes();

  for(int a = 0; a < scenes.length(); ++a) {
    QDomElement scene = scenes.at(a).toElement();
    if(scene.tagName() == "animation") {
      if(!animations.isEmpty()) {
        sceneRotation.append(new SceneDesc(getAnimation(scene.attribute("name")),
                                           SCENE::ANIMATION,
                                           (scene.attribute("duration").toInt() == 0?DURATION::ONESHOT:scene.attribute("duration").toInt()),
                                           (scene.attribute("name") == "random"?true:false)));
      }
    } else if(scene.tagName() == "transition") {
      if(!transitions.isEmpty()) {
        sceneRotation.append(new SceneDesc(getTransition(scene.attribute("name")),
                                           SCENE::TRANSITION,
                                           DURATION::ONESHOT,
                                           (scene.attribute("name") == "random"?true:false)));
      }
    } else if(scene.tagName() == "rss") {
      sceneRotation.append(new SceneDesc(new RssScroll(settings,
                                                       scene.attribute("background"),
                                                       scene.attribute("bgcolor"),
                                                       scene.attribute("url"),
                                                       scene.attribute("showsource"),
                                                       scene.attribute("font"),
                                                       scene.attribute("fontcolor"),
                                                       scene.attribute("fontshadowcolor"),
                                                       scene.attribute("texty"),
                                                       scene.attribute("waveheight"),
                                                       scene.attribute("wavelength"),
                                                       scene.attribute("fps"))));
    } else if(scene.tagName() == "gallery") {
      sceneRotation.append(new SceneDesc(new Gallery(settings,
                                                     scene.attribute("duration"),
                                                     scene.attribute("input"),
                                                     scene.attribute("order"),
                                                     scene.attribute("hvel"),
                                                     scene.attribute("vvel"),
                                                     scene.attribute("scale"))));
    } else if(scene.tagName() == "runcommand") {
      sceneRotation.append(new SceneDesc(new RunCommand(settings,
                                                        scene.attribute("background"),
                                                        scene.attribute("bgcolor"),
                                                        scene.attribute("font"),
                                                        scene.attribute("fontcolor"),
                                                        scene.attribute("fontshadowcolor"),
                                                        scene.attribute("texty"),
                                                        scene.attribute("waveheight"),
                                                        scene.attribute("wavelength"),
                                                        scene.attribute("command"),
                                                        (scene.attribute("interval").toInt() >= 5?scene.attribute("interval"):"5"),
                                                        scene.attribute("fps"))));
      connect(commandHandler, &CommandHandler::resultReady, sceneRotation.last()->scene, &Scene::checkResult);
    } else if(scene.tagName() == "weather") {
      sceneRotation.append(new SceneDesc(new Weather(settings,
                                                     scene.attribute("duration"),
                                                     scene.attribute("background"),
                                                     scene.attribute("bgcolor"),
                                                     scene.attribute("city"),
                                                     scene.attribute("key"),
                                                     scene.attribute("cityfont"),
                                                     scene.attribute("citycolor"),
                                                     scene.attribute("cityshadowcolor"),
                                                     scene.attribute("cityx"),
                                                     scene.attribute("cityy"),
                                                     scene.attribute("cityspacing"),
                                                     scene.attribute("cityalign"),
                                                     scene.attribute("tempfont"),
                                                     scene.attribute("tempcolor"),
                                                     scene.attribute("tempshadowcolor"),
                                                     scene.attribute("tempx"),
                                                     scene.attribute("tempy"),
                                                     scene.attribute("tempspacing"),
                                                     scene.attribute("tempalign"))));
    } else if(scene.tagName() == "gameoflife") {
      sceneRotation.append(new SceneDesc(new GameOfLife(settings,
                                                        scene.attribute("duration"),
                                                        scene.attribute("background"),
                                                        scene.attribute("bgcolor"),
                                                        scene.attribute("fgcolor"),
                                                        scene.attribute("fps"))));
    } else if(scene.tagName() == "snowfall") {
      sceneRotation.append(new SceneDesc(new Snowfall(settings,
                                                      scene.attribute("duration"),
                                                      scene.attribute("background"),
                                                      scene.attribute("bgcolor"),
                                                      scene.attribute("fps"))));
    } else if(scene.tagName() == "timedate") {
      sceneRotation.append(new SceneDesc(new TimeDate(settings,
                                                      scene.attribute("duration"),
                                                      scene.attribute("background"),
                                                      scene.attribute("bgcolor"),
                                                      scene.attribute("fontcolor"),
                                                      scene.attribute("fontshadowcolor"),
                                                      scene.attribute("timefont"),
                                                      scene.attribute("timeformat"),
                                                      scene.attribute("timex"),
                                                      scene.attribute("timey"),
                                                      scene.attribute("timespacing"),
                                                      scene.attribute("timealign"),
                                                      scene.attribute("datefont"),
                                                      scene.attribute("dateformat"),
                                                      scene.attribute("datex"),
                                                      scene.attribute("datey"),
                                                      scene.attribute("datespacing"),
                                                      scene.attribute("datealign"))));
    }
  }
  QDomNodeList actionNodes =
    themeXml.documentElement().elementsByTagName("actions").at(0).childNodes();

  for(int a = 0; a < actionNodes.length(); ++a) {
    QDomElement actionElem = actionNodes.at(a).toElement();
    Action action;
    action.parameter = actionElem.attribute("parameter");
    action.time = actionElem.attribute("time");
    action.value = actionElem.attribute("value").toInt();
    printf("  Action (time '%s', parameter '%s', value '%d'\n",
           action.time.toStdString().c_str(),
           action.parameter.toStdString().c_str(),
           action.value);
    actions.append(action);
  }
}
