// RandomTeam - ServerInterface.cpp

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



#include <RandomTeam/engine/ServerInterface.hpp>
#include <RandomTeam/engine/World.hpp>
#include <RandomTeam/engine/Teammate.hpp>
#include <RandomTeam/engine/Clock.hpp>
#include <RandomTeam/network/TCPNode.hpp>
#include <RandomTeam/network/TCPClient.hpp>
#include <RandomTeam/RandomTeam/tools.hpp>



ServerInterface::ServerInterface(
        World& world,
        std::string host,
        unsigned int port
        ):
    m_world(world),
    m_host(host),
    m_port(port)
{

}



ServerInterface::~ServerInterface()
{

}



bool ServerInterface::connect()
{
    info("Connexion to " + m_host + ":" + std::to_string(m_port));

    m_clients.clear();

    for (unsigned int i = 0; i < m_world.nbTeammates(); i++)
    {
        m_clients.push_back(nullptr);
    }

    if (!this->connectClients())
    {
        error("Connect clients failed");
        return false;
    }

    if (!this->writeClients())
    {
        error("Write clients failed");
        return false;
    }

    if (!this->readClients(2000))
    {
        error("Read clients failed");
        return false;
    }

    for (unsigned int i = 0; i < m_world.nbTeammates(); i++)
    {
        if (!m_world.teammate(i)->connected())
        {
            error("Agent " + m_world.teammate(i)->id() + ": not connected");
            return false;
        }
    }

    return true;
}



bool ServerInterface::connectClients()
{
    for (unsigned int i = 0; i < m_world.nbTeammates(); i++)
    {
        m_clients[i].reset(new TCPClient());
        m_world.teammate(i)->authrequest();

        if (!m_clients[i]->connect(m_host, m_port))
        {
            error("Agent " + m_world.teammate(i)->id() + ": connexion failure");
            return false;
        }
        else
        {
            debug("Agent " + m_world.teammate(i)->id() + ": connected");
        }
    }

    return true;
}



bool ServerInterface::readClients(int ms)
{
    int mask = 0;
    unsigned int n = m_world.nbTeammates();

    Clock clock;

    while (clock.elapsedTime() < ms && mask + 1 != (1 << n))
    {
        for (unsigned int i = 0; i < n; i++)
        {
            Teammate* teammate = m_world.teammate(i);

            if (teammate->remainingTime() > 0)
            {
                mask |= 1 << i;
            }

            if (((mask & (1 << i)) == 0) && m_clients[i]->read(20))
            {
                const std::string msg = m_clients[i]->node().input();

                if (msg.length() > 1)
                {
                    teammate->read(msg);
                    mask |= 1 << i;
                }
            }

            if (clock.elapsedTime() >= ms || mask  + 1 == (1 << n))
            {
                break;
            }
        }
    }

    return mask + 1 == (1 << n);
}



bool ServerInterface::writeClients()
{
    bool flag = true;

    for (unsigned int i = 0; i < m_world.nbTeammates(); i++)
    {
        m_clients[i]->setNodeMsgToSend(m_world.teammate(i)->order());

        if (!m_clients[i]->write(20))
        {
            error("Agent " + m_world.teammate(i)->id() + ": write failure");
            flag = false;
        }
        else
        {
            debug("Agent " + m_world.teammate(i)->id() + ": order sent");
        }
    }

    return flag;
}
