#include <iostream>
#include <fstream>
#include <istream>
//#include <iterator>

#include "cpu.hpp"
#include "memory.hpp"
#include "lcd.hpp"
#include "interrupts.hpp"
#include "timer.hpp"
#include "mbc.hpp"

GameBoy::LCD::LCD(GameBoy::Interrupts &Interrupts,
                  GameBoy::Memory &MMU)
{
    // _MMU = GameBoy::Memory::Memory();
    _Interrupts = &Interrupts;
    _MMU        = &MMU;
}

GameBoy::LCD::~LCD()
{
}

void GameBoy::LCD::updateScreen(const int numCycles)
{
    setLcdMode();

    if (isLcdEnabled()) {
        _scanlineCounter -= numCycles;
    }

    if (_scanlineCounter <= 0) {

        // If scanline counter <= 0 we know that 456 cycles have passed.
        // This means we need to move on and draw the next scanline.

        // _romMemory[LY]++;
        _MMU->writeRom(LY, _MMU->readRom(LY) + 1);
        byte currentScanline = _MMU->readMemory(LY);
        _scanlineCounter = CYCLES_PER_SCANLINE;

        // There are 144 (0 to 143) scanlines to draw before the VBLANK period.
        // 456 clock cycles elapse per scanline.

        if (currentScanline >= 0 && currentScanline < VBLANK_START_SCANLINE) {
            drawScanline();
        }

        // All 144 scanlines have been drawn, this means we have entered
        // the VBLANK period, which consists of 10 scanlines (144 to 153).
        // Entering VBLANK triggers the VBLANK interrupt.
        // There are 10 VBLANK scanlines, so the VBLANK period lasts
        // 4560 clock cycles (456 * 10).

        if (currentScanline == VBLANK_START_SCANLINE) {
            _Interrupts->generateInterrupt(IRQ_VBLANK);
        }

        // VBLANK over, reset current scanline
        else if (currentScanline > VBLANK_END_SCANLINE) {
            // _romMemory[LY] = 0;
            _MMU->writeRom(LY, 0);
        }
    }
}

bool GameBoy::LCD::isLcdEnabled()
{
    return true;
}

void GameBoy::LCD::setLcdMode()
{
    byte status = _MMU->readMemory(LCD_STATUS);

    if (!isLcdEnabled()) {
        _scanlineCounter = CYCLES_PER_SCANLINE;
        _MMU->writeRom(LY, 0);

        status &= 0xFC; // 0b11111100

        // TODO: Fix this line:
        //_MMU->writeMemory(LCD_STATUS, _MMU->isBitSet(_MBC->isBitSet(status, 0)));
        return;
    }
}

void GameBoy::LCD::drawScanline()
{

}

void GameBoy::LCD::renderScreen()
{

}
