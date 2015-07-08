#ifndef GAMEBOY_HPP_INCLUDED
#define GAMEBOY_HPP_INCLUDED

#include <iostream>
#include <cstdlib>
//#include <array>
#include <string>
#include <vector>
#include <cstdint>


typedef std::uint8_t  byte;
typedef std::int8_t   signed_byte;
typedef std::int16_t  word;
typedef std::uint16_t signed_word;

namespace GameBoy
{
    class Emulator
    {
        public:
            Emulator();
            ~Emulator();

            bool initialize                 ();
            bool loadRom                    (const char* filename);
            void emulateCpu                 ();

            // CPU* gbCPU;
        protected:

        private:
            //byte _cartMemory[CART_SIZE];
            //byte* _carMemory = (byte*) malloc(CART_SIZE);
            //std::array<byte, CART_SIZE> _cartMemory;
            // std::vector<byte> _cartMemory;
            // std::vector<byte> _romMemory;
            // std::vector<byte> _ramMemory;

            typedef union {
                struct {
                    byte l, h;
                } b;
                word w;
            } reg;

            enum MBC {
                MBC1,
                MBC2,
                MC3,
                MBC4,
                MBC5
            };

            enum memorySizes {
                SIZE_CART     = 0x200000,
                SIZE_MAX_ROM  = 0x10000,
                SIZE_MAX_RAM  = 0x8000
            };

            enum flags {
                Z = 0x80,
                N = 0x40,
                H = 0x20,
                C = 0x10
            };

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

            enum registers {
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
                IE   = 0xFFFF
            };

            static const int CYCLES_PER_SCANLINE   = 456;
            static const int VBLANK_START_SCANLINE = 144;
            static const int VBLANK_END_SCANLINE   = 153;

            reg     _PC;                    // Program counter
            reg     _SP;                    // Stack pointer
            reg     _AF;                    // AF register pair
            reg     _BC;                    // BC register pair
            reg     _DE;                    // DE register pair
            reg     _HL;                    // HL register pair
           byte*   _cartMemory;            // 2MB cartridge memory
           byte*   _romMemory;             // 64KB ROM memory
           byte*   _ramBanks;              // 32KB RAM banks
            MBC     _MBC;                   // Memory bank controller
            byte    _currentRomBank;        // Currently active ROM bank
            byte    _currentRamBank;        // Currently active RAM bank
            bool    _interruptsEnabled;     // Interrupt enable flag
            bool    _romBankingEnabled;     // ROM banking enabled flag
            bool    _ramBankingEnabled;     // RAM banking enabled flag
            word    _timerCounter;
            byte    _dividerRegister;
            byte    _dividerCounter;
            int    _scanlineCounter;
            //bool    _romBanking;
            bool    _isHalted;              // CPU halted flag
            bool    _isStopped;             // CPU stopped flag
            int     _numCycles;             // Number of cycles to execute

            inline byte setBit              (const byte data, const byte bit) const;
            inline byte resetBit            (const byte data, const byte bit) const;
            inline bool isBitSet            (const byte data, const byte bit) const;

            void emulateCycles              (const unsigned int numCycles);
            void executeOpcode              (const word address);
            void executeExtendedOpcode      (const word address);
            void writeMemory                (const word address, const byte data);
            byte readMemory                 (const word address);
            void detectMemoryBankController ();
            void switchBanks                (const word address, const byte data);
            void enableRamBank              (const word address, const byte data);
            void changeLoRamBank            (const byte data);
            void changeHiRomBank            (const byte data);
            void changeRamBank              (const byte data);
            void changeRomRamMode           (const byte data);
            void updateTimers               (const int numCycles);
            byte getClockFreq               ();
            void setClockFreq               ();
            void doDividerRegister          (const int numCycles);
            bool isClockEnabled             ();
            void generateInterrupt          (const byte interruptVector);
            void doInterrupts               ();
            void serviceInterrupt           (const byte interruptNum);
            void pushWord                   (const word data);
            void doDmaTransfer              (const byte data);
            void updateScreen               (const int numCycles);
            void renderScreen               ();
            void drawScanline               ();
            bool isLcdEnabled               ();
            void setLcdMode                 ();

            // LD reg,reg
            void ld_aa();
            void ld_ab();
            void ld_ac();
            void ld_ad();
            void ld_ae();
            void ld_ah();
            void ld_al();
            void ld_ahl();
            void ld_ba();
            void ld_bb();
            void ld_bc();
            void ld_bd();
            void ld_be();
            void ld_bh();
            void ld_bl();
            void ld_bhl();
            void ld_ca();
            void ld_cb();
            void ld_cc();
            void ld_cd();
            void ld_ce();
            void ld_ch();
            void ld_cl();
            void ld_chl();
            void ld_da();
            void ld_db();
            void ld_dc();
            void ld_dd();
            void ld_de();
            void ld_dh();
            void ld_dl();
            void ld_dhl();
            void ld_ea();
            void ld_eb();
            void ld_ec();
            void ld_ed();
            void ld_ee();
            void ld_eh();
            void ld_el();
            void ld_ehl();
            void ld_ha();
            void ld_hb();
            void ld_hc();
            void ld_hd();
            void ld_he();
            void ld_hh();
            void ld_hl();
            void ld_hhl();
            void ld_la();
            void ld_lb();
            void ld_lc();
            void ld_ld();
            void ld_le();
            void ld_lh();
            void ld_ll();
            void ld_lhl();
            void ld_hla();
            void ld_hlb();
            void ld_hlc();
            void ld_hld();
            void ld_hle();
            void ld_hlh();
            void ld_hll();

            void ld_abc();
            void ld_ade();
            void ld_bca();
            void ld_dea();
            void ldd_ahl();
            void ldd_hla();
    };
}


#endif // GAMEBOY_HPP_INCLUDED
