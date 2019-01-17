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
class AnalysiserModule;

#define globalInstance (Global::instance())

#define KILOBYTE_FACTOR 1000.0
#define MEGABYTE_FACTOR (1000.0 * 1000.0)
#define GIGABYTE_FACTOR (1000.0 * 1000.0 * 1000.0)
#define TERABYTE_FACTOR (1000.0 * 1000.0 * 1000.0 * 1000.0)

#define KIBIBYTE_FACTOR 1024.0
#define MEBIBYTE_FACTOR (1024.0 * 1024.0)
#define GIBIBYTE_FACTOR (1024.0 * 1024.0 * 1024.0)
#define TEBIBYTE_FACTOR (1024.0 * 1024.0 * 1024.0 * 10242.0)

class MainWindow;

class Global : public QObject
{
    Q_OBJECT

public:
    enum SizeFactor
    {
        Byte,
        KiloByte,
        MegaByte,
        GigaByte,
        TeraByte,
        SizeInvalid
    };

    explicit Global(QObject *parent = 0);
    ~Global();

    static Global *instance();
    static void initial(QObject *parent = 0);

    QWidget *mainWindow() const;
    void setMainWindow(MainWindow *mainWindow);

    Preferences *preferences() const { return m_preferences; }
    AnalysiserModule *analysiserModule() { return m_analysiserModule; }

    void initSizeFactor();
    void initMusicFileType();
    bool isRequiredAudioFile(const QString &suffix) const { return m_audioSuffixs.contains(suffix); }
    QString fileTypeDescription(const QString &suffix) const;
    QString byteToString(qreal size);

private:
    static Global *m_instance;
    MainWindow *m_mainWindow = nullptr;
    Preferences *m_preferences = nullptr;
    AnalysiserModule *m_analysiserModule = nullptr;
    QThread *m_analysisThread = nullptr;
    QString m_sizeFactor[SizeInvalid];
    QStringList m_audioSuffixs;
    QStringList m_audioiSuffixsDescription;
};

#endif // _GLOBAL_H
