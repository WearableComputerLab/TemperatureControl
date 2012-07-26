#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ioTimer.setInterval(100);
    ioTimer.setSingleShot(false);

    // timer to io
    QObject::connect(&ioTimer, SIGNAL(timeout()),
                     &tempIO, SLOT(doIO()));

    // connect button to handler
    QObject::connect(connectButton, SIGNAL(clicked()),
                     this, SLOT(connectButtonHandler()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::connectButtonHandler()
{

}
