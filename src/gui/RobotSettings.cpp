/**
        @file
        @brief Implementation

        @date 2013

        @author
        Ryan Pavlik
        <rpavlik@iastate.edu> and <abiryan@ryand.net>
        http://academic.cleardefinition.com/
        Iowa State University Virtual Reality Applications Center
        Human-Computer Interaction Graduate Program
*/

//          Copyright Iowa State University 2013.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// Internal Includes
#include "RobotSettings.h"

// Library/third-party includes
#include <QCoreApplication>
#include <QStringList>
#include <QDebug>
#include <QDataStream>
#include <QFile>
#include <vrpn_Configure.h>

// Standard includes
#include <iostream>

Settings::Settings()
    : deviceBaseName("Robot")
    , vrpnPort(vrpn_DEFAULT_LISTEN_PORT_NO)
    #ifdef _WIN32
    , serialPort("COM4")
    #else
    , serialPort("/dev/ttyUSBS0")
    #endif
    , baud(115200)
    , messageInterval(0)
    , gain(25000)
    , receiveStatus(true)
{}

QDataStream & operator <<(QDataStream & stream, Settings const & s) {
    stream << s.deviceBaseName << s.vrpnPort << s.serialPort << s.baud << s.messageInterval << s.gain << s.receiveStatus;
    return stream;
}

QDataStream & operator >>(QDataStream & stream, Settings & s) {
    stream >> s.deviceBaseName >> s.vrpnPort >> s.serialPort >> s.baud >> s.messageInterval >> s.gain >> s.receiveStatus;
    return stream;
}

void Settings::save(QString const& fn) const {
    QFile file(fn);
    if (!file.open(QFile::WriteOnly)) {
        std::cerr << "Error: Cannot write file "
                  << qPrintable(fn) << ": "
                  << qPrintable(file.errorString()) << std::endl;
        return;
    }
    QDataStream stream(&file);
    stream << *this;
}


void Settings::load(QString const& fn) {

    QFile file(fn);
    if (!file.open(QFile::ReadOnly)) {
        std::cerr << "Error: Cannot read file "
                  << qPrintable(fn) << ": "
                  << qPrintable(file.errorString()) << std::endl;
        return;
    }
    QDataStream stream(&file);
    stream >> *this;
}

RobotSettings::RobotSettings() {}
RobotSettings::~RobotSettings() { settings_.sync(); }

void RobotSettings::checkCommandLineArgs() {
    QStringList args = QCoreApplication::arguments();
    if (args.size() > 1) {
        setFilename(args.at(1));
    } else {
        std::cout << "No settings file passed." << std::endl;
    }
}

void RobotSettings::setFilename(QString const &fn) {
    std::cout << "Selecting settings file: " << fn.toStdString() << std::endl;
    QSettings::setDefaultFormat(QSettings::IniFormat);
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, fn);
    QSettings settings;
    settings.sync();
}

#define SETTINGS(GETTER, SETTER, TYPE, ARGTYPESUFFIX, DEFAULT)                 \
    static const char GETTER##_key[] = #GETTER;                                \
    TYPE RobotSettings::GETTER() const {                                       \
        return qvariant_cast<TYPE>(settings_.value(GETTER##_key, DEFAULT));    \
    }                                                                          \
    void RobotSettings::SETTER(TYPE ARGTYPESUFFIX val) {                       \
        settings_.setValue(GETTER##_key, val);                                 \
    }

SETTINGS(deviceBaseName, setDeviceBaseName, QString, const &, "Robot")
SETTINGS(vrpnPort, setVrpnPort, int, , vrpn_DEFAULT_LISTEN_PORT_NO)

#ifdef _WIN32
SETTINGS(serialPort, setSerialPort, QString, const &, "COM4")
#else
SETTINGS(serialPort, setSerialPort, QString, const &, "/dev/ttyUSBS0")
#endif

SETTINGS(baud, setBaud, int, , 115200)

SETTINGS(messageInterval, setMessageInterval, int, , 0)
SETTINGS(gain, setGain, int, , 25000)
SETTINGS(receiveStatus, setReceiveStatus, bool, , true)


