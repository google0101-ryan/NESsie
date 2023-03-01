#include "ppu.h"

#include <cstdio>
#include <fstream>

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

bool scroll_latch = false;
bool addr_latch = false;

uint8_t horizontal_scroll;
uint8_t vertical_scroll;

uint8_t vram[0x4000];

void PPU::dump()
{
    std::ofstream vr("vram.dump");

    for (int i = 0; i < 0x4000; i++)
        vr << vram[i];
    
    vr.close();

    printf("[PPU]: Background nametable 0x%04x\n", ppu_ctrl.bg_pattern_addr ? 0x1000 : 0x0000);
}

int scanline = -1;
int cycles_elapsed = 0;

void PPU::clock(int cycles)
{
    if (scanline <= 239 && scanline != -1)
    {
        // TODO: on-screen rendering
    }
    else if (scanline == 240)
    {
        // Post-render scanline
    }
    else if (scanline >= 241 && scanline <= 260)
    {
        if (scanline == 241)
        {
            printf("VBLANK!\n");
            ppu_ctrl.vblank_nmi = 1;
            exit(1);
        }

        if (scanline == 260)
        {
            scanline = -1;
            ppu_ctrl.vblank_nmi = 0;
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
    printf("[PPU]: Wrote 0x%02x to 0x%04x\n", data, ppuaddr);
    vram[ppuaddr & 0x3FFF] = data;
    ppuaddr += ppu_ctrl.vram_inc ? 32 : 1;
}

uint8_t PPU::read_ppustatus()
{
    scroll_latch = false;
    addr_latch = false;
    return 0x80; // Hack: VBLANK always on (Used because Donkey Kong)
}
