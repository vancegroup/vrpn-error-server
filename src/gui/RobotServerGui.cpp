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
#include "ui_Configuration.h"

// Library/third-party includes
// - none

// Standard includes
// - none

int main(int argc, char * argv[]) {
    QApplication app(argc, argv);
    Ui::Configuration config;
    QDialog dialog;
    config.setupUi(&dialog);
    dialog.show();
    return app.exec();
}
