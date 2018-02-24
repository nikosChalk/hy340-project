
//
// This is a standard library support code to the chapters of the book
// "Programming -- Principles and Practice Using C++" by Bjarne Stroustrup
//

#ifndef STD_LIB_FACILITIES_GUARD
#define STD_LIB_FACILITIES_GUARD

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <sstream>
#include <vector>

//------------------------------------------------------------------------------

// The call to keep_window_open() is needed on some Windows machines to prevent
// them from closing the window before you have a chance to read the output. 
inline void keep_window_open()
{
    std::cin.get();
}

//------------------------------------------------------------------------------

inline void keep_window_open(const std::string& str)
{
    static int attempts = 10; // Maximum number of attempts before forceful exit

    while (--attempts)
    {
        std::cout << "Please enter " << str << " to exit" << std::endl;

        bool exit = true;

        for(std::string::const_iterator p = str.begin(); p != str.end(); ++p)
            if (*p != std::cin.get())
            {
                exit = false;
                break;
            }

         if (exit)
             break;
    }
}

//------------------------------------------------------------------------------

// Helper function to show an error message
inline void error(const std::string& errormessage)
{
    std::cerr << errormessage << std::endl;
    throw std::runtime_error(errormessage);
}

//------------------------------------------------------------------------------

inline void error(std::string s1, std::string s2)
{
    error(s1+s2);
}

//------------------------------------------------------------------------------

template <typename Target, typename Source>
Target narrow_cast(Source src)
{
    Target tgt = src;

    if ((Source)tgt != src)
        error("Invalid narrowing conversion");

    return tgt;
}

//------------------------------------------------------------------------------

inline std::ios_base& general(std::ios_base& b)    // to complement fixed and scientific
{
    b.setf(std::ios_base::fmtflags(0), std::ios_base::floatfield);
    return b;
}

//------------------------------------------------------------------------------

#endif // STD_LIB_FACILITIES_GUARD

