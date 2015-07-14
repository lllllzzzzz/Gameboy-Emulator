#include <iostream>
#include <fstream>
#include <istream>
//#include <iterator>
#include "emulator.hpp"


GameBoy::Emulator::Emulator()
{
    _Interrupts = new Interrupts();
    // _Timer      = new Timer();
    _MBC        = new MBC();
    _MMU        = new Memory(*_Timer, *_Interrupts, *_MBC);
    _LCD        = new LCD(*_Interrupts, *_MMU);
    _CPU        = new CPU(*_MMU, *_LCD, *_Timer, *_Interrupts, *_MBC);
}

GameBoy::Emulator::~Emulator()
{
    if (_cartMemory != nullptr) {
        delete[] _cartMemory;
    }

    if (_romMemory != nullptr) {
        delete[] _romMemory;
    }

    if (_ramBanks != nullptr) {
        delete[] _ramBanks;
    }
}

inline byte GameBoy::Emulator::setBit(const byte data, const byte bit) const
{
    return (bit >= 0 && bit <= 7) ? data | (1 << bit) : data;
}

inline byte GameBoy::Emulator::resetBit(const byte data, const byte bit) const
{
    return (bit >= 0 && bit <= 7) ? data & ~(1 << bit) : data;
}

inline bool GameBoy::Emulator::isBitSet(const byte data, const byte bit) const
{
    return data & (1 << bit);
}

bool GameBoy::Emulator::initialize()
{
    // // Reset cycle count
    // _numCycles = 0;

    // // Initialise registers
    // _PC.w = 0x0100;
    // _SP.w = 0xFFFE;
    // _AF.w = 0x01B0;
    // _BC.w = 0x0013;
    // _DE.w = 0x00D8;
    // _HL.w = 0x014D;

    // // Initialise ROM
    // _romMemory[0xFF05] = 0x00;    // TIMA
    // _romMemory[0xFF06] = 0x00;    // TMA
    // _romMemory[0xFF07] = 0x00;    // TAC
    // _romMemory[0xFF10] = 0x80;    // NR10
    // _romMemory[0xFF11] = 0xBF;    // NR11
    // _romMemory[0xFF12] = 0xF3;    // NR12
    // _romMemory[0xFF14] = 0xBF;    // NR14
    // _romMemory[0xFF16] = 0x3F;    // NR21
    // _romMemory[0xFF17] = 0x00;    // NR22
    // _romMemory[0xFF19] = 0xBF;    // NR24
    // _romMemory[0xFF1A] = 0x7F;    // NR30
    // _romMemory[0xFF1B] = 0xFF;    // NR31
    // _romMemory[0xFF1C] = 0x9F;    // NR32
    // _romMemory[0xFF1E] = 0xBF;    // NR33
    // _romMemory[0xFF20] = 0xFF;    // NR41
    // _romMemory[0xFF21] = 0x00;    // NR42
    // _romMemory[0xFF22] = 0x00;    // NR43
    // _romMemory[0xFF23] = 0xBF;    // NR30
    // _romMemory[0xFF24] = 0x77;    // NR50
    // _romMemory[0xFF25] = 0xF3;    // NR51
    // _romMemory[0xFF26] = 0xF1;    // NR52
    // _romMemory[0xFF40] = 0x91;    // LCDC
    // _romMemory[0xFF42] = 0x00;    // SCY
    // _romMemory[0xFF43] = 0x00;    // SCX
    // _romMemory[0xFF45] = 0x00;    // LYC
    // _romMemory[0xFF47] = 0xFC;    // BGP
    // _romMemory[0xFF48] = 0xFF;    // OBPO
    // _romMemory[0xFF49] = 0xFF;    // OBP1
    // _romMemory[0xFF4A] = 0x00;    // WY
    // _romMemory[0xFF4B] = 0x00;    // WX
    // _romMemory[0xFFFF] = 0x00;    // IE

    // detectMemoryBankController();

    // if (_MBC == MBC1) {
    //     std::cout << "MBC1\n";
    // } else if (_MBC == MBC2) {
    //     std::cout << "MBC2\n";
    // } else {
    //     //std::cout << "Error reading MBC " << _cartMemory[0x147] << "\n";
    //     printf("%X", _cartMemory[0x147]);
    //     // return false;
    // }

    // _currentRomBank = 1;
    // _currentRamBank = 0;

    return true;
}

bool GameBoy::Emulator::loadRom(const char *filename)
{
    std::ifstream rom;
    rom.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        // Open rom
        rom.open(filename, std::ifstream::binary);

        rom.seekg(0, rom.end);
        int length = rom.tellg();
        rom.seekg(0, rom.beg);

        // Read rom into cartridge memory
        rom.read(reinterpret_cast<char*>(_cartMemory), length);

       // _cartMemory((std::istreambuf_iterator<byte>(rom)),
       //              std::istreambuf_iterator<byte>());

    } catch (std::ifstream::failure e) {
        std::cerr << "Exception opening/reading rom "
                  << filename << std::endl;
        return false;
    }

    rom.close();
    return true;
}

void GameBoy::Emulator::emulateCpu()
{
    const int MAX_CYCLES = 69905;

    emulateCycles(MAX_CYCLES);

    _LCD->renderScreen();
}

void GameBoy::Emulator::doDividerRegister(const int numCycles)
{
    _dividerRegister += numCycles;
    if (_dividerRegister >= 255) {
        _dividerCounter = 0;
        _romMemory[DIV]++; // We must write to divider register directly
    }
}

void GameBoy::Emulator::emulateCycles(const unsigned int numCycles)
{
    // _numCycles += numCycles;
    _CPU->updateCycles(numCycles);

    while (_CPU->getCycles() > 0) {
        _CPU->executeOpcode();       // executeOpcode() decrements numCycles
        _Timer->updateTimers(numCycles);
        _LCD->updateScreen(numCycles);
        _Interrupts->doInterrupts();
    }

    _LCD->renderScreen();
}
