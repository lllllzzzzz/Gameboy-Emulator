#ifndef MBC_HPP_INCLUDED
#define MBC_HPP_INCLUDED

#include <iostream>
#include <cstdlib>
//#include <array>
#include <string>
#include <vector>
#include <cstdint>
#include "memory.hpp"

typedef std::uint8_t  byte;
typedef std::int8_t   signed_byte;
typedef std::int16_t  word;
typedef std::uint16_t signed_word;

namespace GameBoy
{
    class Emulator;
    class Memory;

    class MBC
    {
        public:
            MBC();
            ~MBC();

            void switchBanks                (const word address, const byte data);

            byte    _currentRomBank;        // Currently active ROM bank
            byte    _currentRamBank;        // Currently active RAM bank

        protected:

        private:
            enum class MBCType {
                MBC1,
                MBC2,
                MC3,
                MBC4,
                MBC5
            };

            MBCType _MBC;                   // Memory bank controller
            bool    _romBankingEnabled;     // ROM banking enabled flag
            bool    _ramBankingEnabled;     // RAM banking enabled flag

            Memory* _MMU;

            inline byte setBit              (const byte data, const byte bit) const;
            inline byte resetBit            (const byte data, const byte bit) const;
            inline bool isBitSet            (const byte data, const byte bit) const;
            void detectMemoryBankController ();
            void enableRamBank              (const word address, const byte data);
            void changeLoRamBank            (const byte data);
            void changeHiRomBank            (const byte data);
            void changeRamBank              (const byte data);
            void changeRomRamMode           (const byte data);
    };
}


#endif // MBC_HPP_INCLUDED
