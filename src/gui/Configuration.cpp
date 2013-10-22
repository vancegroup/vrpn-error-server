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

Configuration::Configuration(const QString & fn, Settings &s, QWidget *parent)
    : QDialog(parent)
    , ui_(new Ui::Configuration)
    , s_(s)
    , fn_(fn) {
    ui_->setupUi(this);
    Ui::Configuration *ui = ui_.data();

    ui->deviceBaseName->setText(s.deviceBaseName);
    ui->vrpnPort->setValue(s.vrpnPort);

    ui->serialPort->setText(s.serialPort);
    ui->baud->setCurrentIndex(ui->baud->findText(QString::number(s.baud)));

    ui->interval->setValue(s.messageInterval);
    ui->gain->setValue(s.gain);
    ui->receive->setChecked(s.receiveStatus);

    connect(this, SIGNAL(accepted()), this, SLOT(dialogAccepted()));
}

void Configuration::dialogAccepted() {

    Ui::Configuration *ui = ui_.data();
    Settings & s = s_;
    s.deviceBaseName = ui->deviceBaseName->text();
    s.vrpnPort = ui->vrpnPort->value();

    s.serialPort = ui->serialPort->text();
    s.baud = ui->baud->currentText().toInt();

    s.messageInterval = ui->interval->value();
    s.gain = ui->gain->value();
    s.receiveStatus = ui->receive->isChecked();

    s.save(fn_);
}
