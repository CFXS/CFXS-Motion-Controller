return {
    EntryPoint = "__cfxs_reset",

    Memory = {
        ["ITCMRAM"] = { permissions = "rwx", start = 0x00000000, size = 64 * kB },
        ["FLASH"]   = { permissions = "r-x", start = 0x08000000, size = 2 * MB },
        ["DTCMRAM"] = { permissions = "rwx", start = 0x20000000, size = 128 * kB },
        ["RAM_D1"]  = { permissions = "rwx", start = 0x24000000, size = 512 * kB },
        ["RAM_D2"]  = { permissions = "rwx", start = 0x30000000, size = 288 * kB },
        ["RAM_D3"]  = { permissions = "rwx", start = 0x38000000, size = 64 * kB },

        Alias = {
            ["RAM"]               = "RAM_D1",
            ["RAM_2"]             = "RAM_D2",
            ["RAM_3"]             = "RAM_D3",
            ["RAM_CACHE"]         = "DTCMRAM",
            ["INSTRUCTION_CACHE"] = "ITCMRAM"
        }
    },

    Stack = {
        location = "RAM",
        size = 8 * kB
    },

    NoDefaultSections = false,
    Sections = {
        {
            name = "ram_2",
            location = "FLASH",
            target = "RAM_2",
            content = {
                Define("__RAM_2_START__"),
                Place({ align = 4 }, ".ram_2", ".ram_2*"),
                Define("__RAM_2_END__"),
            }
        },
        {
            name = "ram_3",
            location = "FLASH",
            target = "RAM_3",
            content = {
                Define("__RAM_3_START__"),
                Place({ align = 4 }, ".ram_3", ".ram_3*"),
                Define("__RAM_3_END__"),
            }
        },
        {
            name = "ram_cache",
            location = "FLASH",
            target = "RAM_CACHE",
            content = {
                Define("__RAM_CACHE_START__"),
                Place({ align = 4 }, ".ram_cache", ".ram_cache*"),
                Define("__RAM_CACHE_END__"),
            }
        },
        {
            name = "instruction_cache",
            location = "FLASH",
            target = "INSTRUCTION_CACHE",
            content = {
                Define("__INSTRUCTION_CACHE_START__"),
                Place({ align = 4 }, ".fast_ram_func", ".fast_ram_func*"),
                Define("__INSTRUCTION_CACHE_END__"),
            }
        }
    }
}
