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

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QTextCodec>
#include <QDir>
#include <QDebug>

#include "utils.h"

QString Utils::dateTimeCovertToStr(const QDateTime &dateTime)
{
    return dateTime.toString("yyyy-MM-dd AP hh:mm");
}

QString Utils::msecondCovertToSecondStr(const qint64 &msecond)
{
    qint64 second = msecond/1000;
    QString sencondStr = QString::number(second%60);
    if (sencondStr.length() == 1) {//seconds are in digits
        return QString::number(second/60) + ":0" + sencondStr;
    }
    else {//seconds are multi-digit
        return QString::number(second/60) + ":" + sencondStr;
    }
}

QString Utils::makePathIsValid(const QString &path)
{
    QFileInfo fileInfo(path);
    if (fileInfo.isDir() && fileInfo.exists()) {
        return fileInfo.absoluteFilePath();
    }

    if (fileInfo.isFile()) {
        if (!QFile::remove(fileInfo.absoluteFilePath())) {
            return QString();
        }
    }

    return QDir().mkpath(fileInfo.absoluteFilePath()) ? fileInfo.absoluteFilePath() : QString();
}

bool Utils::saveLyricFile(const QString &filePath, const QString &content)
{
    QFile fp(filePath);

    if (makePathIsValid(QFileInfo(fp).absolutePath()).isEmpty() || !fp.open(QIODevice::WriteOnly)) {
        return false;
    }

    QTextStream textStream(&fp);
    textStream.setCodec(QTextCodec::codecForName("UTF-8"));
    textStream << content << flush;
    fp.close();

    return true;
}
