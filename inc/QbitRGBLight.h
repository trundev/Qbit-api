/**
* QbitRGBLight declaration
*/
#ifndef _QBITRGBLIGHT_
#define _QBITRGBLIGHT_

#include "MicroBitPin.h"
#include "PacketBuffer.h"

namespace qbit
{

namespace QbitRGBLight
{

enum QbitRGBColors {
    //% block=red
    Red = 1,
    //% block=orange
    Orange = 2,
    //% block=yellow
    Yellow = 3,
    //% block=green
    Green = 4,
    //% block=blue
    Blue = 5,
    //% block=indigo
    Indigo = 6,
    //% block=violet
    Violet = 7,
    //% block=purple
    Purple = 8,
    //% block=white
    White = 9
};

enum Lights {
    //% block="Light 1"
    Light1 = 0x00,
    //% block="Light 2"
    Light2 = 0x01
};

/**
* Different modes for RGB or RGB+W RGBLight QbitRGBColors
*/
enum QbitRGBPixelMode {
    //% block="RGB (GRB format)"
    RGB = 0,
    //% block="RGB+W"
    RGBW = 1,
    //% block="RGB (RGB format)"
    RGB_RGB = 2
};

/**
* QbitRGBLight Functions
*/

//% shim=sendBufferAsm
//% parts="QbitRGBLight"
void sendBuffer(PacketBuffer buf, MicroBitPin pin);

/**
* A LHQbitRGBLight class
*/
class LHQbitRGBLight
{
public:
    PacketBuffer buf;
    MicroBitPin pin;
    // TODO: encode as bytes instead of 32bit
    int brightness;
    int start; // start offset in LED strip
    int _length; // number of LEDs
    QbitRGBPixelMode _mode;

    void setBrightness(int brightness);

    void setPin(MicroBitPin pin);

    void setPixelColor(int pixeloffset, QbitRGBColors rgb, bool flag);

private:
    void setPixelRGB(int pixeloffset, QbitRGBColors rgb, bool flag);
    void setBufferRGB(int offset, int red, int green, int blue);
};

LHQbitRGBLight create(MicroBitPin pin, int numleds, QbitRGBPixelMode mode);

}   // qbit::QbitRGBLight

}   // qbit

#endif  // _QBITRGBLIGHT_
