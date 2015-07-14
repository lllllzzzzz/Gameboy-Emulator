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

            static const int CYCLES_PER_SCANLINE   = 456;
            static const int VBLANK_START_SCANLINE = 144;
            static const int VBLANK_END_SCANLINE   = 153;

            int    _scanlineCounter;        // Scanline counter

            Interrupts *_Interrupts;
            Memory     *_MMU;

            void drawScanline   ();
            bool isLcdEnabled   ();
            void setLcdMode     ();
    };
}


#endif // LCD_HPP_INCLUDED
