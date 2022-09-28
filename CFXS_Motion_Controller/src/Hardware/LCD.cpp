#include "LCD.hpp"
#include <CFXS/Platform/CPU.hpp>
#include <CFXS/HW/Peripherals/GPIO.hpp>
#include <CFXS/HW/Peripherals/Descriptors/TM4C/Desc_GPIO_TM4C.hpp>
#include <CFXS/Platform/Task.hpp>

using namespace CFXS::HW;
using namespace CFXS::HW::TM4C;

//////////////////////////////////////////////////////////////////////////
// LCD
static constexpr auto WIDTH  = 240;
static constexpr auto HEIGHT = 320;
//////////////////////////////////////////////////////////////////////////
// Pin Descriptors
static const Desc_GPIO desc_LCD_Data  = {"PM0,1,2,3,4,5,6,7"};
static const Desc_GPIO desc_LCD_Reset = {"PL2"};
static const Desc_GPIO desc_LCD_RS    = {"PL1,2"};
static const Desc_GPIO desc_LCD_WR    = {"PK7"};
static const Desc_GPIO desc_LCD_BL    = {"PA0"};
static const Desc_GPIO desc_LCD_SYNC  = {"PQ3"};

#define WR_PORT   GPIO_PORTK_BASE
#define WR_PIN    GPIO_PIN_7
#define RS_PORT   GPIO_PORTL_BASE
#define RS_PIN    (GPIO_PIN_1 | GPIO_PIN_2)
#define DATA_PORT GPIO_PORTM_BASE
#define DATA_PIN  0xFF
// static const Desc_GPIO desc_LCD_RD    = {""};
//////////////////////////////////////////////////////////////////////////
// Pins
static CFXS::HW::GPIO pin_LCD_Data  = {&desc_LCD_Data};
static CFXS::HW::GPIO pin_LCD_Reset = {&desc_LCD_Reset};
// static CFXS::HW::GPIO pin_LCD_CS    = {&desc_LCD_CS};
static CFXS::HW::GPIO pin_LCD_RS   = {&desc_LCD_RS};
static CFXS::HW::GPIO pin_LCD_WR   = {&desc_LCD_WR};
static CFXS::HW::GPIO pin_LCD_BL   = {&desc_LCD_BL};
static CFXS::HW::GPIO pin_LCD_SYNC = {&desc_LCD_SYNC};
// static CFXS::HW::GPIO pin_LCD_RD    = {&desc_LCD_RD};
//////////////////////////////////////////////////////////////////////////

enum Command {
    SOFTWARE_RESET            = 0x01,
    SLEEP_OUT                 = 0x11,
    DISPLAY_OFF               = 0x28,
    DISPLAY_ON                = 0x29,
    COLUMN_ADDRESS_SET        = 0x2A,
    PAGE_ADDRESS_SET          = 0x2B,
    MEMORY_WRITE              = 0x2C,
    MEMORY_READ               = 0x2E,
    TEARING_EFFECT_LINE       = 0x35,
    MEMORY_ACCESS_CONTROL     = 0x36,
    IDLE_MODE_OFF             = 0x38,
    PIXEL_FORMAT_SET          = 0x3A,
    FRAME_CONTROL_NORMAL      = 0xB1,
    DISPLAY_FUNCTION_CONTROL  = 0xB6,
    POWER_CONTROL_1           = 0xC0,
    POWER_CONTROL_2           = 0xC1,
    VCON_CONTROL_1            = 0xC5,
    VCON_CONTROL_2            = 0xC7,
    POSITIVE_GAMMA_CORRECTION = 0xE0,
    NEGATIVE_GAMMA_CORRECTION = 0xE1,
    InterfaceControl          = 0xF6
};

//////////////////////////////////////////////////////////////////////////////////////////////
static __always_inline void SendResetSequence() {
    pin_LCD_Reset.Write(true);
    CFXS::CPU::BlockingMicroseconds(15);
    pin_LCD_Reset.Write(false);
    CFXS::CPU::BlockingMicroseconds(15);
    pin_LCD_Reset.Write(true);
    CFXS::CPU::BlockingMilliseconds(120);
}

static __always_inline void SendCommand(Command cmd) {
    HWREGB((DATA_PORT) + (DATA_PIN << 2)) = cmd;
    HWREGB((WR_PORT) + (WR_PIN << 2))     = 0;
    HWREGB((RS_PORT) + (RS_PIN << 2))     = 0;
    HWREGB((WR_PORT) + (WR_PIN << 2))     = WR_PIN;
    HWREGB((RS_PORT) + (RS_PIN << 2))     = RS_PIN;
}

static __always_inline void SendData8(size_t data) {
    HWREGB((DATA_PORT) + (DATA_PIN << 2)) = data;
    HWREGB((WR_PORT) + (WR_PIN << 2))     = 0;
    HWREGB((WR_PORT) + (WR_PIN << 2))     = WR_PIN;
}

static __always_inline void SendData16(size_t data) {
    HWREGB((DATA_PORT + 1) + (DATA_PIN << 2)) = data;
    HWREGB((WR_PORT) + (WR_PIN << 2))         = 0;
    HWREGB((WR_PORT) + (WR_PIN << 2))         = WR_PIN;
    HWREGB((DATA_PORT) + (DATA_PIN << 2))     = data;
    HWREGB((WR_PORT) + (WR_PIN << 2))         = 0;
    HWREGB((WR_PORT) + (WR_PIN << 2))         = WR_PIN;
}

static __always_inline void SendCommand8(Command cmd, size_t data) {
    SendCommand(cmd);
    SendData8(data);
}

static __always_inline void SendCommand16(Command cmd, size_t data) {
    SendCommand(cmd);
    SendData16(data);
}

static __always_inline void SetRegion(int y1, int x1, int y2, int x2) {
    y1 = std::min(y1, WIDTH - 1);
    y2 = std::min(y2, WIDTH - 1);
    x1 = std::min(x1, HEIGHT - 1);
    x2 = std::min(x2, HEIGHT - 1);

    SendCommand(Command::PAGE_ADDRESS_SET);
    SendData16(x1);
    SendData16(x2);
    SendCommand(Command::COLUMN_ADDRESS_SET);
    SendData16(y1);
    SendData16(y2);
    SendCommand(Command::MEMORY_WRITE);
}

//////////////////////////////////////////////////////////////////////////////////////////////

static uint16_t s_FrameBuffer[WIDTH][HEIGHT];

void LCD::Initialize() {
    pin_LCD_Reset.Initialize(GPIO::PinType::OUTPUT, static_cast<size_t>(-1));
    pin_LCD_Data.Initialize(GPIO::PinType::OUTPUT);
    pin_LCD_RS.Initialize(GPIO::PinType::OUTPUT);
    pin_LCD_WR.Initialize(GPIO::PinType::OUTPUT);
    pin_LCD_BL.Initialize(GPIO::PinType::OUTPUT);
    pin_LCD_SYNC.Initialize(GPIO::PinType::INPUT);

    pin_LCD_BL.Write(true);
    CFXS::CPU::BlockingMicroseconds(2);
    pin_LCD_BL.Write(false);
    CFXS::CPU::BlockingMicroseconds(3);
    pin_LCD_BL.Write(true);
    CFXS::CPU::BlockingMicroseconds(11);
    pin_LCD_BL.Write(false);
    CFXS::CPU::BlockingMicroseconds(2);

    constexpr int DIM = 31;
    for (int i = 0; i < DIM; i++) {
        pin_LCD_BL.Write(true);
        CFXS::CPU::BlockingMicroseconds(2);
        if (i < DIM - 1) {
            pin_LCD_BL.Write(false);
            CFXS::CPU::BlockingMicroseconds(2);
        }
    }

    SendResetSequence();
    SendCommand(Command::InterfaceControl);
    SendData8(0x01);
    SendData8(0x00);
    SendData8(0x00);

    SendCommand(Command::DISPLAY_OFF);
    SendCommand(Command::SLEEP_OUT);
    SendCommand8(Command::MEMORY_ACCESS_CONTROL, 0b10011100);
    ////////////////////////////////////////////////////// MY  MX  MV  ML  BGR MH  -   -
    // MY - Row Address Order        ______________________|   |   |   |   |   |
    // MX - Column Address Order     __________________________|   |   |   |   |
    // MV - Row/Column Swap          ______________________________|   |   |   |
    // ML - Vertical Refresh Order   __________________________________|   |   |
    // BGR - [0 = RGB] [1 = BGR]     ______________________________________|   |
    // MH - Horizontal Refresh Order __________________________________________|

    SendCommand8(Command::PIXEL_FORMAT_SET, 0b01010101);
    ///////////////////////////////////////////////// -  [ DPI ]  -  [ DBI ]
    // DPI [1, 0, 1] = 16 bits/pixel ____________________|           |
    // BPI [1, 0, 1] = 16 bits/pixel ________________________________|

    SendCommand8(Command::TEARING_EFFECT_LINE, 0); // [0 = V Blank] [1 = V/H Blank]

    SendCommand(Command::DISPLAY_ON);

    //volatile auto t1 = CFXS::CPU::GetCycleCount();
    //volatile auto t2 = CFXS::CPU::GetCycleCount();
    //CFXS_printf("Draw: %ums\n", (t2 - t1) / CFXS::CPU::GetCyclesPerMillisecond());

    SetRegion(0, 0, WIDTH, HEIGHT);
    static CFXS::Time_t nextFrame = 0;
    static CFXS::Time_t lastFrame = 0;
    static int FPS                = 60;
    CFXS::Task::Create(
        LOW_PRIORITY,
        "LCD",
        [](auto...) {
            if (!pin_LCD_SYNC.Read() || (nextFrame > CFXS::Time::ms))
                return;
            auto frameBufferPtr           = reinterpret_cast<const uint16_t*>(s_FrameBuffer);
            constexpr auto frameBufferEnd = &s_FrameBuffer[WIDTH - 1][HEIGHT];
            while (frameBufferPtr < frameBufferEnd) {
                SendData16(*frameBufferPtr++);
            }
            CFXS_printf("%llu FPS\n", 1000 / (CFXS::Time::ms - lastFrame));
            lastFrame = CFXS::Time::ms;
            nextFrame = CFXS::Time::ms + 1000 / FPS;
            for (int y = 0; y < HEIGHT; y++) {
                for (int x = 0; x < WIDTH; x++) {
                    s_FrameBuffer[x][y] = (x ^ y) & 1 ? 0xFFFF : 0x0000;
                }
            }
        },
        0)
        ->Start();
}