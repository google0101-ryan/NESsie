#include <src/memory/Bus.h>
#include <src/cpu/cpu.h>
#include <src/ppu/ppu.h>

#include <cstdlib>

CPU* cpu;

void e()
{
    cpu->Dump();
    PPU::dump();
}

int main()
{
    Bus::LoadCartridge("dk.nes");
    
    cpu = new CPU();

    std::atexit(e);

    while (1)
    {
        int cycles_elapsed = cpu->clock();
    }

    return 0;
}