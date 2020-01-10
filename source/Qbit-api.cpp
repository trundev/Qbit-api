/*
* C++ interface to control Qbit implementation
*
* Based on: https://github.com/LOBOT-ROBOT/Qbit
*/
#include "MicroBitPin.h"
#include "Qbit-api.h"

namespace qbit
{

static QbitRGBLight::LHQbitRGBLight *lhRGBLight = NULL;
static bool obstacleSensor1 = false;
static bool obstacleSensor2 = false;
static int currentVoltage = 0;
static bool versionFlag = false; 
static int readTimes = 0;

static void initRGBLight();


/**
* Qbit initialization, please execute at boot time
*/
//% weight=100 blockId=qbitInit block="Initialize Qbit"
void qbitInit()
{
    initRGBLight();
    ;//TODO:
}

/**
* Set Qbit run carrier mode
*/
//% weight=99 blockId=qbit_carrier block="Set Qbit carrier mode"
void qbit_carrier()
{
    ;//TODO:
}

/**
* Set the speed of the motor, range of -100~100, that can control the Qbit running.
*/
//% weight=98 blockId=setQbitRunSpeed block="Set Qbit(V2.0 version or newer) run|speed %speed|and oriention %oriention"
//% speed.min=0 speed.max=100
void setQbitRunSpeed(int speed, OrientionType oriention)
{
    ;//TODO:
}

/**
* Set the center balance angle of the Qbit
*/
//% weight=96 blockId=setBLAngle block="Set the center balance angle of the Qbit"
void setBLAngle()
{
    ;//TODO:
}

/**
* Do someting when Qbit receive remote-control code
* @param code the ir key button that needs to be pressed
* @param body code to run when event is raised
*/
//% weight=95 blockId=onQbit_remote_ir_pressed block="on remote-control|%code|pressed"
void onQbit_remote_ir_pressed(IRKEY code, void (*body)(MicroBitEvent))
{
    ;//TODO:
}

/**
* Do someting when remote-control stop send
* @param body code to run when event is raised
*/
//% weight=94 blockId=onQbit_remote_no_ir block="on remote-control stop send"
void onQbit_remote_no_ir(void (*body)(MicroBitEvent))
{
    ;//TODO:
}

/**
* Obtain the distance of ultrasonic detection to the obstacle
*/
//% weight=93 blockId=Ultrasonic block="Ultrasonic distance(cm)"
int Ultrasonic()
{
    ;//TODO:
}

/**
* Stop Qbit run.
*/
//% weight=90 blockId=setQbitRun block="Set Qbit balance %runType"
void setQbitRun(QbitRunType runType)
{
    ;//TODO:
}

/**
* Detect whether avoid obstacle sensor detect obstacle
*/
//% weight=86 blockGap=50  blockId=obstacleSensor block="avoid obstacle|%sensor|detect obstacle"
bool obstacleSensor(ObstacleSensor sensor)
{
    ;//TODO:
}

/**
* Initialize RGB
*/
static void initRGBLight()
{
    if (NULL == lhRGBLight)
    {
        lhRGBLight = QbitRGBLight::create(MICROBIT_PIN_P15, 2, QbitRGBLight::RGB);
    }
    clearLight();
}

/**
* Set the brightness of the strip. This flag only applies to future operation.
* @param brightness a measure of LED brightness in 0-255. eg: 255
*/
//% blockId="setBrightness" block="set light brightness %brightness"
//% weight=84
void setBrightness(int brightness)
{
    if (lhRGBLight)
        lhRGBLight->setBrightness(brightness);
}

/**
* Set the color of the colored lights, after finished the setting please perform  the display of colored lights.
*/
//% weight=82 blockId=setPixelRGB block="Set|%lightoffset|color to %rgb"
void setPixelRGB(QbitRGBLight::Lights lightoffset, QbitRGBLight::QbitRGBColors rgb)
{
    if (lhRGBLight)
        lhRGBLight->setPixelColor(lightoffset, rgb, versionFlag);
}

/**
* Set RGB Color argument
*/
//% weight=80 blockId=setPixelRGBArgs block="Set|%lightoffset|color to %rgb"
void setPixelRGBArgs(QbitRGBLight::Lights lightoffset, int rgb)
{
    if (lhRGBLight)
        lhRGBLight->setPixelColor(lightoffset, (QbitRGBLight::QbitRGBColors)rgb, versionFlag);
}

/**
* Display the colored lights, and set the color of the colored lights to match the use. After setting the color of the colored lights, the color of the lights must be displayed.
*/
//% weight=78 blockId=showLight block="Show light"
void showLight()
{
    if (lhRGBLight)
        lhRGBLight->show();
}

/**
* Clear the color of the colored lights and turn off the lights.
*/
//% weight=76 blockGap=50 blockId=clearLight block="Clear light"
void clearLight()
{
    if (lhRGBLight)
        lhRGBLight->clear();
}

/**
* Extension pin set
*/
//% weight=70 blockId=setExtsIO block="Set extension pin|%ext|%iostatus"
void setExtsIO(Exts ext, pinIOStatus iostatus)
{
    ;//TODO:
}

/**
* Extension pin read digital
*/
//% weight=68 blockId=readExtsIODigital block="Read extension pin|%ext|digital"
bool readExtsIODigital(Exts ext)
{
    ;//TODO:
}

/**
* Extension  1 pin read analog
*/
//% weight=66 blockGap=50 blockId=readExt1Analog block="Read extension 1 pin analog"
int readExt1Analog()
{
    ;//TODO:
}

/**
* Get Qbit current voltage,the unit is mV
*/
//% weight=64 blockId=getBatVoltage block="Get Qbit current voltage (mV)"
int getBatVoltage()
{
    ;//TODO:
}

/**
* Resolve the Bluetooth that phone APP send command type, the total of nine types of commands: tank display command, servo debug command, obtaining the distance of ultrasonic command, obtaining temperature command, obtain sound size rank orders, to obtain the light level command, set the color lights command, honking command, firmware version information command.
*/
//% weight=62 blockId=analyzeBluetoothCmd block="Get bluetooth command type %str"
//% subcategory=Bluetooth 
int analyzeBluetoothCmd(ManagedString str)
{
    ;//TODO:
}

/**
* Resolve the parameters that the phone APP send the command,there are 3 parameters of servo debug command,the other command has just one parameter.
*/
//% weight=60  blockId=getArgs block="Get bluetooth command|%str|argument at %index"
//% index.min=1 index.max=3
//% subcategory=Bluetooth 
int getArgs(ManagedString str, int index)
{
    ;//TODO:
}

/**
* Returns the enumeration of the command type, which can be compared with this module after obtaining the bluetooth command type sent by the mobile phone APP.
*/
//% weight=58 blockId=getBluetoothCmdtype block="Bluetooth command type %type"
//% subcategory=Bluetooth 
int getBluetoothCmdtype(CmdType type)
{
    ;//TODO:
}

/**
* The command type of the tank is stop, go ahead, back, turn left, turn right, slow down, turn left slowly, turn right slowly.
*/
//% weight=56 blockId=getRunCarType block="Car run type %type"
//% subcategory=Bluetooth      
int getRunCarType(CarRunCmdType type)
{
    ;//TODO:
}

/**
* The distance from the ultrasonic obstacle to the standard command, which is sent to the mobile phone. The APP will indicate the distance of the ultrasonic obstacle.
*/
//% weight=54 blockId=convertUltrasonic block="Convert ultrasonic distance %data"
//% subcategory=Bluetooth  
ManagedString convertUltrasonic(int data)
{
    ;//TODO:
}

/**
* The conversion temperature value to standard command, sent to the mobile phone, and the APP displays the current temperature.
*/
//% weight=52 blockId=convertTemperature block="Convert temperature %data"
//% subcategory=Bluetooth  
ManagedString convertTemperature(int data)
{
    ;//TODO:
}

/**
* Convert the light value to the standard command and send it to the mobile phone. The APP displays the current light level (0~255).
*/
//% weight=50 blockId=convertLight block="Convert light %data"
//% subcategory=Bluetooth  
ManagedString convertLight(int data)
{
    ;//TODO:
}

/**
* Convert the battery value to the standard command and send it to the mobile phone. The APP displays the current voltage.
*/
//% weight=48 blockId=convertBattery block="Convert battery %data"
//% subcategory=Bluetooth  
ManagedString convertBattery(int data)
{
    ;//TODO:
}

}   // qbit
