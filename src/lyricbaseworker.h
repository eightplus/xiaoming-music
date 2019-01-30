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

#ifndef LYRICBASEWORKER_H
#define LYRICBASEWORKER_H

#include "utils.h"
#include "musicmeta.h"

#include <QMap>

class QNetworkReply;
class QNetworkRequest;
class QNetworkAccessManager;

class LyricBaseWorker : public QObject
{
    Q_OBJECT
public:
    enum CategoryState
    {
        NeteaseInit = 0,
        NeteaseSearch,
        NeteaseSearch2,
        NeteaseLyric,
        NeteaseCover,
        KugouSearch,
        KugouSearch2,
        KugouLyric,
        KugouCover,
        NeteaseInvalid
    };

    struct PostData
    {
        QString id;
        QString fileName;
        QString fileHash;
        QString album;
        QString albumId;
        QString artist;
        QString albumCoverUrl;
        QString lyricUrl;
    };

    struct MusicSearchItem
    {
        QString hashId;
        QList<QNetworkReply *> replyList;
        QMap<QNetworkReply *, CategoryState> stateMap;
        MusicMeta meta;
        QList<PostData> searchData1;
        QList<PostData> searchData2;
        bool ok1;
        bool ok2;
        bool coverOk;
        bool lyricOk;
        bool searchFinish;
        MusicSearchItem() :
            ok1(false),
            ok2(false),
            coverOk(false),
            lyricOk(false),
            searchFinish(false)
        {
        }
    };

    explicit LyricBaseWorker(QObject *parent = 0);
    ~LyricBaseWorker();

    void readyToSearchLyricAndCover(const QString &hashId, const MusicMeta &meta);
    bool findReply(QNetworkReply *reply);
    QMap<QString, MusicSearchItem> m_itemMap;

signals:
    void lyricSaveFinished(const QString &hash, MusicMeta meta, QString lyricContext);
    void albumCoverSaveFinished(const QByteArray &coverData);

public slots:
    void onReplyFinished(QNetworkReply *reply);
    void cancel(const QString &hashId);

protected:
    virtual void searchLyricAndCover(const QString &hashId, const MusicMeta &meta) = 0;
    virtual void analysisData(const QString &hashId, CategoryState state, const QByteArray &replyCache) = 0;

    void getRequest(const QString &hashId, CategoryState state, const QNetworkRequest &request);
    void getRequest(const QString &hashId, CategoryState state, const QString &url);
    void postRequest(const QString &hashId, CategoryState state, const QNetworkRequest &request, const QByteArray &content);

private:
    QNetworkAccessManager *m_accessManager;
};

#endif // LYRICBASEWORKER_H
