return {
    ProjectName = "CFXS Motion Controller",
    
    Toolchain = "GCC",
    --UnityBuildBatchSize = 16,

    CPU = "TM4C1294NCPDT",
    CLOCK_FREQUENCY = 120e6,
    
    Modules = {
        "CMake",
        "CFXS-Base",
        "CFXS-HW",
        "CFXS-CNC",
        "tm4c-driverlib",
        "SeggerRTT"
    },

    IncludeDirectories = {
        "/src"
    },

    Defines = {
        "debug:DEBUG",
        "release:RELEASE",
        "release:NDEBUG",
    },
}