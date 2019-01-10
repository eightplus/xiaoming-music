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

#ifndef _GLOBAL_H
#define _GLOBAL_H

#include <QObject>
#include <QString>

class Preferences;

#define globalInstance (Global::instance())

class MainWindow;

class Global : public QObject
{
    Q_OBJECT

public:
    explicit Global(QObject *parent = 0);
    ~Global();

    static Global *instance();
    static void initial(QObject *parent = 0);

    QWidget *mainWindow() const;
    void setMainWindow(MainWindow *mainWindow);

    Preferences *preferences() const;

private:
    static Global *m_instance;
    MainWindow *m_mainWindow = nullptr;
    Preferences *m_preferences = nullptr;
};

#endif // _GLOBAL_H
