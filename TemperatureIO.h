#ifndef TEMPERATUREIO_H
#define TEMPERATUREIO_H

#include <wcl/rawports/Serial.h>
#include <string>
#include <QObject>

class TemperatureIO : public QObject
{
    Q_OBJECT

public:
    TemperatureIO();
    ~TemperatureIO();

public slots:
    void setTemperature(float goalTemperature);
    void connect(std::string device);
    void disconnect();
    void doIO();

signals:
    void temperatureChanged(float newTemperature);
    void connected();
    void disconnected();


private:
    float mGoalTemperature;
    float mCurrentTemperature;

    wcl::Serial mConnection;

};

#endif // TEMPERATUREIO_H
