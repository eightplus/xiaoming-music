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

#include "mainwindow.h"
#include "mpvplayer.h"
#include "global.h"
#include "preferences.h"
#include "analysisermodule.h"
#include "lyricmodule.h"

#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QFileInfo>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->resize(900, 600);
    m_centralWidget = new QWidget(this);
    m_centralWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    m_centralWidget->setMinimumSize(QSize(1,1));
    m_centralWidget->setFocusPolicy(Qt::StrongFocus);
    m_centralWidget->setFocus();

    QHBoxLayout *m_layout = new QHBoxLayout;
    m_layout->setSpacing(20);
    m_playBtn = new QPushButton(this);
    m_playBtn->setFixedSize(100, 30);
    m_playBtn->setText("Play/Pause");
    m_stopBtn = new QPushButton(this);
    m_stopBtn->setFixedSize(100, 30);
    m_stopBtn->setText("Stop");
    m_prevBtn = new QPushButton(this);
    m_prevBtn->setFixedSize(100, 30);
    m_prevBtn->setText("Prev");
    m_nextBtn = new QPushButton(this);
    m_nextBtn->setFixedSize(100, 30);
    m_nextBtn->setText("Next");
    m_ablumLabel = new QLabel(this);
    m_ablumLabel->setFixedSize(150, 150);
    m_layout->addWidget(m_playBtn);
    m_layout->addWidget(m_stopBtn);
    m_layout->addWidget(m_prevBtn);
    m_layout->addWidget(m_nextBtn);
    m_layout->addWidget(m_ablumLabel);
    m_centralWidget->setLayout(m_layout);

    this->setCentralWidget(m_centralWidget);

    connect(m_playBtn, &QPushButton::clicked, this, [=] {
        m_mpvPlayer->playOrPause();
    });
    connect(m_stopBtn, &QPushButton::clicked, this, [=] {
        m_mpvPlayer->stop();
    });
    connect(m_prevBtn, &QPushButton::clicked, this, [=] {

    });
    connect(m_nextBtn, &QPushButton::clicked, this, [=] {

    });
}

MainWindow::~MainWindow()
{
    if (m_mpvPlayer) {
        m_mpvPlayer->deleteLater();
    }
}

void MainWindow::initPlayer()
{
    m_mpvPlayer = new MpvPlayer(this);
    connect(m_mpvPlayer, &MpvPlayer::musicLoadSuccess, this, &MainWindow::onMusicLoadSuccess);
    m_mpvPlayer->setVolume(5757);
    m_mpvPlayer->setMute(false);
//    m_mpvPlayer->initEngineAndloadMusic("/home/lixiang/970_给我一个吻.mp3");
    m_mpvPlayer->initEngineAndloadMusic("/home/lixiang/王菲陈奕迅-因为爱情.m4a");
//    m_mpvPlayer->initEngineAndloadMusic("/home/lixiang/婚礼的祝福-刘维&薛之谦.mp3");


    //qDebug() << "m_version=" << globalInstance->preferences()->m_version;

//    QString &&suffix = QFileInfo("/home/lixiang/970_给我一个吻.mp3").suffix().toLower();
//    if(suffix=="mp3") {

//    }
}

void MainWindow::initAnalysiserModule()
{
    AnalysiserModule *analysiserModule = globalInstance->analysiserModule();
    analysiserModule->initMusicAnalysiser();

    // test
//    qApp->processEvents();
//
//    QFileInfo fileInfo("/home/lixiang/婚礼的祝福-刘维&薛之谦.mp3");
//    QFileInfo fileInfo("/home/lixiang/970_给我一个吻.mp3");
    QFileInfo fileInfo("/home/lixiang/王菲陈奕迅-因为爱情.m4a");
    if(globalInstance->isRequiredAudioFile(fileInfo.suffix().toLower())) {
        MusicMeta meta;
        analysiserModule->analysisMusicFile(fileInfo, meta);
        m_currentMeta = meta;

        /*qDebug() << "MainWindow Start----------------------";
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
        qDebug() << "MainWindow End----------------------";*/
    }

    this->initLyricModule();
}

void MainWindow::initLyricModule()
{
    LyricModule *lyricModule = globalInstance->lyricModule();
    connect(lyricModule, SIGNAL(albumCoverSaveFinished(QByteArray)), this, SLOT(onAlbumCoverSaveFinished(QByteArray)));
    lyricModule->startLoadLyric(m_currentMeta);
}

void MainWindow::onAlbumCoverSaveFinished(const QByteArray & coverData)
{
    QPixmap coverPixmap = QPixmap::fromImage(QImage::fromData(coverData));
    this->m_ablumLabel->setPixmap(coverPixmap);
    qDebug() << "封面加载成功";
}

void MainWindow::onMusicLoadSuccess()
{
    m_mpvPlayer->playOrPause();
}
