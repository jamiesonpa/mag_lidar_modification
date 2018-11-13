#include <Wire.h>
#include "Adafruit_VL6180X.h"
#include <SoftwareSerial.h>
SoftwareSerial BTserial(8, 9); // RX | TX

Adafruit_VL6180X vl = Adafruit_VL6180X();
const int numReadings = 30;
float readings[numReadings];
int readIndex = 0; // the index of the current reading
float total = 0;   // the running total
float average = 0; // the average
int incomingByte = 0;
const long baudRate = 38400;
char c = ' ';
boolean NL = true;

void setup()
{
    Serial.begin(9600);
    Serial.print("Sketch:   ");
    Serial.println(__FILE__);
    Serial.print("Uploaded: ");
    Serial.println(__DATE__);
    Serial.println(" ");
    BTserial.begin(9600);
    Serial.print("BTserial started at ");
    Serial.println(baudRate);
    Serial.println(" ");
    for (int thisReading = 0; thisReading < numReadings; thisReading++)
    {
        readings[thisReading] = 0;
        while (!Serial)
        {
            delay(1);
        }
        Serial.println("Adafruit VL6180x test!");
        if (!vl.begin())
        {
            Serial.println("Failed to find sensor");
            while (1)
                ;
        }
        Serial.println("Sensor found!");
    }
}

void loop()
{

    float lux = vl.readLux(VL6180X_ALS_GAIN_40);

    //Serial.print("Lux: "); Serial.println(lux);
    uint8_t status = vl.readRangeStatus();
    uint8_t range = vl.readRange();

    readIndex += 1;
    readings[readIndex] = range;
    if (readIndex >= numReadings)
    {
        readIndex = 0;
    }
    average = 0;
    for (int i = 0; i < numReadings; ++i)
    {
        average += readings[i];
    }
    average /= numReadings;

    if (status == VL6180X_ERROR_NONE)
    {
        Serial.print("sent: ");
        Serial.println(average);
        BTserial.write(average);
        delay(10);
    }

    // Some error occurred, print it out!
    if ((status >= VL6180X_ERROR_SYSERR_1) && (status <= VL6180X_ERROR_SYSERR_5))
    {
        Serial.println("System error");
    }
    else if (status == VL6180X_ERROR_ECEFAIL)
    {
        Serial.println("ECE failure");
    }
    else if (status == VL6180X_ERROR_NOCONVERGE)
    {
        Serial.println("No convergence");
    }
    else if (status == VL6180X_ERROR_RANGEIGNORE)
    {
        Serial.println("Ignoring range");
    }
    else if (status == VL6180X_ERROR_SNR)
    {
        Serial.println("Signal/Noise error");
    }
    else if (status == VL6180X_ERROR_RAWUFLOW)
    {
        Serial.println("Raw reading underflow");
    }
    else if (status == VL6180X_ERROR_RAWOFLOW)
    {
        Serial.println("Raw reading overflow");
    }
    else if (status == VL6180X_ERROR_RANGEUFLOW)
    {
        Serial.println("Range reading underflow");
    }
    else if (status == VL6180X_ERROR_RANGEOFLOW)
    {
        Serial.println("Range reading overflow");
    }
}