#include <src/memory/Bus.h>
#include <src/cpu/cpu.h>
#include <src/ppu/ppu.h>
#include <src/memory/Cartridge.h>

#include <cstdlib>

CPU* cpu;

void e()
{
    cpu->Dump();
    PPU::dump();
    Cartridge::Unload();
}

int main()
{
    Bus::LoadCartridge("dk.nes");
    
    cpu = new CPU();

    std::atexit(e);

    while (1)
    {
        int cycles_elapsed = cpu->clock();
        PPU::clock(cycles_elapsed);
    }

    return 0;
}