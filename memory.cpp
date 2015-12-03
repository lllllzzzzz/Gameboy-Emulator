#include <iostream>
#include <fstream>
#include <istream>
//#include <iterator>
#include "memory.hpp"
#include "emulator.hpp"


GameBoy::Memory::Memory(Timer &Timer, Interrupts &Interrupts, MBC &MBC)
{
    _Timer      = &Timer;
    _Interrupts = &Interrupts;
    _MBC        = &MBC;
}

GameBoy::Memory::~Memory()
{
}

inline bool GameBoy::Memory::isBitSet(const byte data, const byte bit) const
{
    return (data & (1 << bit)) ? true : false;
}

inline byte GameBoy::Memory::readRom(const word address) const
{
    return _romMemory[address];
}

inline byte GameBoy::Memory::fetchOpcode(const word address) const
{
    return _cartMemory[address];
}

inline void GameBoy::Memory::writeRom(const word address, const byte data)
{
    _romMemory[address] = data;
}

inline byte GameBoy::Memory::readRom(const word address, const byte data) const
{
    return _romMemory[address];
}

void GameBoy::Memory::reset()
{
    _romMemory[0xFF05] = 0x00;    // TIMA
    _romMemory[0xFF06] = 0x00;    // TMA
    _romMemory[0xFF07] = 0x00;    // TAC
    _romMemory[0xFF10] = 0x80;    // NR10
    _romMemory[0xFF11] = 0xBF;    // NR11
    _romMemory[0xFF12] = 0xF3;    // NR12
    _romMemory[0xFF14] = 0xBF;    // NR14
    _romMemory[0xFF16] = 0x3F;    // NR21
    _romMemory[0xFF17] = 0x00;    // NR22
    _romMemory[0xFF19] = 0xBF;    // NR24
    _romMemory[0xFF1A] = 0x7F;    // NR30
    _romMemory[0xFF1B] = 0xFF;    // NR31
    _romMemory[0xFF1C] = 0x9F;    // NR32
    _romMemory[0xFF1E] = 0xBF;    // NR33
    _romMemory[0xFF20] = 0xFF;    // NR41
    _romMemory[0xFF21] = 0x00;    // NR42
    _romMemory[0xFF22] = 0x00;    // NR43
    _romMemory[0xFF23] = 0xBF;    // NR30
    _romMemory[0xFF24] = 0x77;    // NR50
    _romMemory[0xFF25] = 0xF3;    // NR51
    _romMemory[0xFF26] = 0xF1;    // NR52
    _romMemory[0xFF40] = 0x91;    // LCDC
    _romMemory[0xFF42] = 0x00;    // SCY
    _romMemory[0xFF43] = 0x00;    // SCX
    _romMemory[0xFF45] = 0x00;    // LYC
    _romMemory[0xFF47] = 0xFC;    // BGP
    _romMemory[0xFF48] = 0xFF;    // OBPO
    _romMemory[0xFF49] = 0xFF;    // OBP1
    _romMemory[0xFF4A] = 0x00;    // WY
    _romMemory[0xFF4B] = 0x00;    // WX
    _romMemory[0xFFFF] = 0x00;    // IE

    // detectMemoryBankController();
}

void GameBoy::Memory::writeMemory(const word address, const byte data)
{
    // If writing to ROM (0x0000 - 0x7FFF) switch banks
    if (address >= ROM_BANK_16KB_0 && address < VIDEO_RAM_8KB) {
        _MBC->switchBanks(address, data);
    }

    // If writing to echo RAM, also write to RAM
    else if (address >= INTERNAL_RAM_8KB_ECHO && address < SPRITE_ATTRIB_MEMORY) {
        _romMemory[address]                 = data;
        _romMemory[address - SIZE_RAM_BANK] = data;
        return;
    }

    // Do not allow writing to restricted memory (0xFEA0 - 0xFEFF)
    else if (address >= EMPTY_UNUSABLE_IO_1 && address < 0xFEFF) {
        std::cerr << "Illegal write to restricted memory at address "
                  << address << std::endl;
        return;
    }

    // If writing to TMC, set clock frequency
    else if (address == TMC) {
        byte currentClockFreq = _Timer->getClockFreq();
        _romMemory[TMC] = data;
        byte newClockFreq = _Timer->getClockFreq();

        if (currentClockFreq != newClockFreq) {
            _Timer->setClockFreq();
        }
    }

    // If writing to divider register, reset
    else if (address == DIV) { // 0xFF04
        _romMemory[DIV] = 0;
    }

    // If writing to current scanline, reset scanline
    else if (address == LY) { // 0xFF44
        _romMemory[LY] = 0;
    }

    else if (address == DMA) { // 0xFF46
        doDmaTransfer(data);
    }

    else {
        _romMemory[address] = data;
    }
}

byte GameBoy::Memory::readMemory(const word address)
{
    // Reading from ROM memory bank (0x4000 - 0x7FFF)?
    if (address >= SWITCHABLE_ROM_BANK_16KB && address < VIDEO_RAM_8KB) {
        word newAddress = address - SWITCHABLE_ROM_BANK_16KB;
        return newAddress + (_MBC->_currentRomBank * SWITCHABLE_ROM_BANK_16KB);
    }

    // Reading from RAM memory bank (0xA000 - 0xBFFF)?
    else if (address >= SWITCHABLE_RAM_BANK_8KB && address < INTERAL_RAM_8KB) {
        word newAddress = address - SWITCHABLE_RAM_BANK_8KB;
        return newAddress + (_MBC->_currentRamBank * SIZE_RAM_BANK);
    }

    else {
        return _romMemory[address];
    }
}

void GameBoy::Memory::doDmaTransfer(const byte data)
{
    const word ADDRESS = data << 8;

    for (int i = 0; i < 0xA0; i++) {
        writeMemory(SPRITE_ATTRIB_MEMORY + i, readMemory(ADDRESS + i));
    }
}

void GameBoy::Emulator::pushWord(const word data)
{
}
