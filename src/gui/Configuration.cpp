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
#include "ui_Configuration.h"
#include "RobotSettings.h"

// Library/third-party includes
// - none

// Standard includes
// - none

Configuration::Configuration(QWidget *parent)
    : QDialog(parent)
    , ui_(new Ui::Configuration) {
    ui_->setupUi(this);
    Ui::Configuration *ui = ui_.data();

    RobotSettings s;
    ui->deviceBaseName->setText(s.deviceBaseName());
    ui->vrpnPort->setValue(s.vrpnPort());

    ui->serialPort->setText(s.serialPort());
    ui->baud->setCurrentIndex(ui->baud->findText(QString::number(s.baud())));

    ui->interval->setValue(s.messageInterval());
    ui->gain->setValue(s.gain());
    ui->receive->setChecked(s.receiveStatus());

    connect(this, SIGNAL(accepted()), this, SLOT(dialogAccepted()));
}

void Configuration::dialogAccepted() {

    Ui::Configuration *ui = ui_.data();

    RobotSettings s;
    s.setDeviceBaseName(ui->deviceBaseName->text());
    s.setVrpnPort(ui->vrpnPort->value());

    s.setSerialPort(ui->serialPort->text());
    s.setBaud(ui->baud->currentText().toInt());

    s.setMessageInterval(ui->interval->value());
    s.setGain(ui->gain->value());
    s.setReceiveStatus(ui->receive->isChecked());
}
