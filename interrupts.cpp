#include <iostream>
#include <fstream>
#include <istream>
//#include <iterator>
#include "mbc.hpp"


GameBoy::Interrupts::Interrupts()
{
    // _MMU = GameBoy::Memory::Memory();
}

GameBoy::Interrupts::~Interrupts()
{
}

void GameBoy::Emulator::generateInterrupt(const byte interruptVector)
{
    byte interruptRequest = readMemory(IF);
    interruptRequest = setBit(interruptRequest, interruptVector);
    writeMemory(IF, interruptVector);
}

void GameBoy::Emulator::doInterrupts()
{
    if (_interruptsEnabled) {
        byte interruptRequest = readMemory(0xFF0F);
        byte interruptEnable  = readMemory(IE);

        if (interruptRequest > 0) {
            for (int i = 0; i < 5; i++) {
                if ((interruptRequest & i) && (interruptEnable & i)) {
                    serviceInterrupt(i);
                }
            }
        }
    }
}

void GameBoy::Emulator::serviceInterrupt(const byte interruptNum)
{
    _interruptsEnabled = false;
    byte interruptRequest = readMemory(IF);
    interruptRequest = resetBit(interruptRequest, interruptNum);
    writeMemory(IF, interruptRequest);

    pushWord(_PC.w);

    switch (interruptNum) {
        case IRQ_VBLANK: _PC.w = VECTOR_VBLANK; break;  // VBLANK
        case IRQ_LCDC:   _PC.w = VECTOR_LCDC;   break;  // LCD
        case IRQ_TIMER:  _PC.w = VECTOR_TIMER;  break;  // TIMER
        case IRQ_JOYPAD: _PC.w = VECTOR_JOYPAD; break;  // JOYPAD
    }
}
