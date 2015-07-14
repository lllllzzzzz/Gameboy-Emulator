#ifndef EMULATOR_HPP_INCLUDED
#define EMULATOR_HPP_INCLUDED

#include <iostream>
#include <cstdlib>
//#include <array>
#include <string>
#include <vector>
#include <cstdint>

#include "cpu.hpp"
#include "memory.hpp"
#include "lcd.hpp"
#include "interrupts.hpp"
#include "timer.hpp"
#include "mbc.hpp"


typedef std::uint8_t  byte;
typedef std::int8_t   signed_byte;
typedef std::int16_t  word;
typedef std::uint16_t signed_word;

namespace GameBoy
{
    class CPU;
    class Memory;
    class LCD;
    class Interrupts;
    class Timer;
    class MBC;

    class Emulator
    {
        public:
            Emulator          ();
            ~Emulator         ();

            bool initialize   ();
            bool loadRom      (const char* filename);
            void emulateCpu   ();
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

            byte*        _cartMemory;            // 2MB cartridge memory
            byte*        _romMemory;             // 64KB ROM memory
            byte*        _ramBanks;              // 32KB RAM banks

            byte         _dividerRegister;       // Divider register
            byte         _dividerCounter;        // Divider counter

            // CPU         &_CPU;
            // Memory      &_MMU;
            // LCD         &_LCD;
            // Interrupts  &_Interrupts;
            // Timer       &_Timer;

            CPU         *_CPU;
            Memory      *_MMU;
            LCD         *_LCD;
            Interrupts  *_Interrupts;
            Timer       *_Timer;
            MBC         *_MBC;

            inline byte setBit       (const byte data, const byte bit) const;
            inline byte resetBit     (const byte data, const byte bit) const;
            inline bool isBitSet     (const byte data, const byte bit) const;

            void emulateCycles       (const unsigned int numCycles);
            void changeRomRamMode    (const byte data);
            void doDividerRegister   (const int numCycles);
            void pushWord            (const word data);
            void doDmaTransfer       (const byte data);
    };
}


#endif // EMULATOR_HPP_INCLUDED
