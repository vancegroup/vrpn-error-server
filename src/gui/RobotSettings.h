/** @file
        @brief Header

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

#ifndef INCLUDED_RobotSettings_h_GUID_944e9150_d1ab_4d0d_9329_52a5c8911e0a
#define INCLUDED_RobotSettings_h_GUID_944e9150_d1ab_4d0d_9329_52a5c8911e0a

// Internal Includes
// - none

// Library/third-party includes
#include <QString>
#include <QSettings>

// Standard includes
// - none

struct Settings {
    Settings();
    QString deviceBaseName;
    qint16 vrpnPort;

    QString serialPort;
    qint16 baud;

    qint16 messageInterval;
    qint16 gain;
    bool receiveStatus;

    void save(QString const& fn) const;
    void load(QString const& fn);

    void saveXML(QString const& fn) const;
    void loadXML(QString const& fn);
};

class RobotSettings {
  public:
    RobotSettings();
    ~RobotSettings();

    QString deviceBaseName() const;
    void setDeviceBaseName(QString const &name);

    int vrpnPort() const;
    void setVrpnPort(int port);

    QString serialPort() const;
    void setSerialPort(QString const &port);

    int baud() const;
    void setBaud(int baud);

    int messageInterval() const;
    void setMessageInterval(int interval);

    int gain() const;
    void setGain(int kp);

    bool receiveStatus() const;
    void setReceiveStatus(bool receive);

    static void setFilename(QString const &fn);
    static void checkCommandLineArgs();

  private:
    QSettings settings_;
};
#endif // INCLUDED_RobotSettings_h_GUID_944e9150_d1ab_4d0d_9329_52a5c8911e0a
