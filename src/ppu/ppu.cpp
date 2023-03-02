#include "ppu.h"

#include <cstdio>
#include <fstream>
#include <src/cpu/cpu.h>
#include <src/memory/Bus.h>
#include <src/memory/Cartridge.h>

uint8_t vram[0x4000];
uint8_t oam[0xFF];

uint8_t read8(uint16_t addr)
{
    if (addr < 0x2000)
        return Cartridge::read8_ppu(addr);
    return vram[addr];
}

extern CPU* cpu;

std::ofstream screen; // For now, we'll use an ascii screen

union PPUCtrl
{
    uint8_t value;
    struct
    {
        uint8_t nametable_addr : 2,
        vram_inc : 1,
        spr_pattern_addr : 1,
        bg_pattern_addr : 1,
        sprite_size : 1,
        ppu_master_slave : 1,
        vblank_nmi : 1;
    };
} ppu_ctrl;

union PPU_MASK
{
    uint8_t value;
    struct
    {
        uint8_t grayscale : 1,
        background_left_enable_bg : 1,
        background_left_enable_spr : 1,
        bg_enable : 1,
        spr_enable : 1,
        emphasize_red : 1,
        emphaisze_green : 1,
        emphasize_blue : 1;
    };
} ppu_mask;

union PPU_STATUS
{
    uint8_t status;
    struct
    {
        uint8_t : 5,
        sprite_overflow : 1,
        sprite_0_hit : 1,
        vblank : 1;
    };
} ppu_status;

bool scroll_latch = false;
bool addr_latch = false;
bool is_oam = false;

uint8_t horizontal_scroll;
uint8_t vertical_scroll;

void PPU::dump()
{
    std::ofstream vr("vram.dump");

    for (int i = 0; i < 0x4000; i++)
        vr << vram[i];
    
    vr.close();

    vr.open("oam.dump");

    for (int i = 0; i < 0xFF; i++)
        vr << oam[i];
    
    vr.close();

    vr.open("chr.dump");

    for (int i = 0; i < 0x2000; i++)
        vr << read8(i);
    
    vr.close();

    printf("[PPU]: Background nametable 0x%04x\n", ppu_ctrl.bg_pattern_addr ? 0x1000 : 0x0000);
}

int scanline = -1;
int cycles_elapsed = 0;
uint16_t oam_addr;
uint16_t oam_dma_addr;
bool oam_odd;

void PPU::clock(int cycles)
{
    if (!screen.is_open())
        screen.open("screen.txt");

    if (is_oam)
    {
        if (oam_odd)
        {
            oam_odd = false;
            goto state;
        }
        else
            oam_odd = true;
        
        uint8_t data = Bus::read8(oam_dma_addr);
        oam[oam_addr] = data;
        oam_addr++;
        oam_dma_addr++;
        

        if (oam_addr == 0xFF)
        {
            is_oam = false;
            cpu->SuspendCpu(false);
        }
    }

state:
    if (scanline <= 239 && scanline != -1)
    {
        if ((cycles_elapsed + cycles) >= 341)
        {
            // Draw scanline

            // Draw bg first
            uint16_t bg_map_addr = ppu_ctrl.bg_pattern_addr ? 0x1000 : 0x0000;
            
            for (int i = 0; i < 32; i++)
            {
                int tile_num = vram[0x2000 + ((scanline/8)*32) + i];
                int line = scanline % 8;

                tile_num *= 16;

                uint8_t first_row = read8((bg_map_addr + tile_num) + line);
                uint8_t second_row = read8(bg_map_addr + (tile_num+8) + line);

                for (int i = 7; i >= 0; i--)
                {
                    uint8_t pixel = ((first_row >> i) & 1);
                    pixel |= ((second_row >> i) & 1) << 1;
                    screen << std::to_string(pixel);
                }
            }
            screen << "\n";
        }
    }
    else if (scanline == 240)
    {
        // Post-render scanline
    }
    else if (scanline >= 241 && scanline <= 260)
    {
        if (scanline == 241 && cycles_elapsed == 0)
        {
            ppu_status.vblank = 1;
            screen.seekp(0);
            if (ppu_ctrl.vblank_nmi)
                cpu->nmi();
        }

        if (scanline == 260)
        {
            scanline = -1;
            ppu_status.vblank = 0;
        }
    }

    cycles_elapsed += cycles;
    if (cycles_elapsed >= 341)
    {
        cycles_elapsed = 0;
        scanline++;
    }
}

void PPU::write_ppuctrl(uint16_t addr, uint8_t data)
{
    ppu_ctrl.value = data;
}

void PPU::write_ppumask(uint16_t addr, uint8_t data)
{
    ppu_mask.value = data;
}

void PPU::write_scroll(uint8_t data)
{
    if (!scroll_latch)
    {
        horizontal_scroll = data;
        scroll_latch = true;
    }
    else
        vertical_scroll = data;
}

uint32_t ppuaddr;

void PPU::write_ppuaddr(uint8_t data)
{
    if (!addr_latch)
    {
        ppuaddr = (data << 8);
        addr_latch = true;
    }
    else
        ppuaddr |= data;
}

void PPU::write_vram(uint8_t data)
{
    printf("[PPU]: Writing 0x%02x to 0x%04x\n", data, ppuaddr);
    vram[ppuaddr & 0x3FFF] = data;
    ppuaddr += ppu_ctrl.vram_inc ? 32 : 1;
}

void PPU::write_oamaddr(uint8_t data)
{
    oam_addr = data;
}

void PPU::write_oamdma(uint8_t data)
{
    cpu->SuspendCpu(true);
    is_oam = true;
    oam_dma_addr = (data << 8);
}

uint8_t PPU::read_ppustatus()
{
    scroll_latch = false;
    addr_latch = false;
    return ppu_status.status;
}
