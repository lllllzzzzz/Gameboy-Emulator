#include <iostream>
#include <fstream>
#include <istream>
//#include <iterator>
#include "mbc.hpp"


GameBoy::Interrupts::Interrupts()
{
    _MMU = GameBoy::Memory::Memory();
}

GameBoy::Interrupts::~Interrupts()
{
}

void GameBoy::Interrupts::generateInterrupt(const byte interruptVector)
{
    byte interruptRequest = _MMU->readMemory(IF);
    interruptRequest = setBit(interruptRequest, interruptVector);
    _MMU->writeMemory(IF, interruptVector);
}

void GameBoy::Interrupts::doInterrupts()
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

void GameBoy::Interrupts::serviceInterrupt(const byte interruptNum)
{
    _interruptsEnabled = false;
    byte interruptRequest = _MMU->readMemory(IF);
    interruptRequest = _MMU->resetBit(interruptRequest, interruptNum);
    _MMU->writeMemory(IF, interruptRequest);

    pushWord(_PC.w);

    switch (interruptNum) {
        case IRQ_VBLANK: _PC.w = VECTOR_VBLANK; break;  // VBLANK
        case IRQ_LCDC:   _PC.w = VECTOR_LCDC;   break;  // LCD
        case IRQ_TIMER:  _PC.w = VECTOR_TIMER;  break;  // TIMER
        case IRQ_JOYPAD: _PC.w = VECTOR_JOYPAD; break;  // JOYPAD
    }
}
