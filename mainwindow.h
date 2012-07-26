#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "TemperatureIO.h"

#include <QMainWindow>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    QTimer ioTimer;
    TemperatureIO tempIO;

public slots:
    void connectButtonHandler();
};

#endif // MAINWINDOW_H
