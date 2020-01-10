/**
* QbitRGBLight implementation
*/
#include "QbitRGBLight.h"

namespace qbit
{

namespace QbitRGBLight
{

extern "C" void sendBufferAsm(const void*, size_t len, DigitalOut *pin);

void sendBuffer(PacketBuffer buf, PinName pin)
{
    DigitalOut hwpin(pin);
    sendBufferAsm(buf.getBytes(), buf.length(), &hwpin);
}

/**
* A LHQbitRGBLight class
*/
void LHQbitRGBLight::setBrightness(int brightness)
{
    ;//TODO:
}

void LHQbitRGBLight::setPin(PinName pin)
{
    ;//TODO:
}

void LHQbitRGBLight::setPixelColor(int pixeloffset, QbitRGBColors rgb, bool flag)
{
    ;//TODO:
}

LHQbitRGBLight *create(PinName pin, int numleds, QbitRGBPixelMode mode)
{
    ;//TODO:
}

}   // qbit::QbitRGBLight

}   // qbit
