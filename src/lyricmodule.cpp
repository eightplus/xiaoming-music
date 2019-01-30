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

#include "lyricmodule.h"
#include "lyricneteaseworker.h"
#include "lyrickugouworker.h"

#include <QFileInfo>
#include <QCryptographicHash>
#include <QDebug>

QString absoluteFilePathHash(const QString &filepath)
{
    return QString(QCryptographicHash::hash(filepath.toUtf8(), QCryptographicHash::Md5).toHex());
}


LyricModule::LyricModule(QObject *parent) :
    QObject(parent),
    m_currentMeta(MusicMeta()),
    m_workingMeta(MusicMeta())
{
//    LyricNetEaseWorker *neteaseWorker = new LyricNetEaseWorker;
//    m_lyricWorkers.append(neteaseWorker);
//    connect(neteaseWorker, &LyricNetEaseWorker::lyricSaveFinished, this, &LyricModule::onLyricSaveFinished);
//    connect(neteaseWorker, &LyricNetEaseWorker::albumCoverSaveFinished, this, &LyricModule::albumCoverSaveFinished);

    LyricKugouWorker *kugouWorker = new LyricKugouWorker;
    m_lyricWorkers.append(kugouWorker);
    connect(kugouWorker, &LyricKugouWorker::lyricSaveFinished, this, &LyricModule::onLyricSaveFinished);
    connect(kugouWorker, &LyricKugouWorker::albumCoverSaveFinished, this, &LyricModule::albumCoverSaveFinished);
}

LyricModule::~LyricModule()
{
    qDeleteAll(m_lyricWorkers);
}

void LyricModule::startLoadLyric(const MusicMeta &meta)
{
    m_currentMeta = meta;
    this->addLyricTask(m_currentMeta);
}

void LyricModule::saveLyricContext(const MusicMeta &meta, const QString &content)
{
    QString lyricPath = "/home/lixiang/.cache/xiaoming-music/" + meta.fileName + ".lrc";
    bool b = Utils::saveLyricFile(lyricPath, content);
    if (b) {
        qDebug() << "###下载歌词成功###";
    }
    else {
        qDebug() << "###下载歌词失败###";
    }
}

void LyricModule::addLyricTask(const MusicMeta &meta)
{
    //检查需要处理的音频文件是否和正在处理的音频文件是同一个
    if (meta.fileName == m_workingMeta.fileName && meta.filePath == m_workingMeta.filePath) {
        return;
    }

    if (m_lyricsTaskList.size() == 0) {
        m_lyricsTaskList.append(meta);
    }
    else {
        for (auto i : m_lyricsTaskList) {
            //检查需要处理的音频文件是否在处理队列中
            if (meta.fileName == i.fileName && meta.filePath == i.filePath) {
                return;
            }
        }
        m_lyricsTaskList.append(meta);
    }

    this->doTask();
}

void LyricModule::doTask()
{
    if (m_lyricsTaskList.isEmpty()) {
        return;
    }

    m_workingMeta = m_lyricsTaskList.takeLast();

    for(auto i = m_lyricWorkers.begin(); i != m_lyricWorkers.end(); ++i) {
        QString hashId = absoluteFilePathHash(m_workingMeta.filePath);
        (*i)->readyToSearchLyricAndCover(hashId, m_workingMeta);
    }
}

void LyricModule::onLyricSaveFinished(const QString &hash, const MusicMeta &meta, QString lyricContext)
{
//    if (m_currentFileHash != hash) {
//        return;
//    }
    m_lyricsList.append(lyricContext);
    for (auto i : m_lyricsList) {
        //保存歌词 有一个获取歌词成功则退出，不再继续获取
        QString lyricPath = "/home/lixiang/.cache/xiaoming-music/" + meta.fileName + ".lrc";
        if (QFileInfo::exists(lyricPath)){
            break;
        }

        this->saveLyricContext(meta, i);
        break;
    }
    m_lyricsList.clear();
    m_workingMeta = MusicMeta();

    this->doTask();
}
