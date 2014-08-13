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

#include <cstdio>
#include <iostream>
#include <sstream>

#include "TemperatureIO.h"

using namespace wcl;
using namespace std;

TemperatureIO::TemperatureIO()
 : mTemperatureChangePending(false)
{
}


TemperatureIO::~TemperatureIO()
{
}

void TemperatureIO::setTemperature(float goalTemperature)
{
    char result[5];
	if (mGoalTemperature != goalTemperature)
	{
        mGoalTemperature = goalTemperature;
    mTemperatureChangePending = true;
	}
}

void TemperatureIO::doIO()
{
    // get the temperature;
    char result[7];
    float temp = 0;
    mConnection.write("G\n");
    mConnection.drain();
    mConnection.read(result, 6);
    sscanf(result, "%f\n", &temp);
    if (temp != mCurrentTemperature)
    {
        mCurrentTemperature = temp;
        emit temperatureChanged(temp);
    }
    if (mTemperatureChangePending) {
        stringstream ss;
        ss << "S" << mGoalTemperature << "\r\n";
        mConnection.write(ss.str());
    mConnection.drain();
    //mConnection.read(result, 3);
    mTemperatureChangePending = false;
    }
}



void TemperatureIO::connect(std::string device)
{

    emit connecting();
    bool result = mConnection.open(device.c_str(), Serial::BAUD_9600, Serial::DB_EIGHT, Serial::NONE,
                     Serial::ONE, Serial::RAW, Serial::DISABLED, Serial::NONBLOCKING);

    if (result)
        emit connected();
    else
        emit disconnected();
}


void TemperatureIO::disconnect()
{
    mConnection.close(true);
    emit disconnected();
}


