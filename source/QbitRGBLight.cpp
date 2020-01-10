/**
* QbitRGBLight implementation
*/
#include "QbitRGBLight.h"

namespace qbit
{

namespace QbitRGBLight
{

extern "C" void sendBufferAsm(const void*, size_t len, DigitalOut *pin);

static uint32_t packRGB(uint32_t a, uint32_t b, uint32_t c)
{
    return ((a & 0xFF) << 16) | ((b & 0xFF) << 8) | (c & 0xFF);
}

static uint32_t unpackR(uint32_t rgb)
{
    uint32_t r = (rgb >> 16) & 0xFF;
    return r;
}

static uint32_t unpackG(uint32_t rgb)
{
    uint32_t g = (rgb >> 8) & 0xFF;
    return g;
}

static uint32_t unpackB(uint32_t rgb)
{
    uint32_t b = (rgb) & 0xFF;
    return b;
}


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
    this->brightness = brightness & 0xff;
}

void LHQbitRGBLight::setPin(PinName pin)
{
    this->pin = pin;
}

void LHQbitRGBLight::setPixelColor(int pixeloffset, QbitRGBColors rgb, bool flag)
{
    setPixelRGB(pixeloffset, rgb, flag);
}

void LHQbitRGBLight::show()
{
    sendBuffer(this->buf, this->pin);
}

void LHQbitRGBLight::clear()
{
    const int stride = this->_mode == RGBW ? 4 : 3;
    // Zero fill
    for (int i = 0; i < this->_length * stride; i++)
        this->buf.setByte(this->start * stride + i, 0);
    this->show();
}

void LHQbitRGBLight::setPixelRGB(int pixeloffset, QbitRGBColors rgb, bool flag)
{
    if (pixeloffset < 0
        || pixeloffset >= this->_length)
        return;
    uint32_t tureRgb = 0;

    if ((pixeloffset == 2 || pixeloffset == 3)&&!flag)
    {
        switch (rgb)
        {
            case Red:
                tureRgb = 0x00FF00;
                break;

            case Orange:
                tureRgb = 0xF6BA41;
                break;

            case Yellow:
                tureRgb = 0xFFFF00;
                break;

            case Green:
                tureRgb = 0xFF0000;
                break;

            case Blue:
                tureRgb = 0x0000FF;
                break;

            case Indigo:
                tureRgb = 0x004b82;
                break;

            case Violet:
                tureRgb = 0x2B8AE2;
                break;

            case Purple:
                tureRgb = 0x00FFFF;
                break;

            case White:
                tureRgb = 0xFFFFFF;
                break;
        }
    }
    else
    {
        switch (rgb)
        {
            case Red:
                tureRgb = 0xFF0000;
                break;

            case Orange:
                tureRgb = 0xFFA500;
                break;

            case Yellow:
                tureRgb = 0xFFFF00;
                break;

            case Green:
                tureRgb = 0x00FF00;
                break;

            case Blue:
                tureRgb = 0x0000FF;
                break;

            case Indigo:
                tureRgb = 0x4b0082;
                break;

            case Violet:
                tureRgb = 0x8a2be2;
                break;

            case Purple:
                tureRgb = 0xFF00FF;
                break;

            case White:
                tureRgb = 0xFFFFFF;
                break;
        }
    }

    int stride = this->_mode == RGBW ? 4 : 3;
    pixeloffset = (pixeloffset + this->start) * stride;

    uint32_t red = unpackR(tureRgb);
    uint32_t green = unpackG(tureRgb);
    uint32_t blue = unpackB(tureRgb);

    int br = this->brightness;
    if (br < 255) {
        red = (red * br) >> 8;
        green = (green * br) >> 8;
        blue = (blue * br) >> 8;
    }
    this->setBufferRGB(pixeloffset, red, green, blue);
}

void LHQbitRGBLight::setBufferRGB(int offset, uint32_t red, uint32_t green, uint32_t blue)
{
    if (this->_mode == RGB_RGB)
    {
        this->buf[offset + 0] = red;
        this->buf[offset + 1] = green;
    }
    else
    {
        this->buf[offset + 0] = green;
        this->buf[offset + 1] = red;
    }
    this->buf[offset + 2] = blue;
}

LHQbitRGBLight *create(PinName pin, int numleds, QbitRGBPixelMode mode)
{
    LHQbitRGBLight *light = new LHQbitRGBLight();
    int stride = mode == RGBW ? 4 : 3;
    light->buf = PacketBuffer(numleds * stride);
    light->start = 0;
    light->_length = numleds;
    light->_mode = mode;
    light->setBrightness(255);
    light->setPin(pin);
    return light;
}

}   // qbit::QbitRGBLight

}   // qbit
