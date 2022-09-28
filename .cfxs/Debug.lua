print(string.format("Boot SP: 0x%08X", ReadMem32(0x08000000 + 0)))
print(string.format("Boot PC: 0x%08X", ReadMem32(0x08000000 + 4)))
print(string.format("Main Loop Cycles: %u", ReadMem64(addressof("s_Cycles"))))
math.randomseed(ReadMem64(addressof("s_Cycles")))
WriteMem8(addressof("s_RedState"), math.random(100) > 50 and 1 or 0)
WriteMem8(addressof("s_GreenState"), math.random(100) > 50 and 1 or 0)
WriteMem8(addressof("s_YellowState"), math.random(100) > 50 and 1 or 0)
WriteMem64(addressof("s_Cycles"), 0)
--[[local Heap = {
    _T = CreateStruct({
        {c_str, "m_Label"},
        {void_ptr, "m_Handle"},
        {size_t, "m_Size"},
        {size_t, "m_UsedBytes"},
        {size_t, "m_MaxUsedBytes"},
        {uint64_t, "m_AllocCount"},
        {uint64_t, "m_FreeCount"},
    })
}

local HeapArray = {
    _T = CreateStruct({
        {Heap, "m_Heap0"},
        {Heap, "m_Heap1"},
        {Heap, "m_Heap2"},
        {Heap, "m_Heap3"},
    })
}

----------------------------------------------

local Time_ms = ReadMemX(sizeof("CFXS::Time::ms"), addrof("CFXS::Time::ms"))

local s_HeapCount = ReadMemX(sizeof("CFXS::s_HeapCount"), addrof("CFXS::s_HeapCount"))
local s_Heaps = ReadStruct(HeapArray, addrof("CFXS::s_Heaps"))

printf("Heap count: %u", s_HeapCount);

for i = 0, s_HeapCount-1 do
    local heap = s_Heaps["m_Heap"..i]
    printf("[Heap %u]", i);
    printf(" - Label:       %s", ReadString(heap.m_Label, 32))
    printf(" - Handle:      0x%08X", heap.m_Handle)
    printf(" - Usage:       %.1fkB/%.1fkB (%.1f%%)", heap.m_UsedBytes/1024, heap.m_Size/1024, 100/heap.m_Size*heap.m_UsedBytes);
    printf(" - Peak Usage:  %.1fkB (%.1f%%)", heap.m_MaxUsedBytes/1024, 100/heap.m_Size*heap.m_MaxUsedBytes);
    printf(" - Alloc Count: %u", heap.m_AllocCount)
    printf(" - Free Count:  %u", heap.m_FreeCount)
    printf(" - A-F Delta:   %u", heap.m_AllocCount - heap.m_FreeCount)
end--]]
