#include "TemperatureIO.h"

TemperatureIO::TemperatureIO()
{
}


TemperatureIO::~TemperatureIO()
{
}

void TemperatureIO::setTemperature(float goalTemperature)
{
    mGoalTemperature = goalTemperature;
}

void TemperatureIO::doIO()
{
    //Do some IO
}


void TemperatureIO::connect(std::string device)
{

    emit connected();
}


void TemperatureIO::disconnect()
{
    emit disconnected();
}
