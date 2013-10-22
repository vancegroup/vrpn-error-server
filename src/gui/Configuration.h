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

#ifndef INCLUDED_Configuration_h_GUID_ea2280fe_7682_4de6_bee8_ec471468dd10
#define INCLUDED_Configuration_h_GUID_ea2280fe_7682_4de6_bee8_ec471468dd10

// Internal Includes
// - none

// Library/third-party includes
#include <QDialog>
#include <QSharedPointer>

// Standard includes
// - none

namespace Ui {
    class Configuration;
}

class Configuration : public QDialog {
    Q_OBJECT
  public:
    explicit Configuration(QWidget *parent = 0);
  private
slots:
    void dialogAccepted();

  private:
    QSharedPointer<Ui::Configuration> ui_;
};

#endif // INCLUDED_Configuration_h_GUID_ea2280fe_7682_4de6_bee8_ec471468dd10
