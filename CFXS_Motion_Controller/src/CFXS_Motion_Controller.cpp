// [CFXS] //
#include <CFXS/Platform/App.hpp>
#include <CFXS/Platform/TM4C/CoreInit.hpp>
#include <CFXS/Platform/CPU.hpp>
#include <CFXS/Base/Debug.hpp>
#include <CFXS/Base/Time.hpp>
#include <CFXS/Platform/Task.hpp>

using CFXS::Task;

void InitializeModules() {
    Task::AddGroup(LOW_PRIORITY, 64);
    Task::AddGroup(HIGH_PRIORITY, 32);
    Task::AddGroup(SYSTEM_PRIORITY, 2);
}

void InitializeApp() {
    auto t = Task::Create(
        LOW_PRIORITY,
        "Test Task",
        [](void* data) {
            CFXS_printf("Task %llu\n", CFXS::Time::ms);
        },
        1000);
    t->Start();

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
