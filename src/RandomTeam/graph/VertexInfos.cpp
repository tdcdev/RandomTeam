// RandomTeam - VertexInfos.cpp

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



#include <RandomTeam/graph/VertexInfos.hpp>
#include <RandomTeam/engine/Agent.hpp>
#include <RandomTeam/engine/Clock.hpp>




VertexInfos::VertexInfos():
    m_id(""),
    m_value(0),
    m_team("none"),
    m_visited(false),
    m_deadline(0)
{

}



VertexInfos::VertexInfos(const VertexInfos& infos):
    m_teammates(infos.m_teammates),
    m_opponents(infos.m_opponents),
    m_id(infos.m_id),
    m_value(infos.m_value),
    m_team(infos.m_team),
    m_visited(infos.m_visited),
    m_deadline(infos.m_deadline)
{

}



VertexInfos::VertexInfos(const std::string& id):
    m_id(id),
    m_value(0),
    m_team("none"),
    m_visited(false),
    m_deadline(0)
{

}



VertexInfos& VertexInfos::operator=(const VertexInfos& infos)
{
    m_teammates = infos.m_teammates;
    m_opponents = infos.m_opponents;
    m_id = infos.m_id;
    m_value = infos.m_value;
    m_team = infos.m_team;
    m_visited = infos.m_visited;
    m_deadline = infos.m_deadline;

    return *this;
}



std::vector<const Agent*> VertexInfos::enabledTeammates() const
{
    std::vector<const Agent*> teammates;

    for (
        std::vector<const Agent*>::const_iterator it = m_teammates.begin();
        it != m_teammates.end();
        it++
        )
    {
        if ((*it)->isEnable())
        {
            teammates.push_back(*it);
        }
    }

    return teammates;
}



std::vector<const Agent*> VertexInfos::enabledOpponents() const
{
    std::vector<const Agent*> opponents;

    for (
        std::vector<const Agent*>::const_iterator it = m_opponents.begin();
        it != m_opponents.end();
        it++
        )
    {
        if ((*it)->isEnable())
        {
            opponents.push_back(*it);
        }
    }

    return opponents;
}



void VertexInfos::clear()
{
    long long int diff = m_deadline - Clock::now();

    m_teammates.clear();
    m_opponents.clear();

    if (diff < 0)
    {
        m_team = "none";
    }
}
