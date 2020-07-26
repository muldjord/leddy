/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            main.cpp
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

#include <QtGlobal>

#include <QCoreApplication>
#include <QtDebug>
#include <QDir>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QTimer>

#include "leddy.h"

void customMessageHandler(QtMsgType type, const QMessageLogContext&, const QString &msg)
{
  // Add timestamp to debug message
  QString txt = QDateTime::currentDateTime().toString("dd MMM, hh:mm:ss");
  // Decide which type of debug message it is, and add string to signify it
  // Then append the debug message itself to the same string.
  switch (type) {
  case QtInfoMsg:
    txt += QString(": Info: %1").arg(msg);
    break;
  case QtDebugMsg:
    txt += QString(": Debug: %1").arg(msg);
    break;
  case QtWarningMsg:
    txt += QString(": Warning: %1").arg(msg);
    break;
  case QtCriticalMsg:
    txt += QString(": Critical: %1").arg(msg);
    break;
  case QtFatalMsg:
    txt += QString(": Fatal: %1").arg(msg);
    abort();
  }
  printf("%s", txt.toStdString().c_str());
}

int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);
  app.setApplicationVersion(VERSION);
  
  // Get current dir. If user has specified file(s) on command line we need this.
  QDir::setCurrent(QDir::currentPath());

  // Install the custom debug message handler used by qDebug()
  qInstallMessageHandler(customMessageHandler);

  QCommandLineParser parser;

  parser.setApplicationDescription("Leddy!");
  parser.addHelpOption();
  parser.addVersionOption();
  QCommandLineOption pOption("p", "The platform you wish to scrape.\n(Currently supports)", "PLATFORM", "");

  parser.addOption(pOption);

  parser.process(app);

  if(parser.isSet("help") || parser.isSet("h")) {
    parser.showHelp();
  } else {
    Leddy *x = new Leddy(parser);
    QObject::connect(x, &Leddy::finished, &app, &QCoreApplication::quit);
    QTimer::singleShot(0, x, SLOT(run()));
  }
  return app.exec();
}
