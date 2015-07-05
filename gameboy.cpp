#include <iostream>
#include <fstream>
#include <istream>
//#include <iterator>
#include "gameboy.hpp"

#define TIMA    0xFF05
#define TMA     0xFF06
#define TMC     0xFF07

// Z80 mnemonics, incomplete
const char *Z80_mnemonics[0x100] =
{/*   0          1          2          3             4          5           6          7          8          9           A          B           C          D          E          F       */
 /*0*/"NOP",     "LD BC",   "LD BC",   "INC BC",     "INC B",   "DEC B",    "LD B",    "RLCA",    "Illegal", "ADD HL,BC", "LDAX B",  "DCX B",    "INC C",   "DEC C",   "MVI C",   "RRCA",
 /*1*/"Illegal", "LD DE",   "STAX D",  "INX D",      "INC D",   "DEC D",    "MVI D",   "RLA",     "Illegal", "ADD HL,DE", "LDAX D",  "DCX D",    "INC E",   "DEC E",   "MVI E",   "RRA",
 /*2*/"Illegal", "LD HL",   "LD HL",   "INX H",      "INC H",   "DEC H",    "MVI H",   "DAA",     "Illegal", "ADD HL,HL", "LD HL",   "DCX H",    "INC L",   "DEC L",   "MVI L",   "CPL",
 /*3*/"Illegal", "LD SP",   "STA",     "Illegal",    "INC M",   "DEC M",    "MVI M",   "SCF",     "Illegal", "ADD HL,SP", "LDA",     "DCX SP",   "INC A",   "DEC A",   "MVI A",   "CCF",
 /*4*/"LD B,B",  "LD B,C",  "LD B,D",  "LD B,E",     "LD B,H",  "LD B,L",   "LD B,M",  "LD B,A",  "LD C,B",  "LD C,C",    "LD C,D",  "LD C,E",   "LD C,H",  "LD C,L",  "LD C,M",  "LD C,A",
 /*5*/"LD D,B",  "LD D,C",  "LD D,D",  "LD D,E",     "LD D,H",  "LD D,L",   "LD D,M",  "LD D,A",  "LD E,B",  "LD E,C",    "LD E,D",  "LD E,E",   "LD E,H",  "LD E,L",  "LD E,M",  "LD E,A",
 /*6*/"LD H,B",  "LD H,C",  "LD H,D",  "LD H,E",     "LD H,H",  "LD H,L",   "LD H,M",  "LD H,A",  "LD L,B",  "LD L,C",    "LD L,D",  "LD L,E",   "LD L,H",  "LD L,L",  "LD L,M",  "LD L,A",
 /*7*/"LD M,B",  "LD M,C",  "LD M,D",  "LD M,E",     "LD M,H",  "LD M,L",   "HLT",     "LD M,A",  "LD A,B",  "LD A,C",    "LD A,D",  "LD A,E",   "LD A,H",  "LD A,L",  "LD A,M",  "LD A,A",
 /*8*/"ADD A,B", "ADD A,C", "ADD A,D", "ADD A,E",    "ADD A,H", "ADD A,L",  "ADD A,M", "ADD A,A", "ADC A,B", "ADC A,C",   "ADC A,D", "ADC A,E",  "ADC A,H", "ADC A,L", "ADC A,M", "ADC A",
 /*9*/"SUB B",   "SUB C",   "SUB D",   "SUB E",      "SUB H",   "SUB L",    "SUB M",   "SBB A",   "SBB B",   "SBB C",     "SBB D",   "SBB E",    "SBB H",   "SBB L",   "SBB M",   "SBB A",
 /*A*/"AND B",   "AND C",   "AND D",   "AND E",      "AND H",   "AND L",    "AND M",   "AND A",   "XOR B",   "XOR C",     "XOR D",   "XOR E",    "XOR H",   "XOR L",   "XOR M",   "XOR A",
 /*B*/"OR B",    "OR C",    "OR D",    "OR E",       "OR H",    "OR L",     "OR M",    "OR A",    "CP B",    "CP C",      "CP D",    "CP E",     "CP H",    "CP L",    "CP M",    "CP A",
 /*C*/"RNZ",     "POP B",   "JP NZ",   "JP",         "CALL NZ", "PUSHB",    "ADI",     "RST 0",   "RZ",      "RET",       "JP Z",    "Illegal",  "CALL Z",  "CALL",    "ACI",     "RST 1",
 /*D*/"RNC",     "POP D",   "JP NC",   "OUT",        "CALL NC", "PUSH D",   "SUI",     "RST 2",   "RC",      "Illegal",   "JP C",    "IN",       "CALL C",  "Illegal", "SBI",     "RST 3",
 /*E*/"CPO",     "POP H",   "JP PO",   "EX (SP),HL", "CALL PO", "PUSH H",   "AND",     "RST 4",   "CPE",     "JP (HL)",   "JP PE",   "EX DE,HL", "CALL PE", "Illegal", "XRI",     "RST 5",
 /*F*/"CP",      "POPPSW",  "JP P",    "DI",         "CALL P",  "PUSH PSW", "OR",      "RST 6",   "CM",      "LD SP,HL",  "JP M",    "EI",       "CALL M",  "Illegal", "CP",      "RST 7"
};

// CPU cycles for instructions, incomplete?
const byte cpuCycles[0x100] =
{/*   0   1   2   3   4   5   6   7   8   9   A   B   C   D   E   F*/
 /*0*/4,  10, 7,  5,  5,  5,  7,  4,  0,  10, 7,  5,  5,  5,  7,  4,
 /*1*/0,  10, 7,  5,  5,  5,  7,  4,  0,  10, 7,  5,  5,  5,  7,  4,
 /*2*/0,  10, 16, 5,  5,  5,  7,  4,  0,  10, 7,  5,  5,  5,  7,  11,
 /*3*/0,  10, 13, 0,  5,  5,  7,  0,  0,  10, 13, 5,  5,  5,  7,  0,
 /*4*/5,  5,  5,  5,  5,  5,  5,  4,  5,  5,  5,  5,  5,  5,  5,  11,
 /*5*/5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
 /*6*/5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,  5,
 /*7*/5,  5,  5,  5,  5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  5,  5,
 /*8*/4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
 /*9*/4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
 /*A*/4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
 /*B*/4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,
 /*C*/5,  10, 10, 10, 11, 11, 0,  11, 11, 10, 0,  10, 11, 17, 7,  11,
 /*D*/5,  10, 10, 10, 11, 11, 7,  11, 11, 0,  10, 10, 11, 0,  7,  11,
 /*E*/11, 10, 10, 18, 11, 11, 7,  11, 11, 5,  10, 4,  11, 0,  7,  11,
 /*F*/11, 10, 10, 4,  11, 11, 7,  11, 11, 5,  10, 4,  11, 0,  7,  11
};

GameBoy::GameBoy()
{
    _cartMemory = new byte[CART_SIZE]();
    _romMemory  = new byte[MAX_ROM_SIZE]();
    _ramBanks   = new byte[MAX_RAM_SIZE]();
}

GameBoy::~GameBoy()
{
    delete[] _cartMemory;
    delete[] _romMemory;
    delete[] _ramBanks;
}

inline byte GameBoy::setBit(const byte data, const byte bit)
{
    return (bit >= 0 && bit <= 7) ? data | (1 << bit) : data;
}

inline byte GameBoy::resetBit(const byte data, const byte bit)
{
    return (bit >= 0 && bit <= 7) ? data & ~(1 << bit) : data;
}

bool GameBoy::initialize()
{
    // Reset cycle count
    _numCycles = 0;

    // Initialise registers
    _PC.w = 0x0100;
    _SP.w = 0xFFFE;
    _AF.w = 0x01B0;
    _BC.w = 0x0013;
    _DE.w = 0x00D8;
    _HL.w = 0x014D;

    // Initialise ROM
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

    detectMemoryBankController();

    if (_MBC == MBC1) {
        std::cout << "MBC1\n";
    } else if (_MBC == MBC2) {
        std::cout << "MBC2\n";
    } else {
        //std::cout << "Error reading MBC " << _cartMemory[0x147] << "\n";
        printf("%X", _cartMemory[0x147]);
        // return false;
    }

    _currentRomBank = 1;
    _currentRamBank = 0;

    return true;
}

void GameBoy::detectMemoryBankController()
{
    // Cartridge type is located at address 0x0147 in cartridge header
    switch (_cartMemory[0x0147]) {
        case 0x01: _MBC = MBC1; break;  // MBC1
        case 0x02: _MBC = MBC1; break;  // MBC1+RAM
        case 0x03: _MBC = MBC1; break;  // MBC1+RAM+BATTERY
        case 0x05: _MBC = MBC2; break;  // MBC2
        case 0x06: _MBC = MBC2; break;  // MBC2+BATTERY
        default: break;
    }
}

bool GameBoy::loadRom(const char *filename)
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

void GameBoy::writeMemory(const word address, const byte data)
{
    // If writing to ROM (0x0000 - 0x7FFF) switch banks
    if (address < 0x8000) {
        /*std::cerr << "Illegal write to ROM at address "
                  << address << std::endl;
        return;*/
        switchBanks(address, data);
    }

    // If writing to echo RAM, also write to RAM (echo RAM)
    else if (address >= 0xE000 && address < 0xFE00) {
        _romMemory[address]          = data;
        _romMemory[address - 0x2000] = data;
        return;
    }

    // Do not allow writing to restricted memory (0xFEA0 - 0xFEFF)
    else if (address >= 0xFEA0 && address < 0xFEFF) {
        std::cerr << "Illegal write to restricted memory at address "
                  << address << std::endl;
        return;
    }

    // If writing to TMC, set clock frequency
    else if (address == TMC) {
        byte currentClockFreq = getClockFreq();
        _romMemory[TMC] = data;
        byte newClockFreq = getClockFreq();

        if (currentClockFreq != newClockFreq) {
            setClockFreq();
        }
    }

    // If writing to divider register, reset
    else if (address == 0xFF04) {
        _romMemory[0xFF04] = 0;
    }

    else if (address == 0xFF46) {
        doDmaTransfer(data);
    }

    else {
        _romMemory[address] = data;
    }
}

byte GameBoy::readMemory(const word address)
{
    // Reading from ROM memory bank (0x4000 - 0x7FFF)?
    if (address >= 0x4000 && address <= 0x7FFF) {
        word newAddress = address - 0x4000;
        return newAddress + (_currentRomBank * 0x4000);
    }

    // Reading from RAM memory bank (0xA000 - 0xBFFF)?
    else if (address >= 0xA000 && address <= 0xBFFF) {
        word newAddress = address - 0xA000;
        return newAddress + (_currentRamBank * 0x2000);
    }

    else {
        return _romMemory[address];
    }
}

void GameBoy::switchBanks(const word address, const byte data)
{
    if (address < 0x2000) {
        if (_MBC == MBC1 || _MBC == MBC2) {
            enableRamBank(address, data);
        }
    } else if (address >= 0x2000 && address < 0x4000) {
        if (_MBC == MBC1 || _MBC == MBC2) {
            changeLoRamBank(data);
        }
    } else if (address >= 0x4000 && address < 0x6000) {
        if (_MBC == MBC1) {
            (_romBankingEnabled) ? changeHiRomBank(data) : changeRamBank(data);
        }
    } else if (address >= 0x6000 && address < 0x8000) {
        if (_MBC == MBC1) {
            changeRomRamMode(data);
        }
    }
}

void GameBoy::enableRamBank(const word address, const byte data)
{
    // If MBC2 bit 4 of address must be 0, else return
    if ((_MBC == MBC2) && ((address & 0x8) == 1)) {
        return;
    // If lower nybble of data is 0xA, enable RAM
    } else if ((data & 0xF) == 0xA) {
        _ramBankingEnabled = true;
    } else if ((data & 0xF) == 0x0) {
        _ramBankingEnabled = false;
    }
}

void GameBoy::changeLoRamBank(const byte data)
{
    if (_MBC == MBC2) {
        _currentRomBank = data & 0xF;
        if (_currentRomBank == 0) {
            _currentRomBank++;
            return;
        }
    }

    _currentRomBank &= 224;
    _currentRomBank |= (data & 31);

    if (_currentRomBank == 0) {
        _currentRomBank++;
    }
}

void GameBoy::changeHiRomBank(const byte data)
{
    _currentRomBank = data & 31;
    _currentRomBank |= (data & 224);

    if (_currentRomBank == 0) {
        _currentRomBank++;
    }
}

void GameBoy::changeRamBank(const byte data)
{
    // Cannot change RAM banks in MBC2
    if (_MBC == MBC2) {
        return;
    } else {
        _currentRamBank = data & 0x3;
    }
}

void GameBoy::changeRomRamMode(const byte data)
{
    // If bit 0 of data byte is set, rom banking is enabled
    _romBankingEnabled = ((data & 0x1) == 0) ? true : false;

    if (_romBankingEnabled) {
        _currentRamBank = 0;
    }
}

void GameBoy::updateTimers(const int numCycles)
{
    doDividerRegister(numCycles);

    if (isClockEnabled()) {
        _timerCounter -= numCycles;
    }

    if (_timerCounter <= 0) {
        setClockFreq();
        if (readMemory(TIMA) == 255) {
            writeMemory(TIMA, readMemory(TMA));
            generateInterrupt(2);
        } else {
            writeMemory(TIMA, readMemory(TIMA) + 1);
        }
    }
}

byte GameBoy::getClockFreq()
{
    // Bits 1 and 0 of TMC store current clock frequency
    return readMemory(TMC) & 0x3;
}

void GameBoy::setClockFreq()
{
    byte clockFreq = getClockFreq();
    switch (clockFreq) {
        case 0x00: _timerCounter = 1024; break; // frequency = 4096
        case 0x01: _timerCounter = 16;   break; // frequency = 262144
        case 0x10: _timerCounter = 64;   break; // frequency = 65536
        case 0x11: _timerCounter = 256;  break; // frequency = 16382
    }
}

void GameBoy::doDividerRegister(const int numCycles)
{
    _dividerRegister += numCycles;
    if (_dividerRegister >= 255) {
        _dividerCounter = 0;
        _romMemory[0xFF04]++; // We must write to divider register directly
    }
}

bool GameBoy::isClockEnabled()
{
    // TMC bit 2 is clock enable flag (1 == on, 0 == off)
    return ((readMemory(TMC) & 0x4) != 0) ? true : false;
}

void GameBoy::generateInterrupt(const byte interruptVector)
{
    byte interruptRequest = readMemory(0xFF0F);
    //interruptRequest |= interruptVector;
    interruptRequest = setBit(interruptRequest, interruptVector);
    writeMemory(0xFF0F, interruptVector);
}

void GameBoy::doInterrupts()
{
    if (_interruptsEnabled) {
        byte interruptRequest = readMemory(0xFF0F);
        byte interruptEnable  = readMemory(0xFFFF);

        if (interruptRequest > 0) {
            for (int i = 0; i < 5; i++) {
                if ((interruptRequest & i) && (interruptEnable & i)) {
                        serviceInterrupt(i);
                }
            }
        }
    }
}

void GameBoy::serviceInterrupt(const byte interruptNum)
{
    _interruptsEnabled = false;
    byte interruptRequest = readMemory(0xFF0F);
    //interruptRequest &= ~interruptNum;
    interruptRequest = resetBit(interruptRequest, interruptNum);
    writeMemory(0xFF0F, interruptRequest);

    pushWord(_PC.w);

    switch (interruptNum) {
        case 0x0: _PC.w = 0x40; break;  // VBLANK
        case 0x1: _PC.w = 0x48; break;  // LCD
        case 0x2: _PC.w = 0x50; break;  // TIMER
        case 0x4: _PC.w = 0x60; break;  // JOYPAD
    }
}

void GameBoy::doDmaTransfer(const byte data)
{
    const word ADDRESS = data << 8;
    for (int i = 0; i < 0xA0; i++) {
        writeMemory(0xFE00 + i, readMemory(ADDRESS + i));
    }
}

void GameBoy::pushWord(const word data)
{

}

void GameBoy::executeOpcode(const word address)
{
    byte opcode = _cartMemory[address];

    switch (opcode) {
        // ld reg,reg
        case 0xCB: executeExtendedOpcode(address); break;

        case 0x7F: ld_aa(); break;
        case 0x78: ld_ab(); break;
        case 0x79: ld_ac(); break;
        case 0x7B: ld_ad(); break;
        case 0x7C: ld_ae(); break;
        case 0x7D: ld_ah(); break;
        case 0x7E: ld_al(); break;
        case 0x47: ld_ahl(); break;
        case 0x40: ld_ba(); break;
        default: break;
    }

    _numCycles -= cpuCycles[opcode];
}

void GameBoy::executeExtendedOpcode(const word address)
{

}

void GameBoy::emulateCycles(const unsigned int numCycles)
{
    _numCycles += numCycles;

    while (_numCycles > 0) {
        executeOpcode(_PC.w);
        updateTimers(numCycles);
    }
}

void GameBoy::ld_aa()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_ab()
{
    _AF.b.h = _BC.b.h;
    _PC.w++;
}

void GameBoy::ld_ac()
{
    _AF.b.h = _BC.b.l;
    _PC.w++;
}

void GameBoy::ld_ad()
{
    _AF.b.h = _DE.b.h;
    _PC.w++;
}

void GameBoy::ld_ae()
{
    _AF.b.h = _DE.b.l;
    _PC.w++;
}

void GameBoy::ld_ah()
{
    _AF.b.h = _HL.b.h;
    _PC.w++;
}

void GameBoy::ld_al()
{
    _AF.b.h = _HL.b.l;
    _PC.w++;
}

void GameBoy::ld_ahl()
{
    _AF.b.h = _romMemory[_HL.w];
    _PC.w++;
}

void GameBoy::ld_ba()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_bb()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_bc()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_bd()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_be()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_bh()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_bl()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_bhl()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_ca()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_cb()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_cc()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_cd()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_ce()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_ch()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_cl()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_chl()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_da()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_db()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_dc()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_dd()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_de()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_dh()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_dl()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_dhl()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_ea()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_eb()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_ec()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_ed()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_ee()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_eh()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_el()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_ehl()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_ha()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_hb()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_hc()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_hd()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_he()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_hh()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_hl()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_hhl()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_la()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_lb()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_lc()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_ld()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_le()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_lh()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_ll()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_lhl()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_hla()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_hlb()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_hlc()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_hld()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_hle()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_hlh()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}

void GameBoy::ld_hll()
{
    _AF.b.h = _AF.b.h;
    _PC.w++;
}


void GameBoy::ld_abc() // 0x0A
{
    _AF.b.h = _romMemory[_BC.w];
    _PC.w++;
}
void GameBoy::ld_ade() // 0x1A
{
    _AF.b.h = _romMemory[_DE.w];
    _PC.w++;
}
void GameBoy::ld_bca() // 0x02
{
    _romMemory[_BC.w] = _AF.b.h;
    _PC.w++;
}
void GameBoy::ld_dea() // 0x12
{
    _romMemory[_DE.w] = _AF.b.h;
    _PC.w++;
}
void GameBoy::ldd_ahl() // 0x3A
{
    _AF.b.h = _romMemory[_HL.w];
    _HL.w--;
    _PC.w++;
}
void GameBoy::ldd_hla() // 0x32
{
    _romMemory[_HL.w] = _AF.b.h;
    _HL.w--;
    _PC.w++;
}
