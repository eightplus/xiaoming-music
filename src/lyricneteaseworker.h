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

#ifndef LYRICNETEASEWORKER_H
#define LYRICNETEASEWORKER_H

#include "lyricbaseworker.h"


class LyricNetEaseWorker : public LyricBaseWorker
{
    Q_OBJECT

public:
    explicit LyricNetEaseWorker(QObject *parent = 0);
    ~LyricNetEaseWorker();

    void analysisNeteaseSearchData(const QString &hashId, CategoryState state, const QByteArray &replyCache);
    void analysisNeteaseLyricOrCoverData(const QString &hashId, CategoryState state, const QByteArray &replyCache);

    inline QNetworkRequest generateNeteaseRequest(const QString &url);
    inline QNetworkRequest generateNeteaseRequest2(const QString &url);

protected:
    void searchLyricAndCover(const QString &hashId, const MusicMeta &meta) Q_DECL_OVERRIDE;
    void analysisData(const QString &hashId, CategoryState state, const QByteArray &replyCache) Q_DECL_OVERRIDE;
};

#endif // LYRICNETEASEWORKER_H
