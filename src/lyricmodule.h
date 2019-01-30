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

#ifndef LYRICMODULE_H
#define LYRICMODULE_H

#include "utils.h"
#include "musicmeta.h"

#include <QObject>

class LyricBaseWorker;

class LyricModule : public QObject
{
    Q_OBJECT
public:
    explicit LyricModule(QObject *parent = 0);
    ~LyricModule();

signals:
    void albumCoverSaveFinished(const QByteArray &coverData);

public slots:
    void startLoadLyric(const MusicMeta &meta);
    void addLyricTask(const MusicMeta &meta);
    void doTask();
    void onLyricSaveFinished(const QString &hash, const MusicMeta &meta, QString lyricContext);
    void saveLyricContext(const MusicMeta &meta, const QString &content);

private:
    QList<MusicMeta> m_lyricsTaskList;
    QList<LyricBaseWorker *> m_lyricWorkers;
    QStringList m_lyricsList;
    MusicMeta m_workingMeta;
    MusicMeta m_currentMeta;
    QString m_currentFileHash;
};

#endif // LYRICMODULE_H
