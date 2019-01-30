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

#include "musicanalysiser.h"
#include "musicmeta.h"
#include "global.h"
#include "analysisermodule.h"

#include <QDebug>

AnalysiserModule::AnalysiserModule(QObject *parent) :
    QObject(parent)
{

}

AnalysiserModule::~AnalysiserModule()
{
    if (m_analysiser) {
        m_analysiser->deleteLater();
        m_analysiser = nullptr;
    }
}

void AnalysiserModule::initMusicAnalysiser()
{
    m_analysiser = new MusicAnalysiser;
    m_analysiser->moveToThread(thread());
}

void AnalysiserModule::analysisMusicFile(const QFileInfo &fileInfo, MusicMeta &meta)
{
    meta.fileName = fileInfo.fileName();
    meta.filePath = fileInfo.absoluteFilePath();
    meta.size = fileInfo.size();
    meta.dateAdded = QDateTime::currentDateTime();
    meta.dateLastPlayed = fileInfo.lastRead();
    meta.dateModified = fileInfo.lastModified();
    meta.fileType= globalInstance->fileTypeDescription(fileInfo.suffix());
    m_analysiser->doAnalysis(meta);
    if(meta.duration < 0) {
        meta.duration = 0;
    }
    meta.timeDuration = Utils::msecondCovertToSecondStr(meta.duration);

    qDebug() << "AnalysiserModule Start----------------------";
    qDebug() << "The fileName=" << meta.fileName;
    qDebug() << "The filePath=" << meta.filePath;
    qDebug() << "The album=" << meta.album;
    qDebug() << "The artist=" << meta.artist;
    qDebug() << "The bitRate=" << meta.bitRate;
    qDebug() << "The dateAdded=" << Utils::dateTimeCovertToStr(meta.dateAdded);
    qDebug() << "The dateModified=" << Utils::dateTimeCovertToStr(meta.dateModified);
    qDebug() << "The dateLastPlayed=" << Utils::dateTimeCovertToStr(meta.dateLastPlayed);
    qDebug() << "The fileType=" << meta.fileType;
    qDebug() << "The duration=" << meta.duration;
    qDebug() << "The timeDuration=" << meta.timeDuration;
    qDebug() << "The size=" << globalInstance->byteToString(meta.size);
    qDebug() << "AnalysiserModule End----------------------";
}
