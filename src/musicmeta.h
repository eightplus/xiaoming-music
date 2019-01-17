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

#ifndef _MUSICMETA_H
#define _MUSICMETA_H

#include <QObject>
#include <QString>
#include <QDateTime>

class MusicMeta
{
public:
    QString fileName;//名称
    QString filePath;//路径
    QString album;//专辑
    QString artist;//艺术家
    quint32 bitRate;//比特率 Kbps
    QDateTime dateAdded;//添加日期
    QDateTime dateModified;//修改日期
    QDateTime dateLastPlayed;//上次播放时间
    QString fileType;//文件类型
    qint64 duration;//时长
    QString timeDuration;
    quint64 size;//大小
    qint64 startPosition;//播放位置

    bool invalid = false;
};

Q_DECLARE_METATYPE(MusicMeta)

#endif // _MUSICMETA_H
