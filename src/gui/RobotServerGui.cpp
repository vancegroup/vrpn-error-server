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
#include "RobotServerGui.h"
#include "Configuration.h"
#include "RobotSettings.h"
#include "ui_mainwindow.h"

// Library/third-party includes
#include <QFileDialog>
#include <QMessageBox>

// Standard includes
// - none

RobotServerGui::RobotServerGui(RobotSettings &s, QWidget *parent)
    : QMainWindow(parent)
    , ui_(new Ui::MainWindow)
    , s_(s) {
    ui_->setupUi(this);
}

void RobotServerGui::handleUpdatedSettings() {}

void RobotServerGui::dirtySettings() {
    ui_->action_Save_configuration->setEnabled(true);
}

void RobotServerGui::cleanSettings() {
    ui_->action_Save_configuration->setEnabled(false);
}

void RobotServerGui::on_action_Open_configuration_triggered() {
    QString fn = QFileDialog::getOpenFileName(this);
    if (!fn.isEmpty()) {
        bool success = s_.load(fn);
        if (success) {
            cleanSettings();
            handleUpdatedSettings();
        }
    }
}

void RobotServerGui::on_action_Configuration_triggered() {
    Configuration c(s_, this);
    int result = c.exec();
    if (result == QDialog::Accepted) {
        dirtySettings();
        handleUpdatedSettings();
    }
}

void RobotServerGui::on_action_Save_configuration_triggered() {
    s_.save();
    cleanSettings();
}

void RobotServerGui::on_actionSave_configuration_as_triggered() {
    QString newfn = QFileDialog::getSaveFileName(this);
    if (!newfn.isEmpty()) {
        s_.setFilename(newfn);
        s_.save();
        cleanSettings();
    }
}

void RobotServerGui::on_action_Quit_triggered() {
    QApplication::closeAllWindows();
}

extern const char *vrpn_MAGIC;
void RobotServerGui::on_action_About_triggered() {
    QMessageBox::about(
        this, "Mobile Robot Server",
        QString(
            "Vance Group Mobile Robot Server GUI\nBuilt using %1.")
            .arg(vrpn_MAGIC));
}
