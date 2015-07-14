#include <iostream>
#include <fstream>
#include <istream>
//#include <iterator>
#include "cpu.hpp"
#include "memory.hpp"
#include "emulator.hpp"
#include "lcd.hpp"
#include "timer.hpp"
#include "interrupts.hpp"
#include "mbc.hpp"


// GameBoy::CPU::CPU(GameBoy::LCD &LCD,
//                   GameBoy::Timer &Timer,
//                   GameBoy::Interrupts &Interrupts,
//                   GameBoy::MBC &MBC) :

//     LCD(LCD),
//     Timer(Timer),
//     Interrupts(Interrupts),
//     MBC(MBC)

GameBoy::CPU::CPU(GameBoy::Memory &MMU,
                  GameBoy::LCD &LCD,
                  GameBoy::Timer &Timer,
                  GameBoy::Interrupts &Interrupts,
                  GameBoy::MBC &MBC)
{
    _LCD        = &LCD;
    _Timer      = &Timer;
    _Interrupts = &Interrupts;
    _MBC        = &MBC;
    _MMU        = &MMU;
}

GameBoy::CPU::~CPU()
{
}

void GameBoy::CPU::reset()
{
    _numCycles = 0;
    
    _PC.w      = 0x0100;
    _SP.w      = 0xFFFE;
    _AF.w      = 0x01B0;
    _BC.w      = 0x0013;
    _DE.w      = 0x00D8;
    _HL.w      = 0x014D;
}

void GameBoy::CPU::updateCycles(const unsigned int numCycles)
{
    _numCycles += numCycles;
}

inline int GameBoy::CPU::getCycles() const
{
    return _numCycles;
}

void GameBoy::CPU::executeOpcode()
{
    // static const byte OPCODE = _MMU->_cartMemory[address];
    static const byte OPCODE = _MMU->fetchOpcode(_PC.w);

    switch (OPCODE) {
        // ld reg,reg
        case 0xCB: _PC.w++; executeExtendedOpcode(_PC.w); break;

        // case 0x7F: ld_aa(); break;
        // case 0x78: ld_ab(); break;
        // case 0x79: ld_ac(); break;
        // case 0x7B: ld_ad(); break;
        // case 0x7C: ld_ae(); break;
        // case 0x7D: ld_ah(); break;
        // case 0x7E: ld_al(); break;
        // case 0x47: ld_ahl(); break;
        // case 0x40: ld_ba(); break;
        // default: break;
    }

    _numCycles -= cpuCycles[OPCODE];
}

void GameBoy::CPU::executeExtendedOpcode(const word address)
{
    // static const byte OPCODE = _cartMemory[address];
    static const byte OPCODE = _MMU->fetchOpcode(address);

    switch (OPCODE) {
        default: break;
    }

    _numCycles -= cpuCycles[OPCODE];
}

void GameBoy::CPU::ld_aa()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_ab()
{
    _AF.b.h = _BC.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_ac()
{
    _AF.b.h = _BC.b.l;
    _PC.w++;
}

void GameBoy::CPU::ld_ad()
{
    _AF.b.h = _DE.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_ae()
{
    _AF.b.h = _DE.b.l;
    _PC.w++;
}

void GameBoy::CPU::ld_ah()
{
    _AF.b.h = _HL.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_al()
{
    _AF.b.h = _HL.b.l;
    _PC.w++;
}

void GameBoy::CPU::ld_ahl()
{
    //_AF.b.h = _romMemory[_HL.w];
    _AF.b.h = _MMU->readRom(_HL.w);
    _PC.w++;
}

void GameBoy::CPU::ld_ba()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_bb()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_bc()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_bd()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_be()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_bh()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_bl()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_bhl()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_ca()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_cb()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_cc()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_cd()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_ce()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_ch()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_cl()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_chl()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_da()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_db()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_dc()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_dd()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_de()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_dh()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_dl()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_dhl()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_ea()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_eb()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_ec()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_ed()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_ee()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_eh()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_el()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_ehl()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_ha()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_hb()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_hc()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_hd()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_he()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_hh()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_hl()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_hhl()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_la()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_lb()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_lc()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_ld()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_le()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_lh()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_ll()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_lhl()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_hla()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_hlb()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_hlc()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_hld()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_hle()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_hlh()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::CPU::ld_hll()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}



void GameBoy::CPU::ld_abc() // 0x0A
{
    // _AF.b.h = _romMemory[_BC.w];
    _AF.b.h = _MMU->readRom(_BC.w);
    _PC.w++;
}
void GameBoy::CPU::ld_ade() // 0x1A
{
    // _AF.b.h = _romMemory[_DE.w];
    _AF.b.h = _MMU->readRom(_DE.w);
    _PC.w++;
}
void GameBoy::CPU::ld_bca() // 0x02
{
    // _romMemory[_BC.w] = _AF.b.h;
    _MMU->writeRom(_BC.w, _AF.b.h);
    _PC.w++;
}
void GameBoy::CPU::ld_dea() // 0x12
{
    // _romMemory[_DE.w] = _AF.b.h;
    _MMU->writeRom(_DE.w, _AF.b.h);
    _PC.w++;
}
void GameBoy::CPU::ldd_ahl() // 0x3A
{
    // _AF.b.h = _romMemory[_HL.w];
    _AF.b.h = _MMU->readRom(_HL.w);
    _HL.w--;
    _PC.w++;
}
void GameBoy::CPU::ldd_hla() // 0x32
{
    // _romMemory[_HL.w] = _AF.b.h;
    _MMU->writeRom(_HL.w, _AF.b.h);
    _HL.w--;
    _PC.w++;
}
