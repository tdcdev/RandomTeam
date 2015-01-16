// RandomTeam - tools.cpp

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



#include <RandomTeam/RandomTeam/tools.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>



void initLogging()
{
    namespace expr = boost::log::expressions;
    namespace keywords = boost::log::keywords;

#ifdef NDEBUG

    boost::log::core::get()->set_filter
    (
        boost::log::trivial::severity >= boost::log::trivial::info
    );

#else

    boost::log::core::get()->set_filter
    (
        boost::log::trivial::severity >= boost::log::trivial::debug
    );

    boost::log::add_file_log
        (
         keywords::file_name = "RandomTeam_%y_%m_%d_%Hh%M.log",
         keywords::auto_flush = true,
         keywords::format =
         (
          expr::stream
          << expr::format_date_time<boost::posix_time::ptime>(
              "TimeStamp",
              "%H:%M:%S"
              )
          << " [" << boost::log::trivial::severity
          << "] " << expr::smessage
         )
        );

#endif

    boost::log::add_console_log
        (
         std::cout,
         keywords::format =
         (
          expr::stream
          << expr::format_date_time<boost::posix_time::ptime>(
              "TimeStamp",
              "%H:%M:%S"
              )
          << " [" << boost::log::trivial::severity
          << "] " << expr::smessage
         )
        );


    boost::log::add_common_attributes();
}



void log(boost::log::trivial::severity_level level, const std::string& msg)
{
    boost::log::sources::severity_logger<boost::log::trivial::severity_level> s;

    BOOST_LOG_SEV(s, level) << msg;
}



void debug(const std::string& msg)
{
    log(boost::log::trivial::debug, msg);
}



void info(const std::string& msg)
{
    log(boost::log::trivial::info, msg);
}



void warning(const std::string& msg)
{
    log(boost::log::trivial::warning, msg);
}



void error(const std::string& msg)
{
    log(boost::log::trivial::error, msg);
}



void sleep(int ms)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}



bool getXMLElement(
        tinyxml2::XMLElement* tag,
        const std::string& value,
        tinyxml2::XMLElement** element,
        bool log
        )
{
    *element = tag->FirstChildElement(value.c_str());

    if (*element == nullptr)
    {
        if (log)
        {
            error("No tag <" + value + "> in tag <" + tag->Value() + ">");
        }

        return false;
    }

    return true;
}



bool getXMLAttributes(
        tinyxml2::XMLElement* tag,
        const std::vector<std::string>& attributes,
        std::vector<std::string>& values,
        bool log
        )
{
    bool flag = true;

    values.clear();

    for (
        std::vector<std::string>::const_iterator it = attributes.begin();
        it != attributes.end();
        it++
        )
    {
        const char* v = tag->Attribute(it->c_str());

        if (v == nullptr)
        {
            flag =  false;

            if (log)
            {
                error(
                        "No attribute \"" +
                        *it +
                        "\" in tag <" +
                        tag->Value() +
                        ">"
                        );
            }

            values.push_back(std::string());
        }
        else
        {
            values.push_back(std::string(v));
        }
    }

    return flag;
}
