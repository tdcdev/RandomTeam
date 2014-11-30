// RandomTeam - World.cpp

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



#include <RandomTeam/engine/World.hpp>
#include <RandomTeam/engine/Agent.hpp>
#include <RandomTeam/engine/Clock.hpp>
#include <RandomTeam/RandomTeam/tools.hpp>
#include <tinyxml2/tinyxml2.h>



World::World():
    m_running(true),
    m_started(false),
    m_maxEdges(-1),
    m_maxVertices(-1),
    m_maxSteps(-1),
    m_step(-1)
{

}



World::~World()
{

}



unsigned int World::numAgents() const
{
    return m_agents.size();
}



std::shared_ptr<Agent> World::agent(unsigned int index) const
{
    return *(m_agents.begin() + index);
}



bool World::running() const
{
    return m_running;
}



bool World::started() const
{
    return m_started;
}



const std::string& World::simulationId() const
{
    return m_simulationId;
}



int World::maxEdges() const
{
    return m_maxEdges;
}



int World::maxVertices() const
{
    return m_maxVertices;
}



int World::maxSteps() const
{
    return m_maxSteps;
}



int World::step() const
{
    return m_step;
}



void World::setRunning(bool on)
{
    m_running = on;
}



void World::setStep(int step)
{
    if (step > m_step)
    {
        m_step = step;
        std::string stepstr(std::to_string(m_step));
        info("Simulation " + m_simulationId + " step " + stepstr);
    }
}



void World::initSimulation(
        const std::string& id,
        int maxEdges,
        int maxVertices,
        int maxSteps
        )
{
    if (!m_started)
    {
        std::string infostr;

        m_started = true;
        m_simulationId = id;
        m_maxEdges = maxEdges;
        m_maxVertices = maxVertices;
        m_maxSteps = maxSteps;

        infostr += "Simulation " + m_simulationId + " started with ";
        infostr += "maxEdges = " + std::to_string(m_maxEdges) + " ";
        infostr += "maxVertices = " + std::to_string(m_maxVertices) + " ";
        infostr += "maxSteps = " + std::to_string(m_maxSteps);

        info(infostr);
    }
}



void World::endSimulation(const std::string& rank, const std::string& score)
{
    if (m_started)
    {
        info("Simulation " + m_simulationId + " is over");
        info("rank = " + rank + " score = " + score);

        m_graph = SimulationGraph();
        m_started = false;
        m_simulationId = "";
        m_maxEdges = -1;
        m_maxVertices = -1;
        m_maxSteps = -1;
        m_step = -1;
    }
}



void World::addVertex(const std::string& id, const std::string& team)
{
    VertexInfos *vertex = m_graph.addVertex(id);

    if (vertex != nullptr)
    {
        vertex->m_team = team;
    }
}



void World::addEdge(const std::string& n1, const std::string& n2)
{
    m_graph.addEdge(n1, n2);
}



bool World::loadTeam(const std::string& file)
{
    tinyxml2::XMLDocument doc;

    info("Loading " + file);

    if (doc.LoadFile(file.c_str()) != tinyxml2::XML_NO_ERROR)
    {
        error("File \"" + file + "\": file not found");
        return false;
    }

    tinyxml2::XMLNode* root = doc.FirstChildElement("agents");

    if (root == nullptr)
    {
        error("File \"" + file + "\": no tag <agents>");
        return false;
    }

    tinyxml2::XMLElement* agent = root->FirstChildElement("agent");

    while (agent)
    {
        std::vector<std::string> attributes = {"id", "password"};
        std::vector<std::string> values;

        getXMLAttributes(agent, attributes, values);
        m_agents.push_back(std::shared_ptr<Agent>(
                    new Agent(*this, values[0], values[1])
                    ));
        agent = agent->NextSiblingElement("agent");
    }

    info(std::to_string(m_agents.size()) + " agents created");

    return true;
}



int World::remainingTime() const
{
    std::vector< std::shared_ptr<Agent> >::const_iterator it = m_agents.begin();
    int min = INT_MAX;

    while (it != m_agents.end())
    {
        int test = (*it)->remainingTime();

        if (test < min)
        {
            min = test;
        }

        it++;
    }

    return min;
}



void World::think()
{
    Clock clock;

    if (m_started)
    {

    }

    clock.elapsedTime();
}
