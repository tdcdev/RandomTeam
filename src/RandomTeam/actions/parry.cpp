// RandomTeam - parry.cpp

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



#include <RandomTeam/actions/parry.hpp>
#include <RandomTeam/graph/SimulationGraph.hpp>
#include <RandomTeam/engine/Teammate.hpp>



void parryGenerator(
        const Agent& agent,
        const SimulationGraph& graph,
        std::vector<std::string>& params
        )
{
    if (
            agent.role() != Agent::Role::SENTINEL &&
            agent.role() != Agent::Role::SABOTEUR &&
            agent.role() != Agent::Role::REPAIRER
            )
    {
        return;
    }

    if (agent.energy() < 2 || agent.health() < 1 || !agent.isEnable())
    {
        return;
    }

    std::vector<const VertexInfos*> vertices;
    const VertexInfos* vertex = graph.vertex(agent.position());

    if (vertex != nullptr)
    {
        // graph.getNeighbors(vertex->m_id, vertices);
        vertices.push_back(vertex);

        for (
            std::vector<const VertexInfos*>::iterator it = vertices.begin();
            it != vertices.end();
            it++
            )
        {
            std::vector<const Agent*>::const_iterator opp;

            for (
                    opp = (*it)->m_opponents.begin();
                    opp != (*it)->m_opponents.end();
                    opp++
                )
            {
                const std::string pos = (*opp)->position();
                const EdgeInfos* edge = graph.edge(agent.position(), pos);

                if ((*opp)->role() != Agent::Role::SABOTEUR)
                {
                    continue;
                }

                if (edge != nullptr || agent.position() == pos)
                {
                    params.push_back("");
                    return;
                }
            }
        }
    }
}



bool parrySimulator(
        const Agent& agent,
        const std::string& param,
        SimulationGraph& graph
        )
{
    Agent* newAgent = graph.teammate(agent.id());

    if (newAgent == nullptr)
    {
        return false;
    }

    newAgent->setHealth(newAgent->health() + 4);

    return true;
}



void parryPerformer(Teammate& teammate, const std::string& param)
{
    teammate.setActionOrder("parry", param);
}
