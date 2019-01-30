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

#include "lyricbaseworker.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>

LyricBaseWorker::LyricBaseWorker(QObject *parent) :
    QObject(parent),
    m_accessManager(new QNetworkAccessManager())
{
    connect(m_accessManager, &QNetworkAccessManager::finished, this, &LyricBaseWorker::onReplyFinished);
}

LyricBaseWorker::~LyricBaseWorker()
{
    m_accessManager->deleteLater();
}

void LyricBaseWorker::readyToSearchLyricAndCover(const QString &hashId, const MusicMeta &meta)
{
    this->searchLyricAndCover(hashId, meta);
}

bool LyricBaseWorker::findReply(QNetworkReply *reply)
{
    bool find = false;
    for (auto key : m_itemMap.keys()) {
        MusicSearchItem lyricItem = m_itemMap.value(key);
        if (lyricItem.replyList.contains(reply)) {
            find = true;
            break;
        }
    }
    if (!find) {
        reply->abort();
        return false;
    }
    else {
        return true;
    }
}

void LyricBaseWorker::cancel(const QString &hashId)
{
    if (!m_itemMap.contains(hashId)) {
        return;
    }

    MusicSearchItem lyricItem = m_itemMap.value(hashId);
    QList<QNetworkReply *> replyList = lyricItem.replyList;
    if (!replyList.isEmpty()) {
        for(auto reply: replyList) {
            lyricItem.replyList.removeOne(reply);
            lyricItem.stateMap.remove(reply);
            reply->abort();
            reply->deleteLater();
        }
        m_itemMap.insert(hashId, lyricItem);
    }
}

void LyricBaseWorker::getRequest(const QString &hashId, CategoryState state, const QNetworkRequest &request/*, const QVariant &user*/)
{
    if (!m_itemMap.contains(hashId)) {
        return;
    }
    QNetworkReply *reply = m_accessManager->get(request);
    //    connect(reply, SIGNAL(finished()), this, SLOT(onReplyFinished()));
    //    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onReplyError(QNetworkReply::NetworkError)));
    MusicSearchItem lyricItem = m_itemMap.value(hashId);
    lyricItem.replyList.append(reply);

    CategoryState m_state;
    switch (state) {
    case NeteaseSearch:
    case NeteaseSearch2:
    case NeteaseLyric:
    case NeteaseCover:
    case KugouSearch:
    case KugouSearch2:
    case KugouLyric:
    case KugouCover:
        m_state = state;
        break;
    default:
        break;
    }
    lyricItem.stateMap.insert(reply, m_state);
    m_itemMap.insert(hashId, lyricItem);
}

void LyricBaseWorker::getRequest(const QString &hashId, CategoryState state, const QString &url/*, const QVariant &user*/)
{
    qDebug() << "getRequest url=" << url;
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    getRequest(hashId, state, request);
}

void LyricBaseWorker::postRequest(const QString &hashId, CategoryState state, const QNetworkRequest &request, const QByteArray &data/*, const QVariant &user*/)
{
    if (!m_itemMap.contains(hashId)) {
        return;
    }

    QNetworkReply *reply = m_accessManager->post(request, data);
//    connect(reply, SIGNAL(finished()), this, SLOT(onReplyFinished()));
//    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onReplyError(QNetworkReply::NetworkError)));

    //QMap<QString, MusicSearchItem>::iterator it = m_itemMap.find(hashId);
//    MusicSearchItem &lyricItem = it.value();
    MusicSearchItem lyricItem = m_itemMap.value(hashId);
    lyricItem.replyList.append(reply);
    CategoryState m_state;
    switch (state) {
    case NeteaseSearch:
    case NeteaseSearch2:
    case NeteaseLyric:
    case NeteaseCover:
    case KugouSearch:
    case KugouSearch2:
    case KugouLyric:
    case KugouCover:
        m_state = state;
        break;
    default:
        break;
    }
    lyricItem.stateMap.insert(reply, m_state);
    //update map's value by hashId
    m_itemMap.insert(hashId, lyricItem);
}

void LyricBaseWorker::onReplyFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "LyricBaseWorker reply error:" << reply->errorString();
        foreach (QString key, m_itemMap.keys()) {
            MusicSearchItem lyricItem = m_itemMap.value(key);
            if (lyricItem.replyList.contains(reply)) {
                lyricItem.replyList.removeOne(reply);
                //update map
                m_itemMap.insert(key, lyricItem);
                break;
            }
        }
        reply->deleteLater();
        return;
    }

    MusicSearchItem m_lyricItem;
    QString hashId;
    bool find = false;
    for (auto key : m_itemMap.keys()) {
        MusicSearchItem lyricItem = m_itemMap.value(key);
        if (lyricItem.replyList.contains(reply)) {
            hashId = key;
            m_lyricItem = lyricItem;
            find = true;
            break;
        }
    }
    if (!find) {
        reply->abort();
        return;
    }

    CategoryState state = m_lyricItem.stateMap.take(reply);
    QByteArray replyData;
    replyData = reply->readAll();
    m_lyricItem.replyList.removeOne(reply);
    m_lyricItem.stateMap.remove(reply);
    reply->deleteLater();
    m_itemMap.insert(hashId, m_lyricItem);

    this->analysisData(hashId, state, replyData);
}
