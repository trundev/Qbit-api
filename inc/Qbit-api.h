/*
* C++ interface to control Qbit declaration
*
* Based on: https://github.com/LOBOT-ROBOT/Qbit
*/
#ifndef _QBIT_API_
#define _QBIT_API_

#include "ManagedString.h"
#include "MicroBitEvent.h"

#include "QbitRGBLight.h"

class MicroBitMessageBus;
class MicroBitSerial;
class MicroBitIO;

namespace qbit
{

enum Colors {
    //% blockId="Red" block="Red"
    Red = 0x01,
    //% blockId="Green" block="Green"
    Green = 0x02,
    //% blockId="Blue" block="Blue"
    Blue = 0x03,
    //% blockId="White" block="White"
    White = 0x04,
    //% blockId="Black" block="Black"
    Black = 0x05
};

enum ObstacleSensor {
    //% block="sensor 1"
    SENSOR1_OBSTACLE = 0x01,
    //% block="sensor 2"
    SENSOR2_OBSTACLE = 0x02
};

enum QbitRunType {
    //% block="Stop"
    RunType_STOP = 0x00,
    //% block="Run"
    RunType_RUN = 0x01
};

enum Exts {
    //% block="Ext 1"
    Exts_Ext1 = 0x00,
    //% block="Ext 2"
    Exts_Ext2 = 0x01
};

enum pinIOStatus {
    //% block="LOW"
    pinIOStatus_Low = 0x00,
    //% block="HIGHT"
    pinIOStatus_Hight = 0x01
};

enum CmdType {
    //% block="Invalid command"
    CmdType_NO_COMMAND = 0,
    //% block="car run"
    CmdType_CAR_RUN = 1,
    //% block="Servo"
    CmdType_SERVO = 2,
    //% block="Ultrasonic distance"
    CmdType_ULTRASONIC = 3,
    //% block="Temperature"
    CmdType_TEMPERATURE = 4,
    //% block="Sound"
    CmdType_SOUND = 5,
    //% block="Light"
    CmdType_LIGHT = 6,
    //% block="Voltage"
    CmdType_BAT = 7,
    //% block="Rgb light"
    CmdType_RGB_LIGHT = 8,
    //% block="Honk horn"
    CmdType_DIDI = 9,
    //% block="Read firmware version"
    CmdType_VERSION = 10
};

enum CarRunCmdType {
    //% block="Stop"
    RunCmdType_STOP = 0,
    //% block="Go ahead"
    RunCmdType_GO_AHEAD,
    //% block="Back"
    RunCmdType_GO_BACK,
    //% block="Turn left"
    RunCmdType_TURN_LEFT,
    //% block="Turn right"
    RunCmdType_TURN_RIGHT,
    //% block="Go ahead slowly"
    RunCmdType_GO_AHEAD_SLOW,
    //% block="Turn left slowly"
    RunCmdType_TURN_LEFT_SLOW,
    //% block="Turn right slowly"
    RunCmdType_TURN_RIGHT_SLOW,
    //% block="Invalid command"
    RunCmdType_COMMAND_ERRO
};

enum OrientionType {
    //% block="Stop"
    OrientionType_STOP = 0,
    //% block="Go ahead"
    OrientionType_GO_AHEAD = 1,
    //% block="Back"
    OrientionType_GO_BACK = 2,
    //% block="Turn left"
    OrientionType_TURN_LEFT = 3,
    //% block="Turn right"
    OrientionType_TURN_RIGHT = 4
};

enum IRKEY {
    //% block="CH-"
    CH_MINUS=162,
    //% block="CH"
    CH=98,
    //% block="CH+"
    CH_ADD=226,
    //% block="PREV"
    PREV=34,
    //% block="NEXT"
    NEXT=2,
    //% block="PLAY/PAUSE"
    PLAY_PAUSE=194,
    //% block="+"
    ADD=168,
    //% block="-"
    MINUS=224,
    //% block="EQ"
    EQ=144,
    //% block="100+"
    _100=152,
    //% block="200+"
    _200=176,
    //% block="A"
    A=0x0C,
    //% block="B"
    B=0x8C,
    //% block="C"
    C = 0x4C,
    //% block="D"
    D = 0xCC,
    //% block="E"
    E = 0xAC,
    //% block="F"
    F = 0x5C,
    //% block="UP"
    UP = 0X2C,
    //% block="DOWN"
    DOWN = 0X9C,
    //% block="LEFT"
    LEFT = 0X6C,
    //% block="RIGHT"
    RIGHT = 0X1C,
    //% block="SET"
    SET = 0XEC,
    //% block="R0"
    R0 = 104,
    //% block="R1"
    R1=48,
    //% block="R2"
    R2=24,
    //% block="R3"
    R3=122,
    //% block="R4"
    R4=16,
    //% block="R5"
    R5=56,
    //% block="R6"
    R6=90,
    //% block="R7"
    R7=66,
    //% block="R8"
    R8=74,
    //% block="R9"
    R9=82
};


/**
* Qbit initialization, please execute at boot time
*/
//% weight=100 blockId=qbitInit block="Initialize Qbit"
void qbitInit(MicroBitMessageBus *bus, MicroBitSerial *serial, MicroBitIO *io);

/**
* Set Qbit run carrier mode
*/
//% weight=99 blockId=qbit_carrier block="Set Qbit carrier mode"
void qbit_carrier();

/**
* Set the speed of the motor, range of -100~100, that can control the Qbit running.
*/
//% weight=98 blockId=setQbitRunSpeed block="Set Qbit(V2.0 version or newer) run|speed %speed|and oriention %oriention"
//% speed.min=0 speed.max=100
void setQbitRunSpeed(int speed, OrientionType oriention);

/**
* Set the center balance angle of the Qbit
*/
//% weight=96 blockId=setBLAngle block="Set the center balance angle of the Qbit"
void setBLAngle();

/**
* Do someting when Qbit receive remote-control code
* @param code the ir key button that needs to be pressed
* @param body code to run when event is raised
*/
//% weight=95 blockId=onQbit_remote_ir_pressed block="on remote-control|%code|pressed"
void onQbit_remote_ir_pressed(IRKEY code, void (*body)(MicroBitEvent));

/**
* Do someting when remote-control stop send
* @param body code to run when event is raised
*/
//% weight=94 blockId=onQbit_remote_no_ir block="on remote-control stop send"
void onQbit_remote_no_ir(void (*body)(MicroBitEvent));

/**
* Obtain the distance of ultrasonic detection to the obstacle
*/
//% weight=93 blockId=Ultrasonic block="Ultrasonic distance(cm)"
int Ultrasonic();

/**
* Stop Qbit run.
*/
//% weight=90 blockId=setQbitRun block="Set Qbit balance %runType"
void setQbitRun(QbitRunType runType);

/**
* Detect whether avoid obstacle sensor detect obstacle
*/
//% weight=86 blockGap=50  blockId=obstacleSensor block="avoid obstacle|%sensor|detect obstacle"
bool obstacleSensor(ObstacleSensor sensor);

/**
* Set the brightness of the strip. This flag only applies to future operation.
* @param brightness a measure of LED brightness in 0-255. eg: 255
*/
//% blockId="setBrightness" block="set light brightness %brightness"
//% weight=84
void setBrightness(int brightness);

/**
* Set the color of the colored lights, after finished the setting please perform  the display of colored lights.
*/
//% weight=82 blockId=setPixelRGB block="Set|%lightoffset|color to %rgb"
void setPixelRGB(QbitRGBLight::Lights lightoffset, QbitRGBLight::QbitRGBColors rgb);

/**
* Set RGB Color argument
*/
//% weight=80 blockId=setPixelRGBArgs block="Set|%lightoffset|color to %rgb"
void setPixelRGBArgs(QbitRGBLight::Lights lightoffset, int rgb);

/**
* Display the colored lights, and set the color of the colored lights to match the use. After setting the color of the colored lights, the color of the lights must be displayed.
*/
//% weight=78 blockId=showLight block="Show light"
void showLight();

/**
* Clear the color of the colored lights and turn off the lights.
*/
//% weight=76 blockGap=50 blockId=clearLight block="Clear light"
void clearLight();

/**
* Extension pin set
*/
//% weight=70 blockId=setExtsIO block="Set extension pin|%ext|%iostatus"
void setExtsIO(Exts ext, pinIOStatus iostatus);

/**
* Extension pin read digital
*/
//% weight=68 blockId=readExtsIODigital block="Read extension pin|%ext|digital"
bool readExtsIODigital(Exts ext);

/**
* Extension  1 pin read analog
*/
//% weight=66 blockGap=50 blockId=readExt1Analog block="Read extension 1 pin analog"
int readExt1Analog();

/**
* Get Qbit current voltage,the unit is mV
*/
//% weight=64 blockId=getBatVoltage block="Get Qbit current voltage (mV)"
int getBatVoltage();

/**
* Resolve the Bluetooth that phone APP send command type, the total of nine types of commands: tank display command, servo debug command, obtaining the distance of ultrasonic command, obtaining temperature command, obtain sound size rank orders, to obtain the light level command, set the color lights command, honking command, firmware version information command.
*/
//% weight=62 blockId=analyzeBluetoothCmd block="Get bluetooth command type %str"
//% subcategory=Bluetooth
int analyzeBluetoothCmd(ManagedString str);

/**
* Resolve the parameters that the phone APP send the command,there are 3 parameters of servo debug command,the other command has just one parameter.
*/
//% weight=60  blockId=getArgs block="Get bluetooth command|%str|argument at %index"
//% index.min=1 index.max=3
//% subcategory=Bluetooth
int getArgs(ManagedString str, int index);

/**
* Returns the enumeration of the command type, which can be compared with this module after obtaining the bluetooth command type sent by the mobile phone APP.
*/
//% weight=58 blockId=getBluetoothCmdtype block="Bluetooth command type %type"
//% subcategory=Bluetooth
int getBluetoothCmdtype(CmdType type);

/**
* The command type of the tank is stop, go ahead, back, turn left, turn right, slow down, turn left slowly, turn right slowly.
*/
//% weight=56 blockId=getRunCarType block="Car run type %type"
//% subcategory=Bluetooth
int getRunCarType(CarRunCmdType type);

/**
* The distance from the ultrasonic obstacle to the standard command, which is sent to the mobile phone. The APP will indicate the distance of the ultrasonic obstacle.
*/
//% weight=54 blockId=convertUltrasonic block="Convert ultrasonic distance %data"
//% subcategory=Bluetooth
ManagedString convertUltrasonic(int data);

/**
* The conversion temperature value to standard command, sent to the mobile phone, and the APP displays the current temperature.
*/
//% weight=52 blockId=convertTemperature block="Convert temperature %data"
//% subcategory=Bluetooth
ManagedString convertTemperature(int data);

/**
* Convert the light value to the standard command and send it to the mobile phone. The APP displays the current light level (0~255).
*/
//% weight=50 blockId=convertLight block="Convert light %data"
//% subcategory=Bluetooth
ManagedString convertLight(int data);

/**
* Convert the battery value to the standard command and send it to the mobile phone. The APP displays the current voltage.
*/
//% weight=48 blockId=convertBattery block="Convert battery %data"
//% subcategory=Bluetooth
ManagedString convertBattery(int data);

}   // qbit

#endif // _QBIT_API_
