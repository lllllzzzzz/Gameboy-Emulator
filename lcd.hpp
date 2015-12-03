#ifndef LCD_HPP_INCLUDED
#define LCD_HPP_INCLUDED

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
    class Interrupts;

    class LCD
    {
        public:
            LCD  (Interrupts &Interrupts, Memory &MMU);
            ~LCD ();

            void renderScreen   ();
            void updateScreen   (const int numCycles);

        protected:

        private:
            enum interrupts {
                VECTOR_VBLANK = 0x40,
                VECTOR_LCDC   = 0x48,
                VECTOR_TIMER  = 0x50,
                VECTOR_JOYPAD = 0x10,
                IRQ_VBLANK    = 0,
                IRQ_LCDC      = 1,
                IRQ_TIMER     = 2,
                IRQ_JOYPAD    = 4
            };

            enum io_ports {
                DIV        = 0xFF04,
                TIMA       = 0xFF05,
                TMA        = 0xFF06,
                TMC        = 0xFF07,
                IF         = 0xFF0F,
                LCDC       = 0xFF40,
                LCD_STATUS = 0xFF41,
                SCY        = 0xFF42,
                SCX        = 0xFF43,
                LY         = 0xFF44,
                LYC        = 0xFF45,
                DMA        = 0xFF45,
                BGP        = 0xFF47,
                OPB0       = 0xFF48,
                OPB1       = 0xFF49,
                WY         = 0xFF4A,
                WX         = 0xFF4B,
                P1         = 0xFF00,
                IE         = 0xFFFF
            };

            static const int CYCLES_PER_SCANLINE   = 456;
            static const int VBLANK_START_SCANLINE = 144;
            static const int VBLANK_END_SCANLINE   = 153;

            int    _scanlineCounter;        // Scanline counter

            Interrupts *_Interrupts;
            Memory     *_MMU;
            MBC        *_MBC;

            void drawScanline   ();
            bool isLcdEnabled   ();
            void setLcdMode     ();
    };
}


#endif // LCD_HPP_INCLUDED
