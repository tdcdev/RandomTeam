// RandomTeam - Agent.cpp

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



#include <RandomTeam/engine/Agent.hpp>
#include <RandomTeam/engine/Clock.hpp>
#include <RandomTeam/RandomTeam/tools.hpp>



Agent::Agent(const std::string& id):
    m_id(id),
    m_team("none"),
    m_position(""),
    m_role(Agent::Role::NONE),
    m_energy(-1),
    m_health(-1),
    m_maxEnergy(-1),
    m_maxEnergyDisabled(-1),
    m_maxHealth(-1),
    m_strength(-1),
    m_visRange(-1),
    m_zoneScore(-1),
    m_deadline(-1)
{
    m_actions.push_back(
            Action(
                rechargeGenerator, rechargeSimulator, rechargePerformer
                )
            );
    m_actions.push_back(
            Action(
                gotoGenerator, gotoSimulator, gotoPerformer
                )
            );
}



Agent::Agent(const Agent& agent):
    m_id(agent.m_id),
    m_team(agent.m_team),
    m_position(agent.m_position),
    m_role(agent.m_role),
    m_energy(agent.m_energy),
    m_health(agent.m_health),
    m_maxEnergy(agent.m_maxEnergy),
    m_maxEnergyDisabled(agent.m_maxEnergyDisabled),
    m_maxHealth(agent.m_maxHealth),
    m_strength(agent.m_strength),
    m_visRange(agent.m_visRange),
    m_zoneScore(agent.m_zoneScore),
    m_deadline(agent.m_deadline),
    m_actions(agent.m_actions),
    m_graph(agent.m_graph),
    m_playouts(agent.m_playouts)
{

}



Agent& Agent::operator=(const Agent& agent)
{
    m_id = agent.m_id;
    m_team = agent.m_team;
    m_position = agent.m_position;
    m_role = agent.m_role;
    m_energy = agent.m_energy;
    m_health = agent.m_health;
    m_maxEnergy = agent.m_maxEnergy;
    m_maxEnergyDisabled = agent.m_maxEnergyDisabled;
    m_maxHealth = agent.m_maxHealth;
    m_strength = agent.m_strength;
    m_visRange = agent.m_visRange;
    m_zoneScore = agent.m_zoneScore;
    m_deadline = agent.m_deadline;
    m_actions = agent.m_actions;
    m_graph = agent.m_graph;
    m_playouts = agent.m_playouts;

    return *this;
}



Agent::~Agent()
{

}



const std::string& Agent::id() const
{
    return m_id;
}



const std::string& Agent::team() const
{
    return m_team;
}



const std::string& Agent::position() const
{
    return m_position;
}



Agent::Role Agent::role() const
{
    return m_role;
}



int Agent::energy() const
{
    return m_energy;
}



int Agent::health() const
{
    return m_health;
}



int Agent::maxEnergy() const
{
    return m_maxEnergy;
}



int Agent::maxEnergyDisabled() const
{
    return m_maxEnergyDisabled;
}



int Agent::maxHealth() const
{
    return m_maxHealth;
}



int Agent::strength() const
{
    return m_strength;
}



int Agent::visRange() const
{
    return m_visRange;
}



int Agent::zoneScore() const
{
    return m_zoneScore;
}



long long int Agent::deadline() const
{
    return m_deadline;
}



int Agent::remainingTime() const
{
    long long int diff = m_deadline - Clock::now();

    if (diff < 0)
    {
        return -1;
    }

    if (diff > 10000)
    {
        return 10000;
    }

    return (int)diff;
}



unsigned int Agent::nbPlayouts() const
{
    return m_playouts.size();
}



bool Agent::simulatePlayout(unsigned int index, SimulationGraph& graph) const
{
    if (index >= m_playouts.size())
    {
        error("Agent " + m_id + ": index playout out of range");
        return false;
    }

    Playout playout = m_playouts[index];
    ActionSimulator sim = std::get<1>(playout.first);
    graph = sim(m_id, playout.second, *m_graph);

    return true;
}



void Agent::setTeam(const std::string& team)
{
    m_team = team;
}



void Agent::setPosition(std::string& position)
{
    m_position = position;
}



void Agent::setRole(Agent::Role role)
{
    m_role = role;
}



void Agent::setEnergy(int energy)
{
    m_energy = energy;
}



void Agent::setHealth(int health)
{
    m_health = health;
}



void Agent::setMaxEnergy(int maxEnergy)
{
    m_maxEnergy = maxEnergy;
}



void Agent::setMaxEnergyDisabled(int maxEnergyDisabled)
{
    m_maxEnergyDisabled = maxEnergyDisabled;
}



void Agent::setMaxHealth(int maxHealth)
{
    m_maxHealth = maxHealth;
}



void Agent::setStrength(int strength)
{
    m_strength = strength;
}



void Agent::setVisRange(int visRange)
{
    m_visRange = visRange;
}



void Agent::setZoneScore(int zoneScore)
{
    m_zoneScore = zoneScore;
}



void Agent::setDeadline(long long int deadline)
{
    m_deadline = deadline;
}



void Agent::generatePlayouts(SimulationGraph* graph)
{
    m_playouts.clear();
    m_graph = graph;

    for (
            ActionsVector::const_iterator action = m_actions.begin();
            action != m_actions.end();
            ++action
        )
    {
        ActionGenerator gen = std::get<0>(*action);
        std::vector<std::string> params;

        gen(m_id, *m_graph, params);

        for (
                std::vector<std::string>::const_iterator param = params.begin();
                param != params.end();
                ++param
            )
        {
            m_playouts.push_back(Playout(*action, *param));
        }
    }
}
