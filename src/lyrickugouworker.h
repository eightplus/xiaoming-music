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

#ifndef LYRICKUGOUWORKER_H
#define LYRICKUGOUWORKER_H

#include "lyricbaseworker.h"

class LyricKugouWorker : public LyricBaseWorker
{
    Q_OBJECT

public:
    explicit LyricKugouWorker(QObject *parent = 0);
    ~LyricKugouWorker();

    void analysisKugouSearchData(const QString &hashId, CategoryState state, const QByteArray &replyCache);
    void analysisKugouLyricOrCoverData(const QString &hashId, CategoryState state, const QByteArray &replyCache);

protected:
    void searchLyricAndCover(const QString &hashId, const MusicMeta &meta) Q_DECL_OVERRIDE;
    void analysisData(const QString &hashId, CategoryState state, const QByteArray &replyCache) Q_DECL_OVERRIDE;
};

#endif // LYRICKUGOUWORKER_H
