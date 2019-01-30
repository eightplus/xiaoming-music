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

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDir>
#include <QTimer>
#include <QFile>
#include <QNetworkReply>

#include "lyricneteaseworker.h"

LyricNetEaseWorker::LyricNetEaseWorker(QObject *parent) :
    LyricBaseWorker(parent)

{

}

LyricNetEaseWorker::~LyricNetEaseWorker()
{

}


void LyricNetEaseWorker::searchLyricAndCover(const QString &hashId, const MusicMeta &meta)
{
    /*
    http://music.163.com/api/search/pc
    可以获取专辑封面Url

    http://music.163.com/api/search/get
    获取专辑封面Url为空
    */

    if (!m_itemMap.contains(hashId)) {
        MusicSearchItem lyricItem;
        lyricItem.hashId = hashId;
        lyricItem.meta = meta;
        m_itemMap.insert(hashId, lyricItem);

        QString artist = meta.artist;
        QString fileName = meta.fileName;
        QString album = meta.album;
    //    QString neteaseUrl = "http://music.163.com/api/search/get/?s=" + artist + "-" + fileName + "&limit=10&type=1&offset=0";
        QString neteaseUrl = "http://music.163.com/api/search/pc/?s=" + fileName + "&limit=10&type=1&offset=0";
    //    QString neteaseUrl = "http://music.163.com/api/search/get/?s=" + fileName + "&limit=10&type=1&offset=0";
//        qDebug() << "neteaseUrl=" << neteaseUrl;
        postRequest(/*identifier, */hashId, NeteaseSearch, generateNeteaseRequest(neteaseUrl), QByteArray());

    //    QString neteaseUrl2 = "http://music.163.com/api/search/get/?s=" + fileName + album + "&limit=10&type=1&offset=0";
        QString neteaseUrl2 = "http://music.163.com/api/search/pc/?s=" + fileName + album + "&limit=10&type=1&offset=0";
//        qDebug() << "neteaseUrl2=" << neteaseUrl2;
        postRequest(/*identifier, */hashId, NeteaseSearch2, generateNeteaseRequest(neteaseUrl), QByteArray());
    //    "s=%1&offset=0&limit=5&type=1"
    }
}

void LyricNetEaseWorker::analysisNeteaseSearchData(const QString &hashId, CategoryState state, const QByteArray &replyCache)
{
    if (replyCache.isEmpty()) {
        return;
    }
    if (!m_itemMap.contains(hashId)) {
        return;
    }

    MusicSearchItem lyricItem = m_itemMap.value(hashId);
    QJsonObject dataObject= QJsonDocument::fromJson(replyCache).object();
    if (!dataObject.contains("result")) {
        return;
    }
    dataObject = dataObject.value("result").toObject();
    QJsonArray searchItemList = dataObject.value("songs").toArray();
    if (searchItemList.size() == 0) {
        return;
    }

    for(auto i : searchItemList) {
        PostData data;
        auto song = i.toObject();
        QString url = "http://music.163.com/api/song/lyric?os=osx&id="+QString::number(qint64(song.value("id").toDouble()))+"&lv=-1&kv=-1&tv=-1";
//            qDebug() << "KNMusicNeteaseLyrics 666 url="<<url;
        data.lyricUrl = url;
        auto length = song.value("bMusic").toObject().value("playTime").toInt();
//            qDebug() << "length=" << length;
        data.id = /*QString("netease_%1").arg(*/song.value("id").toString();
        data.fileName = song.value("name").toString();
//            qDebug() << "id=" << data.id;
//            qDebug() << "fileName=" << data.fileName;
        auto album = song.value("album").toObject();
        data.album = album.value("name").toString();
//            qDebug() << "album.name=" << data.album;
        data.albumCoverUrl = album.value("blurPicUrl").toString();
//        qDebug() << "album.coverUrl" << data.albumCoverUrl;
        auto artists = song.value("artists").toArray();
        for (auto artistJson : artists) {
            auto artist = artistJson.toObject();
//                qDebug() << "neArtist.name" << artist.value("name").toString();
        }

        if (state == NeteaseSearch) {
            lyricItem.searchData1.append(data);
        }
        else if (state == NeteaseSearch2) {
            lyricItem.searchData2.append(data);
        }
    }

    if (state == NeteaseSearch) {
        lyricItem.ok1 = true;
        //update map's value by hashId
        m_itemMap.insert(hashId, lyricItem);
    }
    else if (state == NeteaseSearch2) {
        lyricItem.ok2 = true;
        //update map's value by hashId
        m_itemMap.insert(hashId, lyricItem);
    }

    PostData result;
    if (lyricItem.ok1 && lyricItem.ok2) {
        bool find = false;
        for (auto &titleResult : lyricItem.searchData1) {
            for (auto &albumResult : lyricItem.searchData2) {
                if (titleResult.id == albumResult.id && titleResult.album == albumResult.album && titleResult.fileName == albumResult.fileName) {
                    result = titleResult;
                    find = true;
                    break;
                }
            }
            if (find) {
                break;
            }
        }
        if (!find) {
            result = lyricItem.searchData2.at(0);
            find = true;
        }
        lyricItem.ok1 = false;
        lyricItem.ok2 = false;
//        qDebug() << "#######id=" << result.id;
//        qDebug() << "#######fileName=" << result.fileName;
//        qDebug() << "#######album.name=" << result.album;
//        qDebug() << "#######album.coverUrl=" << result.albumCoverUrl;
//        qDebug() << "#######lyricUrl=" << result.lyricUrl;
//        lyricItem.searchFinish = true;
        //update map's value by hashId
        m_itemMap.insert(hashId, lyricItem);

        //获取歌词
        getRequest(hashId, NeteaseLyric, generateNeteaseRequest(result.lyricUrl));
        //获取专辑封面
        getRequest(hashId, NeteaseCover, generateNeteaseRequest2(result.albumCoverUrl));
    }
}

void LyricNetEaseWorker::analysisNeteaseLyricOrCoverData(const QString &hashId, CategoryState state, const QByteArray &replyCache)
{
    if (replyCache.isEmpty()) {
        return;
    }
    if (!m_itemMap.contains(hashId)) {
        return;
    }

    if (state == NeteaseLyric) {
        //歌词
        if (replyCache.length() > 0) {
            /*QFile lyricFile("/home/lixiang/111.lrc");
            lyricFile.open(QIODevice::WriteOnly);
            lyricFile.write(i);
            lyricFile.close();*/
            qDebug() << "analysisNeteaseLyricOrCoverData 44";

            QJsonObject lyricsObject = QJsonDocument::fromJson(replyCache).object();
            QString lrcContent = lyricsObject.contains("lrc") ? lyricsObject.value("lrc").toObject().value("lyric").toString() : QString();//.toUtf8()
            QString klyricsContent = lyricsObject.contains("klyric") ? lyricsObject.value("klyric").toObject().value("lyric").toString() : QString();//.toUtf8()
            MusicSearchItem lyricItem = m_itemMap.value(hashId);
            //update map
            //m_itemMap.insert(hashId, lyricItem);
            emit lyricSaveFinished(hashId, lyricItem.meta, lrcContent + klyricsContent/*lyricsDetail*/);
        }
    }
    else if (state == NeteaseCover) {
        //专辑封面
        if (replyCache.length() > 0) {
            QString coverFilePath = QDir::homePath() + "/" + "111.cover";
            QFile coverFile(coverFilePath);
            coverFile.open(QIODevice::WriteOnly);
            coverFile.write(replyCache);
            coverFile.close();
            qDebug() << "封面保存到本地成功";
            emit albumCoverSaveFinished(replyCache);
        }
    }
}

void LyricNetEaseWorker::analysisData(const QString &hashId, CategoryState state, const QByteArray &replyCache)
{
    if (replyCache.isEmpty()) {
        return;
    }

    switch (state) {
    case NeteaseSearch:
    case NeteaseSearch2:
        this->analysisNeteaseSearchData(hashId, state, replyCache);
        break;
    case NeteaseLyric:
    case NeteaseCover:
        this->analysisNeteaseLyricOrCoverData(hashId, state, replyCache);
        break;
    case KugouSearch:
    case KugouSearch2:
    case KugouLyric:
    case KugouCover:
        break;
    default:
        break;
    }
}

inline QNetworkRequest LyricNetEaseWorker::generateNeteaseRequest(const QString &url)
{
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setUrl(QUrl(url));
    request.setRawHeader("Cookie", "appver=2.0.2");
    request.setRawHeader("Referer", "http://music.163.com/search/");
    request.setRawHeader("Original", "http://music.163.com");
    request.setRawHeader("Connection", "keep-alive");
    request.setRawHeader("Host", "music.163.com");
    request.setRawHeader("User-Agent",
                         "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_9_2) "
                         "AppleWebKit/537.36 (KHTML, like Gecko) "
                         "Chrome/33.0.1750.152 "
                         "Safari/537.36");

    return request;
}

inline QNetworkRequest LyricNetEaseWorker::generateNeteaseRequest2(const QString &albumCoverUrl)
{
    //获取专辑封面
    QNetworkRequest request(albumCoverUrl);
    request.setRawHeader("Content-Type", "application/x-www-form-urlencoded");
    request.setRawHeader("Cookie", "appver=1.5.0.75771;");//appver=2.0.2
    request.setRawHeader("Referer", "http://music.163.com/");//http://music.163.com/search/

    return request;
}
