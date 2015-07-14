#ifndef MEMORY_HPP_INCLUDED
#define MEMORY_HPP_INCLUDED

#include <iostream>
#include <cstdlib>
//#include <array>
#include <string>
#include <vector>
#include <cstdint>
#include "emulator.hpp"

typedef std::uint8_t  byte;
typedef std::int8_t   signed_byte;
typedef std::int16_t  word;
typedef std::uint16_t signed_word;

namespace GameBoy
{
    class Emulator;
    class Timer;
    class Interrupts;
    class MBC;

    class Memory
    {
        public:
            Memory(Timer &Timer, Interrupts &Interrupts, MBC &MBC);
            ~Memory();

            inline byte fetchOpcode (const word address) const;
            inline byte readRom     (const word address) const;
            inline void writeRom    (const word address, const byte data);
            inline byte readRom     (const word address, const byte data) const;

            byte readMemory         (const word address);

            byte*   _cartMemory;    // 2MB cartridge memory

        protected:

        private:
            //byte _cartMemory[CART_SIZE];
            //byte* _carMemory = (byte*) malloc(CART_SIZE);
            //std::array<byte, CART_SIZE> _cartMemory;
            // std::vector<byte> _cartMemory;
            // std::vector<byte> _romMemory;
            // std::vector<byte> _ramMemory;

            enum memorySizes {
                SIZE_CART     = 0x200000,
                SIZE_MAX_ROM  = 0x10000,
                SIZE_MAX_RAM  = 0x8000,
                SIZE_RAM_BANK = 0x2000
            };

            enum memoryMap {
                INTERNAL_RAM             = 0xFF80,
                EMPTY_UNUSABLE_IO_2      = 0xFF4C,
                IO_PORTS                 = 0xFF00,
                EMPTY_UNUSABLE_IO_1      = 0xFEA0,
                SPRITE_ATTRIB_MEMORY     = 0xFE00,
                INTERNAL_RAM_8KB_ECHO    = 0xE000,
                INTERAL_RAM_8KB          = 0xC000,
                SWITCHABLE_RAM_BANK_8KB  = 0xA000,
                VIDEO_RAM_8KB            = 0x8000,
                SWITCHABLE_ROM_BANK_16KB = 0x4000,
                ROM_BANK_16KB_0          = 0x0000
            };

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

            byte*   _romMemory;     // 64KB ROM memory
            byte*   _ramBanks;      // 32KB RAM banks

            Timer      *_Timer;
            Interrupts *_Interrupts;
            MBC        *_MBC;

            inline byte setBit      (const byte data, const byte bit) const;
            inline byte resetBit    (const byte data, const byte bit) const;
            inline bool isBitSet    (const byte data, const byte bit) const;

            void reset              ();
            void writeMemory        (const word address, const byte data);
            void pushWord           (const word data);
            void doDmaTransfer      (const byte data);
    };
}


#endif // MEMORY_HPP_INCLUDED
