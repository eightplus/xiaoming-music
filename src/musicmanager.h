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

#ifndef MUSIC_MANAGER_H
#define MUSIC_MANAGER_H

#include <QObject>

class Global;
class MainWindow;

class MusicManager : public QObject
{
    Q_OBJECT
public:

    explicit MusicManager(QObject *parent = 0);
    ~MusicManager();

    MainWindow *mainWindow() const;
    void setMainWindow(MainWindow *mainWindow);

    void run();

private:
    MainWindow *m_mainWindow = nullptr;
};

#endif // MUSIC_MANAGER_H
