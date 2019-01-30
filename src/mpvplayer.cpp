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

#include "mpvplayer.h"
#include "mainwindow.h"
#include "global.h"
#include "preferences.h"

#include <QApplication>
#include <QEasingCurve>
#include <QDebug>

static void mpvWakeupHandle(void *context)
{
    emit static_cast<MpvPlayer *>(context)->mpvApiEvents();
}


MpvPlayer::MpvPlayer(QObject *parent) : QObject(parent),
    m_mpvHandle(nullptr),
    m_audioDevice(QString()),
    m_filePath(QString()),
    m_forceStereo(false),
    m_startPosition(-1),
    m_endPosition(-1),
    m_nowDuration(-1),
    m_duration(-1),
    m_position(-1),
    m_state(Stopped),
    m_fileLoaded(false),
    m_mute(false),
    m_maxVolume(10000)
{
    setlocale(LC_NUMERIC, "C");
    connect(this, &MpvPlayer::mpvApiEvents, this, &MpvPlayer::onMpvApiEvents, Qt::QueuedConnection);

    //TODO
    m_audioDevice = "auto";
    m_bufferLength = QString::number((qreal)500/1000.0, 'g', 3);
    m_sampleRate = -1;
    m_forceStereo = false;
}

MpvPlayer::~MpvPlayer()
{
    if (this->state() == Playing) {
        this->pause();
    }

    resetParameters();
    destroyMpvEngine();
}

bool MpvPlayer::initMpvEngine()
{
    if (m_mpvHandle) {
        return true;
    }
    m_mpvHandle = mpv_create();
    if (!m_mpvHandle) {
        return false;
    }

    mpv_set_option_string(m_mpvHandle, "audio-display", "no");
    mpv_set_option_string(m_mpvHandle, "vo", "null");
    if (!m_audioDevice.isEmpty()) {
        mpv_set_option_string(m_mpvHandle, "audio-device", m_audioDevice.toLatin1().data());
    }
    if (!m_bufferLength.isEmpty()) {
        mpv_set_option_string(m_mpvHandle, "audio-buffer", m_bufferLength.toLatin1().data());
    }
    if (m_sampleRate != -1) {
        mpv_set_option_string(m_mpvHandle, "audio-samplerate", QString::number(m_sampleRate).toLatin1().data());
    }
    if (m_forceStereo) {
        mpv_set_option_string(m_mpvHandle, "audio-channels", "stereo");
    }
    mpv_observe_property(m_mpvHandle, 0, "playback-time", MPV_FORMAT_DOUBLE);
    mpv_set_wakeup_callback(m_mpvHandle, mpvWakeupHandle, this);
    mpv_initialize(m_mpvHandle);

    return true;
}

void MpvPlayer::destroyMpvEngine()
{
    if (m_mpvHandle) {
        mpv_terminate_destroy(m_mpvHandle);
        m_mpvHandle = nullptr;
    }
}

void MpvPlayer::resetParameters()
{
    if (!m_mpvHandle) {
        return;
    }
    const char *arguments[]={"stop"};
    mpv_command(m_mpvHandle, arguments);
//    qDebug() << "call setState 111";
    setState(Stopped);

    m_filePath.clear();
    m_forceStereo = false;
    m_startPosition = -1;
    m_endPosition = -1;
    m_nowDuration = -1;
    m_duration =-1;
    m_position= -1;
    m_state = Stopped;
    m_fileLoaded = false;
    m_mute = false;
}

bool MpvPlayer::initEngineAndloadMusic(const QString &filePath, const qint64 &start, const qint64 &duration)
{
    resetParameters();

    bool b = initMpvEngine();
    if (!b) {
        return false;
    }
    b =  mpvLoadFile(filePath);
    if (!b) {
        return false;
    }

    this->updateSeekPos(start, duration);

    return true;
}

bool MpvPlayer::mpvLoadFile(const QString &filePath)
{
    m_filePath = filePath;
    const QByteArray utfFilePath = filePath.toUtf8();
    const char *arguments[] = {"loadfile", utfFilePath.data(), NULL};

    return mpv_command(m_mpvHandle, arguments)==0;
}

void MpvPlayer::play()
{
    if (!m_mpvHandle) {
        return;
    }

    if (Stopped == m_state) {
        if(!m_fileLoaded && !m_filePath.isEmpty()) {
            mpvLoadFile(m_filePath);
        }
        seek(m_position == -1 ? 0: m_position);
    }

    int flag = 0;
    mpv_set_property(m_mpvHandle, "pause", MPV_FORMAT_FLAG, &flag);
//    qDebug() << "call setState 333";
    setState(Playing);
}

void MpvPlayer::pause()
{
    if (!m_mpvHandle || m_state!=Playing) {
        return;
    }

    int flag=1;
    mpv_set_property(m_mpvHandle, "pause", MPV_FORMAT_FLAG, &flag);
    setState(Paused);
}

void MpvPlayer::stop()
{
    if (!m_mpvHandle) {
        return;
    }

    int flag=1;
    mpv_set_property(m_mpvHandle, "pause", MPV_FORMAT_FLAG, &flag);
    const char *arguments[]={"stop"};
    mpv_command(m_mpvHandle, arguments);

    m_fileLoaded = false;
    m_position = m_startPosition;

    emit positionChanged(0);
//    qDebug() << "call setState 222";
    setState(Stopped);
}


void MpvPlayer::setState(int state)
{
    if (state != m_state) {
        m_state = state;
    }
}

void MpvPlayer::seek(const qint64 &position)
{
    qint64 nowPos = position;
    if (m_startPosition > -1 && m_endPosition > -1) {
        nowPos += m_startPosition;
        if (nowPos > m_endPosition) {
            nowPos = m_endPosition;
        }
    }

    double seekPos = (double)(nowPos)/1000.0;
    const QByteArray positionData = QString::number(seekPos).toUtf8();
    const char *arguments[] = {"seek", positionData.constData(), "absolute", NULL};
    mpv_command(m_mpvHandle, arguments);
}

void MpvPlayer::setVolume(int volume)
{
    if (mute()) {
        setMute(false);
    }

    if (volume < 0) {
        volume = 0;
    }
    else if (volume > m_maxVolume) {
        volume = m_maxVolume;
    }

    globalInstance->preferences()->m_volume = volume;

    this->setMpvVolumeProperty(volume);
}

void MpvPlayer::setMpvVolumeProperty(const int &volume)
{
    if (m_mpvHandle) {
        double mpvVolume= (double)volume/100.0;
        QEasingCurve volumeCurve = QEasingCurve(QEasingCurve::OutQuad);
        mpvVolume = volumeCurve.valueForProgress(mpvVolume/100.0)*100.0;
        mpv_set_property(m_mpvHandle, "volume", MPV_FORMAT_DOUBLE, &mpvVolume);
    }
}

void MpvPlayer::setMute(bool mute)
{
    if (m_mute == mute) {
        return;
    }

    m_mute = mute;

    if (m_mute) {
        this->setMpvVolumeProperty(0);
    }
    else {
        this->setMpvVolumeProperty(globalInstance->preferences()->m_volume);
    }
}

void MpvPlayer::updateSeekPos(const qint64 &start, const qint64 &duration)
{
    m_startPosition = start;

    if (duration != -1) {
        m_nowDuration = duration;
        m_endPosition = m_startPosition + m_nowDuration;
    }
    if (m_duration != -1) {
        if (m_startPosition == -1) {//开始的位置是-1，则播放整首歌
            m_startPosition = 0;
            m_nowDuration = m_duration;
            m_endPosition = -1;
        }
        else {
            if (m_endPosition > m_duration) {
                m_endPosition = m_duration;
            }
            if (m_startPosition > m_endPosition) {
                m_startPosition = m_endPosition;
            }
            m_nowDuration = m_endPosition - m_startPosition;
        }
        //TODO:需要将m_duration传递给UI去更新进度条

        seek(0);
    }
}

void MpvPlayer::musicPlayEnd()
{
    m_fileLoaded = false;
    stop();
}

void MpvPlayer::playOrPause()
{
    if (state() == Playing) {
        pause();
        return;
    }

    play();
}

void MpvPlayer::onMpvApiEvents()
{
    mpv_event *event = nullptr;
    if (m_mpvHandle) {
        event = mpv_wait_event(m_mpvHandle, 0);
    }

    /*while (1) {
        mpv_event *event = mpv_wait_event(m_mpvHandle, -1);
        qDebug() << "Got event: " << event->event_id;
        if (event->event_id == MPV_EVENT_SHUTDOWN)
            break;
    }*/

    while (event && event->event_id != MPV_EVENT_NONE) {
        //printf("event: %s\n", mpv_event_name(event->event_id));
        switch (event->event_id)
        {
        case MPV_EVENT_FILE_LOADED://文件加载后会触发该信号,即执行了mpv_command::loadfile之后
        {
            m_fileLoaded = true;

            int flag=1;
            mpv_set_property(m_mpvHandle, "pause", MPV_FORMAT_FLAG, &flag);
//            qDebug() << "call setState 555";
            setState(Paused);

            //获取音频时长
            double duration;
            mpv_get_property(m_mpvHandle, "duration", MPV_FORMAT_DOUBLE, &duration);
            m_duration = (qint64)(duration*1000.0);
            emit musicLoadSuccess();
            break;
        }
        case MPV_EVENT_PROPERTY_CHANGE:
        {
            mpv_event_property *property = (mpv_event_property *)event->data;
            if (strcmp(property->name, "playback-time") == 0) {
                if (property->format == MPV_FORMAT_DOUBLE) {//MPV_FORMAT_INT64
                    double time = *(double *)property->data;
                    m_position = (qint64)(time*1000.0);
                    emit positionChanged(m_position - m_startPosition);
                    if (m_endPosition > 0 && m_position >= m_endPosition)  {
                        musicPlayEnd();
                    }
                }
            }
            break;
        }
        case MPV_EVENT_UNPAUSE:
        {
//            qDebug() << "call setState 666";
            setState(Playing);
            setMpvVolumeProperty(globalInstance->preferences()->m_volume);
            break;
        }
        case MPV_EVENT_END_FILE:
            if (m_fileLoaded) {
                musicPlayEnd();
            }
            else {
                emit musicLoadFailed();
            }
            break;
//        case MPV_EVENT_SHUTDOWN:
//            break;
        default:
            break;
        };

        if(m_mpvHandle) {
            event = mpv_wait_event(m_mpvHandle, 0);
        }
        qApp->processEvents();
    }
}
