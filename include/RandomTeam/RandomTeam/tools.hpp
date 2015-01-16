// RandomTeam - tools.hpp

// Authors:

//     Thomas Da Costa <tdc.input@gmail.com>
//     Pierre Luce-Vayrac <p.luce.vayrac@gmail.com>

// Copyright (C) 2014-2015 Thomas Da Costa and Pierre Luce-Vayrac

// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
// OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.



#ifndef _TOOLS_HPP_
#define _TOOLS_HPP_



#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <tinyxml2/tinyxml2.h>



void initLogging();
void debug(const std::string& msg);
void info(const std::string& msg);
void warning(const std::string& msg);
void error(const std::string& msg);
void sleep(int ms);
bool getXMLElement(
        tinyxml2::XMLElement* tag,
        const std::string& value,
        tinyxml2::XMLElement** element,
        bool log = true
        );
bool getXMLAttributes(
        tinyxml2::XMLElement* tag,
        const std::vector<std::string>& attributes,
        std::vector<std::string>& values,
        bool log = true
        );



#endif
