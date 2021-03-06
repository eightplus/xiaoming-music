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

#ifndef _UTILS_H_
#define _UTILS_H_

#include <QString>
#include <QDateTime>

class Utils
{
public:
    static QString dateTimeCovertToStr(const QDateTime &dateTime);
    static QString msecondCovertToSecondStr(const qint64 &msecond);
    static QString makePathIsValid(const QString &path);
    static bool saveLyricFile(const QString &filePath, const QString &content);
};

#endif // _UTILS_H_
