// [CFXS] //
#include <CFXS/Platform/App.hpp>
#include <CFXS/Platform/TM4C/CoreInit.hpp>
#include <CFXS/Platform/CPU.hpp>
#include <CFXS/Base/Debug.hpp>
#include <CFXS/Base/Time.hpp>
#include <CFXS/Platform/Task.hpp>

using CFXS::Task;

void InitializeModules() {
    Task::AddGroups({
        {LOW_PRIORITY, 64},
        {HIGH_PRIORITY, 32},
        {SYSTEM_PRIORITY, 2},
    });
}

#include <CFXS/HW/Peripherals/GPIO.hpp>
#include <CFXS/HW/Peripherals/Descriptors/TM4C/Desc_GPIO_TM4C.hpp>

#include <driverlib/ssi.h>
#include <inc/hw_ssi.h>
#include <inc/hw_memmap.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>

using namespace CFXS::HW;
using namespace CFXS::HW::TM4C;
static const Desc_GPIO desc_LED = {"PF0"};
static CFXS::HW::GPIO pin_LED   = {&desc_LED};

static const Desc_GPIO desc_BTN = {"PJ0", 0, GPIO_PIN_TYPE_STD_WPU};
static CFXS::HW::GPIO pin_BTN   = {&desc_BTN};

static const Desc_GPIO desc_RST = {"PC4"};
static CFXS::HW::GPIO pin_RST   = {&desc_RST};

static const Desc_GPIO desc_SWO = {"PC5"};
static CFXS::HW::GPIO pin_SWO   = {&desc_SWO};

__used volatile int asd = 1337;

void InitializeApp() {
    pin_LED.Initialize(GPIO::PinType::OUTPUT);
    pin_RST.Initialize(GPIO::PinType::OUTPUT);
    pin_BTN.Initialize(GPIO::PinType::INPUT);
    pin_SWO.Initialize(GPIO::PinType::INPUT);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

    GPIOPinConfigure(GPIO_PE4_SSI1XDAT0);
    GPIOPinConfigure(GPIO_PE5_SSI1XDAT1);
    GPIOPinConfigure(GPIO_PB5_SSI1CLK);

    GPIOPinTypeSSI(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);
    GPIOPinTypeSSI(GPIO_PORTB_BASE, GPIO_PIN_5);

    GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_4, GPIO_STRENGTH_6MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_5, GPIO_STRENGTH_6MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_5, GPIO_STRENGTH_6MA, GPIO_PIN_TYPE_STD_WPU);

    //SSIConfigSetExpClk(SSI1_BASE, CFXS::CPU::CLOCK_FREQUENCY, SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 1000, 9);
    //SSIEnable(SSI1_BASE);

    auto t2 = Task::Create(
        LOW_PRIORITY,
        "Test",
        [](void* data) {
            pin_LED.Write(!pin_BTN.Read());
            asd += !pin_BTN.Read();
        },
        1);
    t2->Start();

    Task::EnableProcessing();
}

///////////////////////////////////////////////////////////////////////////////////

void CFXS_SystemPriorityLoop() {
    CFXS::Time::ms++;
    Task::ProcessGroup(SYSTEM_PRIORITY);
}

void CFXS_HighPriorityLoop() {
    Task::ProcessGroup(HIGH_PRIORITY);
}

void CFXS_LowPriorityLoop() {
    Task::ProcessGroup(LOW_PRIORITY);
}

///////////////////////////////////////////////////////////////////////////////////

static const CFXS::TM4C::CoreInitDescriptor s_CoreInitDesc{
    .highPriorityTimer      = 7,
    .splitHighPriorityTimer = 0,
};

const CFXS::AppDescriptor e_AppDescriptor{
    .platformInitDescriptor     = &s_CoreInitDesc,
    .moduleInit                 = InitializeModules,
    .postModuleInit             = InitializeApp,
    .highPriorityLoopPeriod     = CFXS::CPU::GetCyclesPerMillisecond(),
    .highPriorityLoopPriority   = 4,
    .systemPriorityLoopPeriod   = CFXS::CPU::GetCyclesPerMillisecond(),
    .systemPriorityLoopPriority = 1, // ignored
};
