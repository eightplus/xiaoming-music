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

#include <QPushButton>
#include <QHBoxLayout>
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
    m_layout->addWidget(m_playBtn);
    m_layout->addWidget(m_stopBtn);
    m_layout->addWidget(m_prevBtn);
    m_layout->addWidget(m_nextBtn);
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
    m_mpvPlayer->initEngineAndloadMusic("/home/lixiang/970_给我一个吻.mp3");

    //qDebug() << "m_version=" << globalInstance->preferences()->m_version;
}


void MainWindow::onMusicLoadSuccess()
{
    m_mpvPlayer->playOrPause();
}
