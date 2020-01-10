/**
* QbitRGBLight implementation
*/
#include "QbitRGBLight.h"

namespace qbit
{

namespace QbitRGBLight
{

//% shim=sendBufferAsm
//% parts="QbitRGBLight"
void sendBuffer(PacketBuffer buf, MicroBitPin pin)
{
}

/**
* A LHQbitRGBLight class
*/
void LHQbitRGBLight::setBrightness(int brightness)
{
    ;//TODO:
}

void LHQbitRGBLight::setPin(MicroBitPin pin)
{
    ;//TODO:
}

void LHQbitRGBLight::setPixelColor(int pixeloffset, QbitRGBColors rgb, bool flag)
{
    ;//TODO:
}

LHQbitRGBLight create(MicroBitPin pin, int numleds, QbitRGBPixelMode mode)
{
    ;//TODO:
}

}   // qbit::QbitRGBLight

}   // qbit
