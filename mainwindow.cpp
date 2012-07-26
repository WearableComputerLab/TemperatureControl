// Copyright 2012 Michael Marner (michael@20papercups.net) all rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
// 
//    1. Redistributions of source code must retain the above copyright notice, this list of
//       conditions and the following disclaimer.
// 
//    2. Redistributions in binary form must reproduce the above copyright notice, this list
//       of conditions and the following disclaimer in the documentation and/or other materials
//       provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY <COPYRIGHT HOLDER> ''AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
// FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// 
// The views and conclusions contained in the software and documentation are those of the
// authors and should not be interpreted as representing official policies, either expressed
// or implied, of the authors.


#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
        isConnected(false)
{
    ui->setupUi(this);

    this->statusBar()->addWidget(status);


    ioTimer.setInterval(100);
    ioTimer.setSingleShot(false);

    // timer to io
    QObject::connect(&ioTimer, SIGNAL(timeout()),
                     &tempIO, SLOT(doIO()));

    // connect button to handler
    QObject::connect(ui->connectButton, SIGNAL(clicked()),
                     this, SLOT(connectButtonHandler()));

    QObject::connect(ui->tempSlider, SIGNAL(valueChanged(int)),
                     this, SLOT(handleSlider(int)));

        status = new QLabel("Ready to connect");
    this->statusBar()->addWidget(status);

    QObject::connect(&tempIO, SIGNAL(connecting()),
                     this, SLOT(handleConnecting()));

    QObject::connect(&tempIO, SIGNAL(connected()),
                     this, SLOT(handleConnected()));

    QObject::connect(&tempIO, SIGNAL(disconnected()),
                     this, SLOT(handleDisconnected()));

    QObject::connect(&tempIO, SIGNAL(temperatureChanged(int)),
                     this, SLOT(handleTemperatureChange(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::connectButtonHandler()
{
    if (isConnected) {
        tempIO.disconnect();
    }
    else {
        std::string path = ui->serialDevice->text().toUtf8().constData();
        ui->connectButton->setEnabled(false);
        ui->serialDevice->setEnabled(false);
        tempIO.connect(path);
    }
}

void MainWindow::handleConnected()
{
    this->status->setText("Connected");
    this->statusBar()->clearMessage();
    ui->connectButton->setEnabled(true);
    ui->connectButton->setText("Disconnect");
    ioTimer.start();
    isConnected = true;
}

void MainWindow::handleConnecting()
{
    this->statusBar()->showMessage("Connecting...");
}


void MainWindow::handleTemperatureChange(int newTemperature) {
    ui->tempBar->setValue(newTemperature);
    ui->actualLabel->setText(QString::number(newTemperature) + QString::fromUtf8("°C"));
}


void MainWindow::handleSlider(int value) {
    ui->goalLabel->setText(QString::number(value) + QString::fromUtf8("°C"));
    tempIO.setTemperature(value);
}

void MainWindow::handleDisconnected() {
    ui->serialDevice->setEnabled(true);
    ui->connectButton->setEnabled(true);
    ui->connectButton->setText("Connect");
    isConnected = false;
    ioTimer.stop();
    this->statusBar()->clearMessage();
    this->status->setText("Disconnected");
}
