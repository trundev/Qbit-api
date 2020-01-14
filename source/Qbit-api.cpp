/*
* C++ interface to control Qbit implementation
*
* Based on: https://github.com/LOBOT-ROBOT/Qbit
*/
#include "MicroBitPin.h"
#include "MicroBitMessageBus.h"
#include "MicroBitSerial.h"
#include "MicroBitIO.h"
#include "MicroBitFiber.h"
#include "MicroBitSystemTimer.h"
#include "Qbit-api.h"

namespace qbit
{

// Pointers to object-members of the global uBit MicroBit object
static MicroBitMessageBus *uBit_messageBus = NULL;
static MicroBitSerial *uBit_serial = NULL;
static MicroBitIO *uBit_io = NULL;

static QbitRGBLight::LHQbitRGBLight *lhRGBLight = NULL;
static bool obstacleSensor1 = false;
static bool obstacleSensor2 = false;
static int currentVoltage = 0;
static bool versionFlag = false;

#define MESSAGE_HEAD        0xff
#define MESSAGE_HEAD_STOP   0x101

int adress = 0;
bool sendFlag = false;

static void initRGBLight();
static void getHandleCmd();
static void sendVersionCmd();
int findIndexof(const ManagedString &src, const char *strFind, int startIndex);
int strToNumber(const ManagedString &str);
int decStrToNumber(const ManagedString &str);


// Our version of forever_stub() from pxt-microbit/libs/core/codal.cpp:
// https://github.com/microsoft/pxt-microbit/blob/3d5b6d43cc2f7dcd08fe8b80c49edaeee5c608a5/libs/core/codal.cpp#L98-L103
static void forever_stub(void *a)
{
    while (true) {
        void (*fn)() = (void (*)())a;
        fn();
        fiber_sleep(20);
    }
}

/**
* Qbit initialization, please execute at boot time
*/
//% weight=100 blockId=qbitInit block="Initialize Qbit"
void qbitInit(MicroBitMessageBus *bus, MicroBitSerial *serial, MicroBitIO *io)
{
    uBit_messageBus = bus;
    uBit_serial = serial;
    uBit_io = io;
    initRGBLight();
    if (uBit_messageBus && uBit_serial)
    {
        uBit_serial->baud(115200);  // This is the default baud-rate
        uBit_serial->redirect(MICROBIT_PIN_P12, MICROBIT_PIN_P8);
        create_fiber(forever_stub, (void*)getHandleCmd);
    }

    fiber_sleep(1200);
    int readTimes = 0;
    while(readTimes < 10 && !versionFlag)
    {
        readTimes++;
        sendVersionCmd();
        fiber_sleep(30);
    }
}

/**
* Set Qbit run carrier mode
*/
//% weight=99 blockId=qbit_carrier block="Set Qbit carrier mode"
void qbit_carrier()
{
    uint8_t buf[] =
    {
        0x55,
        0x55,
        0x04,
        0x3B,//cmd type
        0x0,
        0x0,
    };
    if (uBit_serial)
        uBit_serial->send(buf, sizeof(buf), ASYNC);
}

static void sendVersionCmd()
{
    uint8_t buf[] =
    {
        0x55,
        0x55,
        0x02,
        0x12,//cmd type
    };
    if (uBit_serial)
        uBit_serial->send(buf, sizeof(buf), ASYNC);
}

/**
* Get the handle command.
*/
static void getHandleCmd()
{
    static ManagedString handleCmd;

    ManagedString charStr = uBit_serial->read(MICROBIT_SERIAL_DEFAULT_BUFFER_SIZE, ASYNC);
    handleCmd = handleCmd + charStr;
    int index = findIndexof(handleCmd, "$", 0);
    if (index != -1) {
        ManagedString cmd = handleCmd.substring(0, index);
        if (cmd.charAt(0) == 'C' && cmd.length() == 5)
        {
            int arg1Int = strToNumber(cmd.substring(1,1));
            int arg2Int = strToNumber(cmd.substring(2,1));
            int arg3Int = strToNumber(cmd.substring(3,2));
            if (arg1Int != -1 && arg2Int != -1)
            {
                if (arg1Int == 0)
                {
                    obstacleSensor1 = true;
                }
                else
                {
                    obstacleSensor1 = false;
                }

                if (arg2Int == 0)
                {
                    obstacleSensor2 = true;
                }
                else
                {
                    obstacleSensor2 = false;
                }
            }
            if (arg3Int != -1) {
                if (arg3Int == 0) {
                    if (adress != 0) {
                        MicroBitEvent evt(MESSAGE_HEAD_STOP, 0, CREATE_AND_FIRE);
                    }
                    sendFlag = false;
                    adress = 0;
                }
                else {
                    if (adress != arg3Int) {
                        if (!sendFlag) {
                            MicroBitEvent evt(MESSAGE_HEAD, arg3Int, CREATE_AND_FIRE);
                            sendFlag = true;
                        }
                        adress = arg3Int;
                    }
                }
            }
        }
        if (cmd.charAt(0) == 'U' && cmd.length() == 5)
        {
            int argInt = decStrToNumber(cmd.substring(1, 4));
            if (argInt != -1)
            {
                currentVoltage = argInt;
            }
        }
        if (cmd.charAt(0) == 'V' && cmd.length() == 4)
        {
            versionFlag = true;
        }
    }
    handleCmd = ManagedString::EmptyString;
}

int findIndexof(const ManagedString &src, const char *strFind, int startIndex)
{
    if (src.length() < startIndex)
        return -1;
    const char *data = src.toCharArray();
    const char *res = strstr(data + startIndex, strFind);
    if (res)
        return res - data;
    return -1;
}

int strToNumber(const ManagedString &str)
{
    char *endptr = NULL;
    int res = strtol(str.toCharArray(), &endptr, 16);
    if (endptr && *endptr == 0)     // All symbols are numerical
        return res;
    return -1;
}

int decStrToNumber(const ManagedString &str)
{
    char *endptr = NULL;
    int res = strtol(str.toCharArray(), &endptr, 10);
    if (endptr && *endptr == 0)     // All symbols are numerical
        return res;
    return -1;
}

/**
* Set the speed of the motor, range of -100~100, that can control the Qbit running.
*/
//% weight=98 blockId=setQbitRunSpeed block="Set Qbit(V2.0 version or newer) run|speed %speed|and oriention %oriention"
//% speed.min=0 speed.max=100
void setQbitRunSpeed(int speed, OrientionType oriention)
{
    if (speed> 100 || speed < 0) {
        return;
    }
    uint8_t buf[] =
    {
        0x55,
        0x55,
        0x04,
        0x32,//cmd type
        speed,
        oriention,
    };
    if (uBit_serial)
        uBit_serial->send(buf, sizeof(buf), ASYNC);
}

/**
* Set the center balance angle of the Qbit
*/
//% weight=96 blockId=setBLAngle block="Set the center balance angle of the Qbit"
void setBLAngle()
{
    uint8_t buf[] =
    {
        0x55,
        0x55,
        0x03,
        61,//cmd type
        0,
    };
    if (uBit_serial)
        uBit_serial->send(buf, sizeof(buf), ASYNC);
}

/**
* Do someting when Qbit receive remote-control code
* @param code the ir key button that needs to be pressed
* @param body code to run when event is raised
*/
//% weight=95 blockId=onQbit_remote_ir_pressed block="on remote-control|%code|pressed"
void onQbit_remote_ir_pressed(IRKEY code, void (*body)(MicroBitEvent))
{
    if (uBit_messageBus)
        uBit_messageBus->listen(MESSAGE_HEAD, code, body);
}

/**
* Do someting when remote-control stop send
* @param body code to run when event is raised
*/
//% weight=94 blockId=onQbit_remote_no_ir block="on remote-control stop send"
void onQbit_remote_no_ir(void (*body)(MicroBitEvent))
{
    if (uBit_messageBus)
        uBit_messageBus->listen(MESSAGE_HEAD_STOP, 0, body);
}

// Our version of pulseIn() from pxt-microbit/libs/core/pins.cpp:
// https://github.com/microsoft/pxt-microbit/blob/912d4d95daa8b6f5f6f853433d781db162f719eb/libs/core/pins.cpp#L231-L261
static int pulseIn(MicroBitPin *pin, bool low_value, int maxDuration = 2000000)
{
    if (!pin) return 0;

    int pulse = low_value == false ? 1 : 0;
    uint64_t tick =  system_timer_current_time_us();
    uint64_t maxd = (uint64_t)maxDuration;
    while(pin->getDigitalValue() != pulse) {
        if(system_timer_current_time_us() - tick > maxd)
            return 0;
    }

    uint64_t start =  system_timer_current_time_us();
    while(pin->getDigitalValue() == pulse) {
        if(system_timer_current_time_us() - tick > maxd)
            return 0;
    }
    uint64_t end =  system_timer_current_time_us();
    return end - start;
}

/**
* Obtain the distance of ultrasonic detection to the obstacle
*/
//% weight=93 blockId=Ultrasonic block="Ultrasonic distance(cm)"
int Ultrasonic()
{
    static int distanceBak = 0;
    if (NULL == uBit_io)
        return distanceBak;

    //init pins
    MicroBitPin &echoPin = uBit_io->P13;
    MicroBitPin &trigPin = uBit_io->P14;
    int distance = 0;
    echoPin.setPull(PullNone);
    trigPin.setPull(PullNone);

    // send pulse
    trigPin.setDigitalValue(0);
    wait_us(5);
    trigPin.setDigitalValue(1);
    wait_us(10);
    trigPin.setDigitalValue(0);
    wait_us(5);
    // read pulse
    int d = pulseIn(&echoPin, false, 11600);
    distance = d;
    // filter timeout spikes
    if (distance == 0 || distance >= 13920) {
        distance = distanceBak;
    }
    else
        distanceBak = d;
    return (distance * 10 + 6 * 58 / 2) / 6 / 58;   // Math.round()
}

/**
* Stop Qbit run.
*/
//% weight=90 blockId=setQbitRun block="Set Qbit balance %runType"
void setQbitRun(QbitRunType runType)
{
    uint8_t buf[] =
    {
        0x55,
        0x55,
        0x03,
        0x3C,//cmd type
        runType,
    };
    if (uBit_serial)
        uBit_serial->send(buf, sizeof(buf), ASYNC);
}

/**
* Detect whether avoid obstacle sensor detect obstacle
*/
//% weight=86 blockGap=50  blockId=obstacleSensor block="avoid obstacle|%sensor|detect obstacle"
bool obstacleSensor(ObstacleSensor sensor)
{
    if (sensor == SENSOR1_OBSTACLE)
    {
        return obstacleSensor1;
    }
    else
    {
        return obstacleSensor2;
    }
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
    if (NULL == uBit_io)
        return;

    if (ext == Exts_Ext1)
    {
        uBit_io->P2.setDigitalValue(iostatus);
    }
    else if (ext == Exts_Ext2)
    {
        uBit_io->P16.setDigitalValue( iostatus);
    }
}

/**
* Extension pin read digital
*/
//% weight=68 blockId=readExtsIODigital block="Read extension pin|%ext|digital"
bool readExtsIODigital(Exts ext)
{
    if (NULL == uBit_io)
        return false;

    int status = 0;
    if (ext == Exts_Ext1)
    {
        status = uBit_io->P2.getDigitalValue();
    }
    else if (ext == Exts_Ext2)
    {
        status = uBit_io->P16.getDigitalValue();
    }
    if(status == 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

/**
* Extension  1 pin read analog
*/
//% weight=66 blockGap=50 blockId=readExt1Analog block="Read extension 1 pin analog"
int readExt1Analog()
{
    if (NULL == uBit_io)
        return 0;

    return uBit_io->P2.getAnalogValue();
}

/**
* Get Qbit current voltage,the unit is mV
*/
//% weight=64 blockId=getBatVoltage block="Get Qbit current voltage (mV)"
int getBatVoltage()
{
    return currentVoltage;
}

/**
* Resolve the Bluetooth that phone APP send command type, the total of nine types of commands: tank display command, servo debug command, obtaining the distance of ultrasonic command, obtaining temperature command, obtain sound size rank orders, to obtain the light level command, set the color lights command, honking command, firmware version information command.
*/
//% weight=62 blockId=analyzeBluetoothCmd block="Get bluetooth command type %str"
//% subcategory=Bluetooth
int analyzeBluetoothCmd(ManagedString str)
{
    if (str.length() > 6) {
        ManagedString cmdHead = str.substring(0, 3);

        if (cmdHead == "CMD") {
            ManagedString cmdTypeStr = str.substring(4, 2);
            int cmdType = strToNumber(cmdTypeStr);
            if (cmdType > CmdType_VERSION || cmdType < 0) {
                return CmdType_NO_COMMAND;
            }
            else {
                return cmdType;
            }
        }
        else {
            return CmdType_NO_COMMAND;
        }
    }
    else {
        return CmdType_NO_COMMAND;
    }
}

/**
* Resolve the parameters that the phone APP send the command,there are 3 parameters of servo debug command,the other command has just one parameter.
*/
//% weight=60  blockId=getArgs block="Get bluetooth command|%str|argument at %index"
//% index.min=1 index.max=3
//% subcategory=Bluetooth
int getArgs(ManagedString str, int index)
{
    CmdType cmdType = (CmdType)analyzeBluetoothCmd(str);
    if (cmdType == CmdType_NO_COMMAND)
    {
        return RunCmdType_COMMAND_ERRO;
    }
    else {
        int dataIndex = 7;
        int subLegth = 2;
        if (index == 2)
        {
            dataIndex = 10;
            subLegth = 4;
        }
        else if (index == 3)
        {
            dataIndex = 15;
            subLegth = 4;
        }
        if (cmdType == CmdType_SERVO)
        {
            if (str.length() < 19)
            {
                return CmdType_NO_COMMAND;
            }
        }
        if ((index == 1 && str.length() < 10)||(index == 2 && str.length() < 15)||(index == 3 && str.length() < 19))
        {
            return 0;
        }
        ManagedString strArgs = str.substring(dataIndex, subLegth);
        int arg = strToNumber(strArgs);
        if (arg == -1)
            return 0;
        return arg;
    }
}

/**
* Returns the enumeration of the command type, which can be compared with this module after obtaining the bluetooth command type sent by the mobile phone APP.
*/
//% weight=58 blockId=getBluetoothCmdtype block="Bluetooth command type %type"
//% subcategory=Bluetooth
int getBluetoothCmdtype(CmdType type)
{
    return (int)type;
}

/**
* The command type of the tank is stop, go ahead, back, turn left, turn right, slow down, turn left slowly, turn right slowly.
*/
//% weight=56 blockId=getRunCarType block="Car run type %type"
//% subcategory=Bluetooth
int getRunCarType(CarRunCmdType type)
{
    return (int)type;
}

/**
* The distance from the ultrasonic obstacle to the standard command, which is sent to the mobile phone. The APP will indicate the distance of the ultrasonic obstacle.
*/
//% weight=54 blockId=convertUltrasonic block="Convert ultrasonic distance %data"
//% subcategory=Bluetooth
ManagedString convertUltrasonic(int data)
{
    ManagedString cmdStr("CMD|03|");
    cmdStr = cmdStr + ManagedString(data);
    cmdStr = cmdStr + "|$";
    return cmdStr;
}

/**
* The conversion temperature value to standard command, sent to the mobile phone, and the APP displays the current temperature.
*/
//% weight=52 blockId=convertTemperature block="Convert temperature %data"
//% subcategory=Bluetooth
ManagedString convertTemperature(int data)
{
    ManagedString cmdStr("CMD|04|");
    cmdStr = cmdStr + ManagedString(data);
    cmdStr = cmdStr + "|$";
    return cmdStr;
}

/**
* Convert the light value to the standard command and send it to the mobile phone. The APP displays the current light level (0~255).
*/
//% weight=50 blockId=convertLight block="Convert light %data"
//% subcategory=Bluetooth
ManagedString convertLight(int data)
{
    ManagedString cmdStr("CMD|06|");
    cmdStr = cmdStr + ManagedString(data);
    cmdStr = cmdStr + "|$";
    return cmdStr;
}

/**
* Convert the battery value to the standard command and send it to the mobile phone. The APP displays the current voltage.
*/
//% weight=48 blockId=convertBattery block="Convert battery %data"
//% subcategory=Bluetooth
ManagedString convertBattery(int data)
{
    ManagedString cmdStr("CMD|07|");
    cmdStr = cmdStr + ManagedString(data);
    cmdStr = cmdStr + "|$";
    return cmdStr;
}

}   // qbit
