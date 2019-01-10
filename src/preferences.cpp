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

#include "preferences.h"

#include <QSettings>
#include <QDir>
#include <QDebug>

Preferences::Preferences()
{
    QString filename = QDir::homePath() + "/.config/xiaoming/xiaoming-music.ini";
    m_settings = new QSettings(filename, QSettings::IniFormat);
    m_settings->setIniCodec("UTF-8");

    m_version = 8;
    m_volume = 0;

    resetSettings();
    loadSettings();
}

Preferences::~Preferences()
{
    saveSettings();

    qDebug() << "m_volume=" << m_volume;

    delete m_settings;
}

void Preferences::resetSettings()
{

}

void Preferences::saveSettings()
{
    /*QSettings *set = m_settings;
    set->beginGroup("General");
    set->setValue("config_version", m_version);
    set->endGroup();
    set->sync();*/
}

void Preferences::loadSettings()
{
    /*QSettings *set = m_settings;
    set->beginGroup("General");
    m_version = set->value("config_version", 0).toInt();
    set->endGroup();*/
}
