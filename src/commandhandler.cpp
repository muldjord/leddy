/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/***************************************************************************
 *            commandhandler.cpp
 *
 *  Mon Jan 3 09:41:00 CEST 2022
 *  Copyright 2022 Lars Muldjord
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

#include "commandhandler.h"

#include <QProcess>

CommandHandler::CommandHandler(Settings &settings) : settings(settings)
{
  pollTimer = new QTimer(this);
  pollTimer->setInterval(500);
  pollTimer->setSingleShot(true);
  connect(pollTimer, &QTimer::timeout, this, &CommandHandler::checkQueue);
  pollTimer->start();
}

CommandHandler::~CommandHandler()
{
}

void CommandHandler::checkQueue()
{
  while(settings.commandQueue->hasEntry()) {
    QString command = settings.commandQueue->takeEntry();
    QString commandResult = "Command aborted: Took too long or couldn't execute.";
    QProcess process;
    process.start(command);
    if(process.waitForFinished(60000)) { // Max 1 minute
      commandResult = QString::fromUtf8(process.readAllStandardOutput());
      commandResult.append(QString::fromUtf8(process.readAllStandardError()));
    }
    commandResult = commandResult.trimmed();
    emit resultReady(command, commandResult);
  }
  pollTimer->start();
}
