// RandomTeam - Clock.cpp

// Authors:

//     Thomas Da Costa <tdc.input@gmail.com>
//     Pierre Luce-Vayrac <p.luce.vayrac@gmail.com>

// Copyright (C) 2014 Thomas Da Costa and Piere Luce-Vayrac

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



#include <RandomTeam/engine/Clock.hpp>



Clock::Clock():
    m_now(boost::posix_time::microsec_clock::local_time())
{

}



Clock::~Clock()
{

}



int Clock::reset()
{
    boost::posix_time::ptime now(
            boost::posix_time::microsec_clock::local_time()
            );
    boost::posix_time::time_duration delta(now - m_now);

    m_now = now;

    return static_cast<int>(delta.total_milliseconds());
}



int Clock::elapsedTime()
{
    boost::posix_time::time_duration delta(
            boost::posix_time::microsec_clock::local_time() - m_now
            );

    return static_cast<int>(delta.total_milliseconds());
}



long long int Clock::now()
{
    boost::posix_time::time_duration now =
        boost::posix_time::microsec_clock::universal_time() -
        boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1));

    return now.total_milliseconds();
}
