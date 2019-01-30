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
#include <QFile>
#include <QCryptographicHash>
#include <QNetworkReply>

#include "lyrickugouworker.h"

LyricKugouWorker::LyricKugouWorker(QObject *parent) :
    LyricBaseWorker(parent)

{

}

LyricKugouWorker::~LyricKugouWorker()
{

}

void LyricKugouWorker::searchLyricAndCover(const QString &hashId, const MusicMeta &meta)
{
    if (!m_itemMap.contains(hashId)) {
        MusicSearchItem lyricItem;
        lyricItem.hashId = hashId;
        lyricItem.meta = meta;
        m_itemMap.insert(hashId, lyricItem);

        QString artist = meta.artist;
        QString fileName = meta.fileName;
        QString album = meta.album;

        QString keyWord;
        if (!artist.isEmpty()) {
            keyWord = artist.replace("&", "、") + "-" +fileName;
        }
        else {
            keyWord = fileName;
        }

//        qDebug() << "meta.duration=" << meta.duration << ",artist=" << artist << ",fileName=" << fileName << ",album=" << album << ",keyWord=" << keyWord;
        QString kugouUrl = "http://songsearch.kugou.com/song_search_v2?keyword=" + QUrl::toPercentEncoding(fileName) + "&page=1&pagesize=30&userid=-1&clientver=&platform=WebFilter&tag=em&filter=2&iscorrection=1&privilege_filter=0";
        getRequest(hashId, KugouSearch, kugouUrl);
//        qDebug() << "kugouUrl=" << kugouUrl;

        QString kugouUrl2 = "http://songsearch.kugou.com/song_search_v2?keyword=" + QUrl::toPercentEncoding(keyWord) + "&page=1&pagesize=30&userid=-1&clientver=&platform=WebFilter&tag=em&filter=2&iscorrection=1&privilege_filter=0";
        getRequest(hashId, KugouSearch2, kugouUrl2);

//        QString kugouUrl2 = QString("http://lyrics.kugou.com/search?ver=1&man=yes&client=pc&keyword=%1&duration=%2&hash=").arg(QString(QUrl::toPercentEncoding(keyWord))).arg(QString::number(meta.duration));
//        qDebug() << "kugouUrl2=" << kugouUrl2;
    }
}

void LyricKugouWorker::analysisKugouSearchData(const QString &hashId, CategoryState state, const QByteArray &replyCache)
{
    if (replyCache.isEmpty()) {
        return;
    }
    if (!m_itemMap.contains(hashId)) {
        return;
    }

    MusicSearchItem lyricItem = m_itemMap.value(hashId);
    QJsonObject dataObject= QJsonDocument::fromJson(replyCache).object();
    if (!dataObject.contains("data")) {
        return;
    }
    dataObject = dataObject.value("data").toObject();
    QJsonArray searchItemList = dataObject.value("lists").toArray();
    if (searchItemList.size() == 0) {
        return;
    }

    for(auto i : searchItemList) {
        PostData data;
        auto song = i.toObject();
//        qDebug() << "==================================";
        data.id = song.value("ID").toString();
        data.fileName = song.value("FileName").toString();
//        qDebug() << "id=" << data.id;
        data.albumId = song.value("AlbumID").toString();
//        qDebug() << "fileName=" << data.fileName;
        data.fileHash = song.value("FileHash").toString();
        data.album = song.value("AlbumName").toString();
//        qDebug() << "albumName=" << data.album;
//        qDebug() << "FileHash=" << data.fileHash;
//        data.albumCoverUrl = album.value("blurPicUrl").toString();
//        qDebug() << "album.coverUrl" << data.albumCoverUrl;

        if (state == KugouSearch) {
            lyricItem.searchData1.append(data);
        }
        else if (state == KugouSearch2) {
            lyricItem.searchData2.append(data);
        }
    }

    if (state == KugouSearch) {
        lyricItem.ok1 = true;
        //update map's value by hashId
        m_itemMap.insert(hashId, lyricItem);
    }
    else if (state == KugouSearch2) {
        lyricItem.ok2 = true;
        //update map's value by hashId
        m_itemMap.insert(hashId, lyricItem);
    }

    PostData result;
    if (lyricItem.ok1 && lyricItem.ok2) {
        bool find = false;
        for (auto &titleResult : lyricItem.searchData1) {
            for (auto &albumResult : lyricItem.searchData2) {
                if (titleResult.id == albumResult.id && titleResult.fileHash == albumResult.fileHash && titleResult.album == albumResult.album) {
                    //Attension:  && titleResult.fileName == albumResult.fileName不一定相同，如：
//                    "FileName":"刘维、薛之谦 - <em>婚礼的祝福<\/em> (Live)"
//                    "FileName":"刘维、薛之谦 - 婚礼的祝福 (Live)"
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
        qDebug() << "#######id=" << result.id;
        qDebug() << "#######fileName=" << result.fileName;
        qDebug() << "#######album=" << result.album;
        qDebug() << "#######albumId=" << result.albumId;
        qDebug() << "#######fileHash=" << result.fileHash;
//        qDebug() << "#######album.coverUrl=" << result.albumCoverUrl;
//        qDebug() << "#######lyricUrl=" << result.lyricUrl;
//        lyricItem.searchFinish = true;
        //update map's value by hashId
        m_itemMap.insert(hashId, lyricItem);

        //获取歌词内容，以及专辑封面的url
        QString kugouUrl = QString("http://www.kugou.com/yy/index.php?r=play/getdata&hash=%1&album_id=%2").arg(result.fileHash).arg(result.albumId);
        getRequest(hashId, KugouLyric, kugouUrl);
    }
}

void LyricKugouWorker::analysisKugouLyricOrCoverData(const QString &hashId, CategoryState state, const QByteArray &replyCache)
{
    if (replyCache.isEmpty()) {
        return;
    }
    if (!m_itemMap.contains(hashId)) {
        return;
    }

    if (state == KugouLyric) {
        //歌词
        if (replyCache.length() > 0) {
            /*QFile lyricFile("/home/lixiang/111.lrc");
            lyricFile.open(QIODevice::WriteOnly);
            lyricFile.write(i);
            lyricFile.close();*/


            QJsonObject dataObject= QJsonDocument::fromJson(replyCache).object();
            if (!dataObject.contains("data")) {
                return;
            }
            dataObject = dataObject.value("data").toObject();
            QString lrcContent = dataObject.contains("lyrics") ? dataObject.value("lyrics").toString() : QString();//.toUtf8()
            QString coverUrl = dataObject.contains("img") ? dataObject.value("img").toString() : QString();//.toUtf8()


//            qDebug() << "lrcContent=" << lrcContent;
//            qDebug() << "coverUrl=" << coverUrl;

            //歌词保存路径：  ~/.cache/xiaoming-music
            //专辑封面保存路径： ~/111.cover


            MusicSearchItem lyricItem = m_itemMap.value(hashId);
            //update map
            //m_itemMap.insert(hashId, lyricItem);
            emit lyricSaveFinished(hashId, lyricItem.meta, lrcContent);

            if (!coverUrl.isEmpty()) {
                getRequest(hashId, KugouCover, coverUrl);
            }
        }
    }
    else if (state == KugouCover) {
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

void LyricKugouWorker::analysisData(const QString &hashId, CategoryState state, const QByteArray &replyCache)
{
    if (replyCache.isEmpty()) {
        return;
    }

    switch (state) {
    case NeteaseSearch:
    case NeteaseSearch2:
    case NeteaseLyric:
    case NeteaseCover:
        break;
    case KugouSearch:
    case KugouSearch2:
        this->analysisKugouSearchData(hashId, state, replyCache);
        break;
    case KugouLyric:
    case KugouCover:
        this->analysisKugouLyricOrCoverData(hashId, state, replyCache);
        break;
    default:
        break;
    }
}
