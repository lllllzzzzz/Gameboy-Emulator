#ifndef INTERRUPTS_HPP_INCLUDED
#define INTERRUPTS_HPP_INCLUDED

#include <iostream>
#include <cstdlib>
//#include <array>
#include <string>
#include <vector>
#include <cstdint>
// #include "memory.hpp"

typedef std::uint8_t  byte;
typedef std::int8_t   signed_byte;
typedef std::int16_t  word;
typedef std::uint16_t signed_word;

namespace GameBoy
{
    class Emulator;
    class Memory;
    class CPU;
    class MBC;
    class Timer;

    class Interrupts
    {
        public:
            Interrupts();
            ~Interrupts();

            void doInterrupts();
            void generateInterrupt(const byte interruptVector);

        protected:

        private:
            enum io_ports {
                DIV  = 0xFF04,
                TIMA = 0xFF05,
                TMA  = 0xFF06,
                TMC  = 0xFF07,
                IF   = 0xFF0F,
                LCDC = 0xFF40,
                SCY  = 0xFF42,
                SCX  = 0xFF43,
                LY   = 0xFF44,
                LYC  = 0xFF45,
                DMA  = 0xFF45,
                BGP  = 0xFF47,
                OPB0 = 0xFF48,
                OPB1 = 0xFF49,
                WY   = 0xFF4A,
                WX   = 0xFF4B,
                P1   = 0xFF00,
                IE   = 0xFFFF
            };

            bool _interruptsEnabled; // Interrupt enable flag

            void serviceInterrupt(const byte interruptNum);

            Memory *_MMU;
    };
}


#endif // INTERRUPTS_HPP_INCLUDED
