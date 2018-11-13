#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
SoftwareSerial BTserial(8, 9);
#define PIXEL 5
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIXEL, NEO_GRB + NEO_KHZ800);
const long baudRate = 9600;
float transmission = 0;
float magFullValue = 0;
float remainingBulletFraction;
float bulletslost;
byte button;
byte oldbutton = 0;
byte buttonpin = 12;
int buttoncounter = 0;
bool state = false;
long time = 0;
long debounce = 400;
bool calibrated = false;
String magState = "";
void setup()
{
    pinMode(buttonpin, INPUT_PULLUP);
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
    strip.begin();
    showEmpty(strip.Color(255, 0, 0));
    strip.show();
}
void showFull(uint32_t c)
{
    strip.setBrightness(32);
    strip.setPixelColor(0, c);
    strip.setPixelColor(1, c);
    strip.setPixelColor(2, c);
    strip.setPixelColor(3, c);
    strip.setPixelColor(4, c);
    strip.setPixelColor(5, c);
    strip.setPixelColor(6, c);
    strip.setPixelColor(7, c);
    strip.show();
}

void showHigh(uint32_t c)
{
    strip.setBrightness(32);
    strip.setPixelColor(0, c);
    strip.setPixelColor(1, c);
    strip.setPixelColor(2, c);
    strip.setPixelColor(3, c);
    strip.setPixelColor(4, c);
    strip.setPixelColor(5, c);
    strip.setPixelColor(6, strip.Color(0, 0, 0));
    strip.setPixelColor(7, strip.Color(0, 0, 0));
    strip.show();
}

void showMedium(uint32_t c)
{
    strip.setBrightness(32);
    strip.setPixelColor(0, c);
    strip.setPixelColor(1, c);
    strip.setPixelColor(2, c);
    strip.setPixelColor(3, c);
    strip.setPixelColor(4, c);
    strip.setPixelColor(5, strip.Color(0, 0, 0));
    strip.setPixelColor(6, strip.Color(0, 0, 0));
    strip.setPixelColor(7, strip.Color(0, 0, 0));
    strip.show();
}

void showLow(uint32_t c)
{
    strip.setBrightness(32);
    strip.setPixelColor(0, c);
    strip.setPixelColor(1, c);
    strip.setPixelColor(2, c);
    strip.setPixelColor(3, strip.Color(0, 0, 0));
    strip.setPixelColor(4, strip.Color(0, 0, 0));
    strip.setPixelColor(5, strip.Color(0, 0, 0));
    strip.setPixelColor(6, strip.Color(0, 0, 0));
    strip.setPixelColor(7, strip.Color(0, 0, 0));
    strip.show();
}
void showEmpty(uint32_t c)
{
    strip.setBrightness(32);
    strip.setPixelColor(0, c);
    strip.setPixelColor(1, strip.Color(0, 0, 0));
    strip.setPixelColor(2, strip.Color(0, 0, 0));
    strip.setPixelColor(3, strip.Color(0, 0, 0));
    strip.setPixelColor(4, strip.Color(0, 0, 0));
    strip.setPixelColor(5, strip.Color(0, 0, 0));
    strip.setPixelColor(6, strip.Color(0, 0, 0));
    strip.setPixelColor(7, strip.Color(0, 0, 0));
    strip.show();
}

void showNothing(uint32_t c)
{
    strip.setBrightness(32);
    strip.setPixelColor(0, strip.Color(0, 0, 0));
    strip.setPixelColor(1, strip.Color(0, 0, 0));
    strip.setPixelColor(2, strip.Color(0, 0, 0));
    strip.setPixelColor(3, strip.Color(0, 0, 0));
    strip.setPixelColor(4, strip.Color(0, 0, 0));
    strip.setPixelColor(5, strip.Color(0, 0, 0));
    strip.setPixelColor(6, strip.Color(0, 0, 0));
    strip.setPixelColor(7, strip.Color(0, 0, 0));
    strip.show();
}
void loop()
{
    //Serial.println(state);
    button = digitalRead(buttonpin);
    while (BTserial.available())
    {
        delay(100);
        if (buttoncounter < 1)
        {
            showEmpty(strip.Color(0, 0, 255));
        }
        transmission = BTserial.read();
        if (button && !oldbutton && millis() - time > debounce)
        {
            buttoncounter += 1;
            if (state == false && buttoncounter > 1)
            {
                showFull(strip.Color(0, 255, 0));
                state = true;
                magState = "full";
                magFullValue = transmission;
            }
            else if (state == true && buttoncounter > 1)
            {
                magState = "empty";
                state = false;
                showEmpty(strip.Color(255, 0, 0));
                magFullValue = 0;
            }
            time = millis();
            oldbutton = 1;
        }
        else if (!button && oldbutton)
        {
            oldbutton = 0;
        }
        if (magState = "full" && buttoncounter > 0 && buttoncounter > 0)
        {
            remainingBulletFraction = (magFullValue / transmission);
            bulletslost = ((1 - remainingBulletFraction) * 45);
            Serial.print("bulletsLost: ");
            Serial.println(bulletslost);

            if (bulletslost < 20)
            {
                showFull(strip.Color(0, 255, 0));
            }
            else if (bulletslost < 26)
            {
                showHigh(strip.Color(0, 255, 0));
            }
            else if (bulletslost < 27.5)
            {
                showMedium(strip.Color(255, 255, 0));
            }
            else if (bulletslost < 28.5)
            {
                showLow(strip.Color(255, 255, 0));
            }
            else if (bulletslost < 29.5)
            {
                showEmpty(strip.Color(155, 155, 0));
            }
            else if (bulletslost < 30.5)
            {
                showEmpty(strip.Color(255, 0, 0));
            }
        }
    }
}
