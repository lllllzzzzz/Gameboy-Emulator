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

GameBoy::Timer::Timer(GameBoy::Interrupts &Interrupts,
                      GameBoy::Memory &MMU)
{
    // _MMU = GameBoy::Memory::Memory();
}

GameBoy::Timer::~Timer()
{
}

void GameBoy::Timer::updateTimers(const int numCycles)
{
    doDividerRegister(numCycles);

    if (isClockEnabled()) {
        _timerCounter -= numCycles;
    }

    if (_timerCounter <= 0) {
        setClockFreq();

        // If the current timer (TIMA) has reached overflow limit, reset
        // TIMA to the value of the timer modulator (TMA).
        // TIMA overflow also generates a timer interrupt.

        if (readMemory(TIMA) == TIMA_OVERFLOW) {
            writeMemory(TIMA, readMemory(TMA));
            generateInterrupt(IRQ_TIMER);
        } else {
            writeMemory(TIMA, readMemory(TIMA) + 1);
        }
    }
}

byte GameBoy::Timer::getClockFreq()
{
    static const int LOWER_2_BYTES = 0x3;

    // Lower 2 bytes of TMC store current clock frequency
    return readMemory(TMC) & LOWER_2_BYTES;
}

void GameBoy::Timer::setClockFreq()
{
    byte clockFreq = getClockFreq();
    switch (clockFreq) {
        case 0x00: _timerCounter = 1024; break; // Frequency = 4096
        case 0x01: _timerCounter = 16;   break; // Frequency = 262144
        case 0x10: _timerCounter = 64;   break; // Frequency = 65536
        case 0x11: _timerCounter = 256;  break; // Frequency = 16382
    }
}

bool GameBoy::Timer::isClockEnabled()
{
    // TMC bit 2 is clock enable flag (1 == on, 0 == off)
    return (isBitSet(readMemory(TMC), 2));
}
