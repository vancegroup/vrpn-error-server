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
#include "Configuration.h"
#include "RobotSettings.h"

// Library/third-party includes
#include <QApplication>

// Standard includes
// - none


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName(
        "Iowa State University Virtual Reality Applications Center");
    QCoreApplication::setOrganizationDomain("vrac.iastate.edu");
    QCoreApplication::setApplicationName("Vance Mobile Robot Server");

    RobotSettings s;
    QStringList args = QCoreApplication::arguments();
    if (args.size() > 1) {
        s.load(args.at(1));
    } else {
        s.setFilename("config.dat");
    }

    Configuration config(s);
    config.show();
    return app.exec();
}
