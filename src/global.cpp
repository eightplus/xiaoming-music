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

#include "global.h"
#include "preferences.h"
#include "mainwindow.h"
#include "analysisermodule.h"
#include "lyricmodule.h"

#include <QThread>
#include <QDebug>

Global *Global::m_instance = nullptr;

Global::Global(QObject *parent) :
    QObject(parent),
  m_audioSuffixs(QStringList()),
  m_audioiSuffixsDescription(QStringList()),
  m_lyricModule(new LyricModule(this))
{
    initSizeFactor();
    initMusicFileType();

//    qRegisterMetaType<MusicMesa>("MusicMesa");

    m_preferences = new Preferences;
    m_analysiserModule = new AnalysiserModule;
    m_analysisThread = new QThread(this);
    m_analysiserModule->moveToThread(m_analysisThread);
    m_analysisThread->start();

}

Global::~Global()
{
    delete m_preferences;
    m_preferences = 0;

    m_analysisThread->quit();
    m_analysisThread->wait();

    delete m_analysiserModule;
}

Global *Global::instance()
{
    return m_instance;
}

void Global::initial(QObject *parent)
{
    if (!m_instance) {
        m_instance = new Global(parent);
    }
}

QWidget *Global::mainWindow() const
{
    return static_cast<QWidget *>(m_mainWindow);
}

void Global::setMainWindow(MainWindow *mainWindow)
{
    m_mainWindow = mainWindow;
}

void Global::initSizeFactor()
{
    m_sizeFactor[Byte] = "Byte";
    m_sizeFactor[KiloByte] = "KB";//Kib
    m_sizeFactor[MegaByte] = "MB";//Mib
    m_sizeFactor[GigaByte] = "GB";//Gib
    m_sizeFactor[TeraByte] = "TB";//Tib
}

void Global::initMusicFileType()
{
    m_audioSuffixs<<"mp3"
             <<"m4a"
             <<"wav"
             <<"flac"
             <<"ape"
             <<"ogg"
             <<"oga"
             <<"thd"
             <<"mka"
             <<"aac"
             <<"wma"
             <<"ra"
             <<"dts"
             <<"opus"
             <<"ac3";

    m_audioiSuffixsDescription<<"MPEG Audio Layer III (mp3)"
                       <<"MPEG-4 Part 14 (m4a)"
                       <<"Waveform Audio File Format (wav)"
                       <<"Free Lossless Audio Codec (flac)"
                       <<"Monkey's Audio (ape)"
                       <<"Ogg Vorbis Audio (ogg)"
                       <<"Ogg Vorbis Audio (oga)"
                       <<"Total Harmonic Distortion (thd)"
                       <<"Matroska (mka)"
                       <<"Advanced Audio Coding (aac)"
                       <<"Windows Media Audio (wma)"
                       <<"Real Audio (ra)"
                       <<"DTS Audio Codec (dts)"
                       <<"Opus Audio Codec (opus)"
                       <<"Dolby Surround Audio Coding-3 (ac3)";

}

QString Global::fileTypeDescription(const QString &suffix) const
{

    int index = m_audioSuffixs.indexOf(suffix.toLower());
    return index == -1? QString() : m_audioiSuffixsDescription.at(index);
}

QString Global::byteToString(qreal size)
{
    int index = Byte;
    while (size > 1024.0 && index < SizeInvalid) {
        size /= 1024.0;
        index++;
    }
    return QString::number(size, 'f', 2) + " " + m_sizeFactor[index];
}
