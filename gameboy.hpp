#ifndef GAMEBOY_HPP_INCLUDED
#define GAMEBOY_HPP_INCLUDED

#include <iostream>
#include <cstdlib>
//#include <array>
#include <string>
//#include <vector>

#include "cpu.hpp"

#define CART_SIZE       0x200000    // 2MB cartridge size
#define MAX_ROM_SIZE    0x10000     // 64KB rom size
#define MAX_RAM_SIZE    0x8000      // 32KB rom size

#define FLAG_Z          0x80        // Zero flag
#define FLAG_N          0x40        // Negative flag
#define FLAG_H          0x20        // Half carry flag
#define FLAG_C          0x10        // Carry flag

class GameBoy
{
    public:
        GameBoy();
        ~GameBoy();

        bool initialize                 ();
        bool loadRom                    (const char* filename);
        void executeOpcode              (const word address);
        void emulateCycles              (const unsigned int numCycles);

        // CPU* gbCPU;

    protected:

    private:
        //byte _cartMemory[CART_SIZE];
        //byte* _carMemory = (byte*) malloc(CART_SIZE);
        //std::array<byte, CART_SIZE> _cartMemory;
        //std::vector<byte> _cartMemory;

        typedef union {
            struct {
                byte l, h;
            } b;
            word w;
        } reg;

        enum MBC {
            MBC1, MBC2
        };

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
        //bool    _romBanking;            
        bool    _isHalted;              // CPU halted flag
        bool    _isStopped;             // CPU stopped flag
        int     _numCycles;             // Number of cycles to execute

        void writeMemory                (const word address, const byte data);
        byte readMemory                 (const word address);
        void detectMemoryBankController ();
        void switchBanks                (const word address, const byte data);
        void enableRamBank              (const word address, const byte data);
        void changeLoRamBank            (const byte data);
        void changeHiRomBank            (const byte data);
        void changeRamBank              (const byte data);
        void changeRomRamMode           (const byte data);

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

#endif // GAMEBOY_HPP_INCLUDED
