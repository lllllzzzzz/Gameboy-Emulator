#ifndef INTERRUPTS_HPP_INCLUDED
#define INTERRUPTS_HPP_INCLUDED

#include <iostream>
#include <cstdlib>
//#include <array>
#include <string>
#include <vector>
#include <cstdint>
// #include "memory.hpp"

typedef std::uint8_t  byte;
typedef std::int8_t   signed_byte;
typedef std::int16_t  word;
typedef std::uint16_t signed_word;

namespace GameBoy
{
    class Emulator;
    class Memory;
    class CPU;
    class MBC;
    class Timer;

    class Interrupts
    {
        public:
            Interrupts  ();
            ~Interrupts ();

            void doInterrupts        ();
            void generateInterrupt   (const byte interruptVector);

        protected:

        private:
            bool    _interruptsEnabled;     // Interrupt enable flag

            void serviceInterrupt    (const byte interruptNum);
    };
}


#endif // INTERRUPTS_HPP_INCLUDED
