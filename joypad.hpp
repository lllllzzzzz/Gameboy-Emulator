#ifndef JOYPAD_HPP_INCLUDED
#define JOYPAD_HPP_INCLUDED

#include <iostream>
#include <cstdlib>
//#include <array>
#include <string>
#include <vector>
#include <cstdint>
#include "emulator.hpp"

typedef std::uint8_t  byte;
typedef std::int8_t   signed_byte;
typedef std::int16_t  word;
typedef std::uint16_t signed_word;

namespace GameBoy
{

    class Joypad
    {
        public:
            Joypad();
            ~Joypad();

        protected:

        private:
            
    };
}


#endif // JOYPAD_HPP_INCLUDED
