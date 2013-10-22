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

RobotSettings::RobotSettings()
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
    , receiveStatus(true) {
}

// Use preprocessor here to be sure that fields are saved and loaded in the same
// order.
#define STREAMIT(OP)                                                           \
    OP s.deviceBaseName OP s.vrpnPort OP s.serialPort OP s.baud OP             \
    s.messageInterval OP s.gain OP s.receiveStatus

QDataStream &operator<<(QDataStream &stream, RobotSettings const &s) {
    stream STREAMIT(<< );
    return stream;
}

QDataStream &operator>>(QDataStream &stream, RobotSettings &s) {
    stream STREAMIT(>> );
    return stream;
}
#undef STREAMIT

void RobotSettings::save(const QString &fn) {
    setFilename(fn);
    save();
}

void RobotSettings::save() const {
    QFile file(fn_);
    if (!file.open(QFile::WriteOnly)) {
        std::cerr << "Error: Cannot write file " << qPrintable(fn_) << ": "
                  << qPrintable(file.errorString()) << std::endl;
        return;
    }
    QDataStream stream(&file);
    stream << *this;
}

bool RobotSettings::load(QString const &fn) {
    setFilename(fn);
    return load();
}
bool RobotSettings::load() {
    QFile file(fn_);
    if (!file.open(QFile::ReadOnly)) {
        std::cerr << "Error: Cannot read file " << qPrintable(fn_) << ": "
                  << qPrintable(file.errorString()) << std::endl;
        return false;
    }
    QDataStream stream(&file);
    stream >> *this;
    return true;
}

void RobotSettings::setFilename(QString const &fn) {
    std::cout << "Selecting settings file: " << qPrintable(fn) << std::endl;
    fn_ = fn;
}
