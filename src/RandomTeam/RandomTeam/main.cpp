// RandomTeam - main.cpp

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



#include <RandomTeam/RandomTeam/tools.hpp>
#include <RandomTeam/engine/World.hpp>
#include <RandomTeam/engine/ServerInterface.hpp>
#include <RandomTeam/engine/Clock.hpp>
#include <boost/program_options.hpp>



std::string g_team;
std::string g_host;
unsigned int g_port;
unsigned int g_attempts;



boost::program_options::options_description createOptionsDescription()
{
    namespace bpo = boost::program_options;

    bpo::options_description desc("Allowed options");

    desc.add_options()
        ("help,h", "produce this help message")
        (
         "team,t",
         bpo::value<std::string>(&g_team)->required(),
         "team configuration file"
        )
        (
         "host",
         bpo::value<std::string>(&g_host)->default_value("localhost"),
         "host ip"
        )
        (
         "port",
         bpo::value<unsigned int>(&g_port)->default_value(12300),
         "host port"
        )
        (
         "attempts",
         bpo::value<unsigned int>(&g_attempts)->default_value(3),
         "number of connexion attempts"
        )
        ;

    return desc;
}



int main(int argc, char** argv)
{
    namespace bpo = boost::program_options;

    bpo::variables_map vm;
    bpo::options_description desc(createOptionsDescription());

    try
    {
        bpo::store(bpo::parse_command_line(argc, argv, desc), vm);
        bpo::notify(vm);
    }
    catch (bpo::error& e)
    {
        std::cerr << "error: " << e.what() << std::endl;
        std::cerr << desc << std::endl;
        return 1;
    }

    if (vm.count("help"))
    {
        std::cout << desc << std::endl;
        return 0;
    }

    initLogging();

    World world;
    ServerInterface serverInterface(world, g_host, g_port);
    unsigned int attempts = 0;
    bool connected = false;
    bool write = false;
    int endSleepTime = 5;

    if (!world.loadTeam(g_team))
    {
        error("Load team configuration file failed");
        return 1;
    }

    while (attempts < g_attempts && !connected)
    {
        connected = serverInterface.connect();
        attempts++;
    }

    if (!connected)
    {
        return 1;
    }

    while (world.running())
    {
        if (world.remainingTime() <= 0)
        {
            write = true;
            serverInterface.readClients(1300);
        }

        world.think();

        if (write && world.remainingTime() > 0 && world.remainingTime() < 150)
        {
            write = false;
            serverInterface.writeClients();
        }
    }

    for (int i = 0; i < endSleepTime; i++)
    {
        info("SHUTDOWN in " + std::to_string(endSleepTime - i) + "s");
        sleep(1000);
    }

    return 0;
}
