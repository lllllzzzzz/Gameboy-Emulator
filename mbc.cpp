#include <iostream>
#include <fstream>
#include <istream>
//#include <iterator>
#include "mbc.hpp"


GameBoy::MBC::MBC()
{
    // _MMU = GameBoy::Memory::Memory();
}

GameBoy::MBC::~MBC()
{
}

void GameBoy::MBC::detectMemoryBankController()
{
    // Cartridge type is located at address 0x0147 in cartridge header
    switch (_MMU->_cartMemory[0x0147]) {
        case 0x01: _MBC = MBCType::MBC1; break;  // MBC1
        case 0x02: _MBC = MBCType::MBC1; break;  // MBC1+RAM
        case 0x03: _MBC = MBCType::MBC1; break;  // MBC1+RAM+BATTERY
        case 0x05: _MBC = MBCType::MBC2; break;  // MBC2
        case 0x06: _MBC = MBCType::MBC2; break;  // MBC2+BATTERY
        default: break;
    }
}

void GameBoy::MBC::switchBanks(const word address, const byte data)
{
    // To enable RAM banking, the game attempts to write to ROM
    // at an address between 0x0000 and 0x2000, which is detected by the MMU.

    if (address >= 0x0000 && address < 0x2000) {
        if (_MBC == MBCType::MBC1 || _MBC == MBCType::MBC2) {
            enableRamBank(address, data);
        }
    }

    else if (address >= 0x2000 && address < 0x4000) {
        if (_MBC == MBCType::MBC1 || _MBC == MBCType::MBC2) {
            changeLoRamBank(data);
        }
    }

    else if (address >= 0x4000 && address < 0x6000) {
        if (_MBC == MBCType::MBC1) {
            if (_romBankingEnabled) {
                changeHiRomBank(data);
            } else {
                changeRamBank(data);
            }
        }
    }

    else if (address >= 0x6000 && address < 0x8000) {
        if (_MBC == MBCType::MBC1) {
            changeRomRamMode(data);
        }
    }
}

void GameBoy::MBC::enableRamBank(const word address, const byte data)
{
    static const int LOWER_NYBBLE       = 0xF;
    static const int ENABLE_RAM_BANKING = 0xA;

    // If using MBC2, RAM banking is only enabled if bit 4 of the address
    // is not set. MBC1 does not have this requirement.

    if (_MBC == MBCType::MBC2 && !isBitSet(address, 4)) {
        return;
    }

    // The value of the lower nybble (4 bits) of the data byte determines
    // whether RAM banking will be enabled: 0xA = enable, else disable.

    _ramBankingEnabled = ((data & LOWER_NYBBLE) == ENABLE_RAM_BANKING);
}

void GameBoy::MBC::changeLoRamBank(const byte data)
{
    static const int UPPER_3_BITS = 0xE0; // 0b11100000
    static const int LOWER_5_BITS = 0x1F; // 0b00011111
    static const int LOWER_NYBBLE = 0xF;  // 0b00001111

    // MBC2: Set the current rom bank equal to the lower nybble (4 bits)
    // of the data byte.

    if (_MBC == MBCType::MBC2) {
        _currentRomBank = data & LOWER_NYBBLE;

        if (_currentRomBank == 0) {
            _currentRomBank++;
        }

        return;
    }

    // MBC1: To change the current rom bank, we need to turn off the lower
    // 5 bits with a bitwise AND operation (mask with the upper 3 bits).
    // Then, bitwise OR the resulting value with the lower 5 bits of
    // the data byte.

    _currentRomBank &= UPPER_3_BITS;
    _currentRomBank |= (data & LOWER_5_BITS);

    if (_currentRomBank == 0) {
        _currentRomBank++;
    }
}

void GameBoy::MBC::changeHiRomBank(const byte data)
{
    static const int UPPER_3_BITS = 0xE0; // 0b11100000
    static const int LOWER_5_BITS = 0x1F; // 0b00011111

    // Set the current rom bank to the lower 5 bits of the data byte,
    // then bitwise OR the resulting value with the upper 3 bits of data.

    _currentRomBank = data & LOWER_5_BITS;
    _currentRomBank |= (data & UPPER_3_BITS);

    if (_currentRomBank == 0) {
        _currentRomBank++;
    }
}

void GameBoy::MBC::changeRamBank(const byte data)
{
    static const int LOWER_2_BITS = 0xE0; // 0b00000011

    // Cannot change RAM banks in MBC2
    if (_MBC == MBCType::MBC2) {
        return;
    } else {
        // The current ram bank is set to the lower 2 bits of data
        _currentRamBank = data & LOWER_2_BITS;
    }
}

void GameBoy::MBC::changeRomRamMode(const byte data)
{
    // If bit 0 of data byte is set, rom banking is enabled
    _romBankingEnabled = (isBitSet(data, 0));

    if (_romBankingEnabled) {
        _currentRamBank = 0;
    }
}
