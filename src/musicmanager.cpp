/*
 * Copyright (C) 2018 ~ 2019 kobe24_lixiang@126.com
 *
 * Authors:
 *  lixiang    kobe24_lixiang@126.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "musicmanager.h"
#include "global.h"
#include "mainwindow.h"

#include <QApplication>

MusicManager::MusicManager(QObject *parent) :
    QObject(parent)
{
    Global::initial(this);
}

MusicManager::~MusicManager()
{

}

MainWindow *MusicManager::mainWindow() const
{
    return m_mainWindow;
}

void MusicManager::setMainWindow(MainWindow *mainWindow)
{
    if(m_mainWindow == nullptr) {
        m_mainWindow = mainWindow;
        globalInstance->setMainWindow(m_mainWindow);
    }
}

void MusicManager::run()
{
    m_mainWindow->initPlayer();
    m_mainWindow->show();
}
