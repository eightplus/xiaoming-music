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

#include "global.h"
#include "preferences.h"
#include "mainwindow.h"

#include <QDebug>

Global *Global::m_instance = nullptr;

Global::Global(QObject *parent) :
    QObject(parent)
{
    qDebug() << "Global.....";
    m_preferences = new Preferences;
}

Global::~Global()
{
    qDebug() << "~Global.....";
    delete m_preferences;
    m_preferences = 0;
}

Global *Global::instance()
{
    return m_instance;
}

Preferences *Global::preferences() const
{
    return m_preferences;
}

void Global::initial(QObject *parent)
{
    if (!m_instance) {
        m_instance = new Global(parent);
    }
}

QWidget *Global::mainWindow() const
{
    return static_cast<QWidget *>(m_mainWindow);
}

void Global::setMainWindow(MainWindow *mainWindow)
{
    m_mainWindow = mainWindow;
}
