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

#ifndef ANALYSISER_MODULE_H
#define ANALYSISER_MODULE_H

#include <QObject>
#include <QFileInfo>

#include "utils.h"

class MusicAnalysiser;

class AnalysiserModule : public QObject
{
    Q_OBJECT
public:
    explicit AnalysiserModule(QObject *parent = 0);
    ~AnalysiserModule();

    void initMusicAnalysiser();
    void analysisMusicFile(const QFileInfo &fileInfo);

private:
    MusicAnalysiser *m_analysiser = nullptr;
};

#endif // ANALYSISER_MODULE_H
