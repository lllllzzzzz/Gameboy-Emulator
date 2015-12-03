#ifndef TIMER_HPP_INCLUDED
#define TIMER_HPP_INCLUDED

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
    class Interrupts;
    class LCD;

    class Timer
    {
        public:
            Timer  (Interrupts &Interrupts, Memory &MMU);
            ~Timer ();

            void updateTimers   (const int numCycles);
            byte getClockFreq   ();
            void setClockFreq   ();

            void doDividerRegister(const int numCycles);

        protected:

        private:
            static const int TIMA_OVERFLOW = 255;

            word    _timerCounter;          // Timer counter

            bool isClockEnabled ();
    };
}


#endif // TIMER_HPP_INCLUDED
