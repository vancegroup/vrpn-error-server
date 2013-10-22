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

// Standard includes
// - none

class RobotSettings {
  public:
    RobotSettings();
    QString deviceBaseName;
    qint16 vrpnPort;

    QString serialPort;
    qint32 baud;

    qint16 messageInterval;
    qint32 gain;
    bool receiveStatus;

    void save() const;
    void save(QString const &fn);
    bool load();
    bool load(QString const &fn);
    void setFilename(QString const &fn);

  private:
    QString fn_;
};

#endif // INCLUDED_RobotSettings_h_GUID_944e9150_d1ab_4d0d_9329_52a5c8911e0a
