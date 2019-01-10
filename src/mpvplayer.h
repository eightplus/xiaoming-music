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

#ifndef MPVPLAYER_H
#define MPVPLAYER_H

#include <QObject>
#include <mpv/client.h>

class MpvPlayer : public QObject
{
    Q_OBJECT
public:

    enum PlayingState
    {
        Stopped,
        Playing,
        Paused
    };

    explicit MpvPlayer(QObject *parent = 0);
    ~MpvPlayer();

    bool initMpvEngine();
    void destroyMpvEngine();

    void resetParameters();

    bool initEngineAndloadMusic(const QString &filePath, const qint64 &start=-1, const qint64 &duration=-1);
    bool mpvLoadFile(const QString &filePath);

    void play();
    void pause();
    void stop();

    void setState(int state);
    int state() const {return m_state;}

    void seek(const qint64 &position);
    qint64 position() const {return m_position - m_startPosition;}

    void setVolume(int volume);
    void setMpvVolumeProperty(const int &volume);
    void setMute(bool mute);
    bool mute() {return m_mute;}

    void updateSeekPos(const qint64 &start=-1, const qint64 &duration=-1);
    void musicPlayEnd();

signals:
    void mpvApiEvents();
    void musicLoadFailed();
    void musicLoadSuccess();
    void positionChanged(qint64 position);

public slots:
    void playOrPause();
    void onMpvApiEvents();

private:
    mpv_handle *m_mpvHandle = nullptr;

    QString m_audioDevice;//音频驱动
    QString m_filePath;//音频文件
    QString m_bufferLength;//buffer长度
    int m_sampleRate;//音频采样率
    bool m_forceStereo;//是否强制立体声

    qint64 m_startPosition;
    qint64 m_endPosition;

    qint64 m_nowDuration;//音频文件当前的播放时长
    qint64 m_duration;//音频文件的总时长

    qint64 m_position;
    int m_state;//播放状态

    bool m_fileLoaded;//mpv是否已加载完音频文件的标识

    bool m_mute;
    int m_maxVolume;
};

#endif // MPVPLAYER_H
