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

#ifndef INCLUDED_RobotServerGui_h_GUID_b91bed08_c403_4ced_a3ce_329653f0ea00
#define INCLUDED_RobotServerGui_h_GUID_b91bed08_c403_4ced_a3ce_329653f0ea00

// Internal Includes
// - none

// Library/third-party includes
#include <QMainWindow>
#include <QSharedPointer>

// Standard includes
// - none

namespace Ui {
    class MainWindow;
}

class RobotSettings;

class RobotServerGui : public QMainWindow {
    Q_OBJECT
  public:
    explicit RobotServerGui(RobotSettings &s, QWidget *parent = 0);

    void handleUpdatedSettings();

    void dirtySettings();
    void cleanSettings();

  public
slots:

    void on_action_Open_configuration_triggered();
    void on_action_Configuration_triggered();
    void on_action_Save_configuration_triggered();
    void on_actionSave_configuration_as_triggered();
    void on_action_Quit_triggered();
    void on_action_About_triggered();

  private:
    QSharedPointer<Ui::MainWindow> ui_;
    RobotSettings &s_;
};

#endif // INCLUDED_RobotServerGui_h_GUID_b91bed08_c403_4ced_a3ce_329653f0ea00
