#ifndef CPU_HPP_INCLUDED
#define CPU_HPP_INCLUDED

#include <iostream>
#include <cstdlib>
//#include <array>
#include <string>
#include <vector>
#include <cstdint>

#define LITTLE_ENDIAN
// #define BIG_ENDIAN

typedef std::uint8_t  byte;
typedef std::int8_t   signed_byte;
typedef std::int16_t  word;
typedef std::uint16_t signed_word;

namespace GameBoy {
    class CPU;
    class Memory;
    class LCD;
    class Interrupts;
    class Timer;
    class MBC;

	class CPU {
	    public:
	        CPU  (Memory &_MMU, LCD &LCD, Timer &Timer, Interrupts &Interrupts, MBC &MBC);
	        ~CPU ();

            void        updateCycles  (const unsigned int numCycles);
            inline int  getCycles     () const;

            void        executeOpcode ();

	    protected:

	    private:
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

            enum flags {
                Z = 0x80,
                N = 0x40,
                H = 0x20,
                C = 0x10
            };

            typedef union {
                struct {
                    #ifdef LITTLE_ENDIAN
                    byte l, h;
                    #else
                    byte h, l;
                    #endif // LITTLE_ENDIAN
                } b;
                word w;
            } reg;

            reg     _PC;                    // Program counter
            reg     _SP;                    // Stack pointer
            reg     _AF;                    // AF register pair
            reg     _BC;                    // BC register pair
            reg     _DE;                    // DE register pair
            reg     _HL;                    // HL register pair
            bool    _isHalted;              // CPU halted flag
            bool    _isStopped;             // CPU stopped flag
            int     _numCycles;             // Number of cycles to execute

            // LCD         &_LCD;
            // Timer       &_Timer;
            // Interrupts  &_Interrupts;
            // MBC         &_MBC;

            LCD         *_LCD;
            Timer       *_Timer;
            Interrupts  *_Interrupts;
            MBC         *_MBC;
            Memory      *_MMU;

            void executeExtendedOpcode (const word address);
            void reset                 ();

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


#endif // CPU_HPP_INCLUDED
