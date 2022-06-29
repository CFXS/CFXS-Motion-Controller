#include <CFXS/Platform/App.hpp>
#include <CFXS/Platform/TM4C/CoreInit.hpp>
#include <CFXS/Platform/CPU.hpp>
#include <CFXS/Base/Debug.hpp>
#include <CFXS/Base/Time.hpp>

void InitializeModules() {
    asm volatile("nop");
}

void PostInit() {
    asm volatile("nop");
}

void CFXS_SystemPriorityLoop() {
    CFXS::Time::ms++;
}

void CFXS_HighPriorityLoop() {
    asm volatile("nop");
}

///////////////////////////////////////////////////////////////////////////////////

static const CFXS::TM4C::CoreInitDescriptor s_CoreInitDesc{
    .highPriorityTimer      = 7,
    .splitHighPriorityTimer = 0,
};

const CFXS::AppDescriptor e_AppDescriptor{
    .platformInitDescriptor     = &s_CoreInitDesc,
    .moduleInit                 = InitializeModules,
    .postInit                   = PostInit,
    .highPriorityLoopPeriod     = CFXS::CPU::GetCyclesPerMillisecond(),
    .highPriorityLoopPriority   = 4,
    .systemPriorityLoopPeriod   = CFXS::CPU::GetCyclesPerMillisecond(),
    .systemPriorityLoopPriority = 1, // ignored
};
