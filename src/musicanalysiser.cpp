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

extern "C"
{
#include <libavformat/avformat.h>
#include <libavformat/version.h>
#include "libavutil/dict.h"
}

#include "musicanalysiser.h"
#include "musicmeta.h"

#include <QDebug>

MusicAnalysiser::MusicAnalysiser(QObject *parent) :
    QObject(parent)
{
    av_register_all();
}

MusicAnalysiser::~MusicAnalysiser()
{

}

void MusicAnalysiser::setKeyValue(AVDictionary *dict, const char *key, MusicMeta &meta)
{
    AVDictionaryEntry *entry = av_dict_get(dict, key, NULL, 0);
    if (!entry) {
        return;
    }
    QString avKey = QString::fromStdString(std::string(key));
    if (avKey == "title") {
        meta.fileName = QString(entry->value).simplified();
    }
    else if (avKey == "album") {
        meta.album = QString(entry->value).simplified();
    }
    else if (avKey == "artist") {
        meta.artist = QString(entry->value).simplified();
    }
}

bool MusicAnalysiser::doAnalysis(MusicMeta &meta)
{
    AVFormatContext *formatContext = NULL;
    int result;

    if ((result = avformat_open_input(&formatContext, meta.filePath.toStdString().data(), NULL, NULL))) {
        qDebug() << "avformat_open_input error";
        return false;
    }
    if (!formatContext) {
        avformat_close_input(&formatContext);
        return false;
    }

    AVDictionary *metaData = formatContext->metadata;
    this->setKeyValue(metaData, "title", meta);
    this->setKeyValue(metaData, "album", meta);
    this->setKeyValue(metaData, "artist", meta);

    if (avformat_find_stream_info(formatContext, NULL) < 0) {
        avformat_close_input(&formatContext);
        formatContext = NULL;
        return false;
    }

    AVCodecContext *codecContext = NULL;
    for (int i=0; i<formatContext->nb_streams; ++i) {
        AVStream *stream = formatContext->streams[i];
        if (stream) {
            AVCodecContext *context = stream->codec;
            if(context && context->codec_type == AVMEDIA_TYPE_AUDIO) {
                codecContext = context;
                break;
            }
        }
    }

    if (!codecContext) {
        avformat_close_input(&formatContext);
        formatContext = NULL;
        return false;
    }

    meta.duration = formatContext->duration/(AV_TIME_BASE/1000);//s to ms
    meta.bitRate = (double)meta.size/meta.duration*8 + 0.5;

    avformat_close_input(&formatContext);
    formatContext = NULL;

    return true;
}
