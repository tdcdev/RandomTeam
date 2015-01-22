// RandomTeam - World.cpp

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



#include <RandomTeam/engine/World.hpp>
#include <RandomTeam/engine/Clock.hpp>
#include <RandomTeam/RandomTeam/tools.hpp>
#include <chrono>
#include <thread>



std::random_device rd;
std::mt19937 World::s_randGen(rd());



World::World():
    m_running(true),
    m_started(false),
    m_simulationId(""),
    m_maxEdges(-1),
    m_maxVertices(-1),
    m_maxSteps(-1),
    m_step(-1),
    m_team("none"),
    m_currentIt(0)
{

}



World::~World()
{

}



unsigned int World::nbTeammates() const
{
    return m_teammates.size();
}



Teammate* World::teammate(unsigned int index)
{
    return &m_teammates[index];
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



const std::string& World::team() const
{
    return m_team;
}



int World::remainingTime() const
{
    int min = INT_MAX;

    for (
            std::vector<Teammate>::const_iterator it = m_teammates.begin();
            it != m_teammates.end();
            it++
        )
    {
        int test = it->remainingTime();

        if (test < min)
        {
            min = test;
        }
    }

    return min;
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



void World::setTeam(const std::string& team)
{
    if (m_team == "none")
    {
        m_team = team;

        for (
                std::vector<Teammate>::iterator teammate = m_teammates.begin();
                teammate != m_teammates.end();
                teammate++
            )
        {
            teammate->setTeam(team);
        }

        info("Playing as team \"" + m_team + "\"");
    }
}



void World::startSimulation(
        const std::string& id,
        int maxEdges,
        int maxVertices,
        int maxSteps
        )
{
    if (!m_started)
    {
        std::string infostr;

        m_graph = SimulationGraph();
        m_started = true;
        m_simulationId = id;
        m_maxEdges = maxEdges;
        m_maxVertices = maxVertices;
        m_maxSteps = maxSteps;

        m_opponents.clear();
        m_opponentsLU.clear();

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

        m_started = false;
        m_simulationId = "";
        m_maxEdges = -1;
        m_maxVertices = -1;
        m_maxSteps = -1;
        m_step = -1;
        m_team = "none";

        m_opponents.clear();
        m_opponentsLU.clear();
    }
}



void World::inspectAgent(const Agent& agent)
{
    for (
        std::vector<Agent>::iterator opponent = m_opponentsLU.begin();
        opponent != m_opponentsLU.end();
        opponent++
        )
    {
        if (opponent->id() == agent.id())
        {
            *opponent = agent;
            return;
        }
    }

    m_opponentsLU.push_back(agent);
}



void World::seeAgent(const Agent& agent)
{
    if (m_team == agent.team())
    {
        return;
    }

    for (
        std::vector<Teammate>::const_iterator teammate = m_teammates.begin();
        teammate != m_teammates.end();
        teammate++
        )
    {
        if (teammate->id() == agent.id())
        {
            this->setTeam(agent.team());
            return;
        }
    }

    for (
            std::vector<Agent>::iterator opponent = m_opponentsLU.begin();
            opponent != m_opponentsLU.end();
            opponent++
        )
    {
        if (opponent->id() == agent.id())
        {
            opponent->setPosition(agent.position());
            opponent->setEnable(agent.isEnable());
            opponent->setDeadline(agent.deadline());
            m_opponents.push_back(*opponent);
            return;
        }
    }

    for (
        std::vector<Agent>::iterator opponent = m_opponents.begin();
        opponent != m_opponents.end();
        opponent++
        )
    {
        if (opponent->id() == agent.id())
        {
            return;
        }
    }


    m_opponents.push_back(agent);
}



void World::addVertex(
        const std::string& id,
        const std::string& team,
        long long int deadline
        )
{
    VertexInfos *vertex = m_graph.addVertex(id, deadline);

    if (vertex != nullptr)
    {
        vertex->m_team = team;
    }
}



void World::addEdge(const std::string& n1, const std::string& n2)
{
    m_graph.addEdge(n1, n2);
}



void World::updateVertex(const std::string& id, int v)
{
    VertexInfos *vertex = m_graph.vertex(id);

    if (vertex != nullptr)
    {
        vertex->m_value = v;
    }
}



void World::updateEdge(const std::string& n1, const std::string& n2, int w)
{
    EdgeInfos *edge = m_graph.edge(n1, n2);

    if (edge != nullptr)
    {
        edge->m_weight = w;
    }
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

        if (getXMLAttributes(agent, attributes, values))
        {
            m_teammates.push_back(Teammate(this, values[0], values[1]));
        }
        agent = agent->NextSiblingElement("agent");
    }

    info(std::to_string(m_teammates.size()) + " agents created");

    return true;
}



void World::clear()
{
    m_opponents.erase(
            std::remove_if(
                m_opponents.begin(),
                m_opponents.end(),
                [](const Agent& agent) {return agent.remainingTime() < 0;}
                ),
            m_opponents.end()
            );
}



void World::generateAllPlayouts()
{
    if (!m_started)
    {
        return;
    }

    m_graph.setAgents(m_teammates, m_opponents);
    m_solution.clear();
    m_fitness = std::numeric_limits<float>::min();
    m_currentIt = 0;

    for (
            std::vector<Teammate>::iterator it = m_teammates.begin();
            it != m_teammates.end();
            it++
        )
    {
        it->generatePlayouts(m_graph);
        unsigned int nb = it->nbPlayouts();
        int rand = -1;

        if (nb > 0)
        {
            std::uniform_int_distribution<unsigned int> dist(0, nb - 1);
            rand = dist(s_randGen);
        }

        m_solution.push_back(rand);
    }
}



void World::thinkThread(const std::vector<int>& solution, float& score) const
{
    SimulationGraph g(m_graph);
    int i = 0;

    for (
            std::vector<Teammate>::const_iterator it = m_teammates.begin();
            it != m_teammates.end();
            it++, i++
        )
    {
        if (solution[i] > 0)
        {
            it->simulatePlayout(solution[i], g);
        }
    }

    score = g.fitness();
}



void World::think(unsigned int nbThreads)
{
    if (!m_started)
    {
        return;
    }

    if (m_teammates.size() != m_solution.size())
    {
        error("number of teammates != solution size");
        return;
    }

    std::vector< std::shared_ptr<std::thread> > threads;
    std::vector< std::vector<int> > solutions(nbThreads, m_solution);
    std::vector<float> scores(nbThreads);
    unsigned int index = m_currentIt % m_teammates.size();
    unsigned int nb = m_teammates[index].nbPlayouts();
    unsigned int rand = 0;
    std::uniform_int_distribution<int> dist(1, nb - 1);

    threads.assign(nbThreads, nullptr);

    rand = dist(s_randGen);

    if (nb <= 0)
    {
        return;
    }

    for (unsigned int i = 0; i < nbThreads; i++)
    {
        solutions[i][index] += rand + i;
        solutions[i][index] %= nb;

        threads[i].reset(
                new std::thread(
                    &World::thinkThread,
                    this,
                    std::ref(solutions[i]),
                    std::ref(scores[i])
                    )
                );
    }

    for (unsigned int i = 0; i < nbThreads; i++)
    {
        threads[i]->join();

        if (scores[i] > m_fitness)
        {
            m_fitness = scores[i];
            m_solution = solutions[i];
            debug(
                    "Solution upgraded (" +
                    std::to_string(m_currentIt) +
                    "): " +
                    std::to_string(m_fitness)
                    );
        }
    }

    m_currentIt++;
}



void World::randomThink()
{
    if (!m_started)
    {
        return;
    }

    if (m_teammates.size() != m_solution.size())
    {
        error("number of teammates != solution size");
        return;
    }

    if (m_currentIt > 0)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return;
    }

    unsigned int n = 0;

    for (
        std::vector<Teammate>::const_iterator teammate = m_teammates.begin();
        teammate != m_teammates.end();
        teammate++, n++
        )
    {
        unsigned int nb = teammate->nbPlayouts();
        std::uniform_int_distribution<int> dist(0, nb - 1);

        if (nb > 0)
        {
            m_solution[n] += dist(s_randGen);
            m_solution[n] %= nb;
        }
    }

    std::thread thread(
            &World::thinkThread,
            this,
            std::ref(m_solution),
            std::ref(m_fitness)
            );

    thread.join();

    debug(
            "Solution upgraded (" +
            std::to_string(m_currentIt) +
            "): " +
            std::to_string(m_fitness)
         );

    m_currentIt++;
}



void World::perform()
{
    if (m_teammates.size() != m_solution.size())
    {
        error("number of teammates != solution size");
        return;
    }

    std::vector<Teammate>::iterator it;
    std::vector<int>::const_iterator n;

    for (
            it = m_teammates.begin(), n = m_solution.begin();
            it != m_teammates.end();
            it++, n++
        )
    {
        if (*n >= 0)
        {
            it->performPlayout(*n);
        }
    }
}
