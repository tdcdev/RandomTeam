// RandomTeam - goto.cpp

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



#include <RandomTeam/actions/goto.hpp>
#include <RandomTeam/graph/SimulationGraph.hpp>
#include <RandomTeam/engine/Teammate.hpp>



void gotoGenerator(
        const Agent& agent,
        const SimulationGraph& graph,
        std::vector<std::string>& params
        )
{
    if (agent.energy() < 1)
    {
        return;
    }

    std::vector<const VertexInfos*> vertices;
    const VertexInfos* vertex = graph.vertex(agent.position());

    if (vertex != nullptr)
    {
        graph.getNeighbors(vertex->m_id, vertices);

        for (
            std::vector<const VertexInfos*>::iterator it = vertices.begin();
            it != vertices.end();
            it++
            )
        {
            const EdgeInfos* edge = graph.edge(vertex->m_id, (*it)->m_id);

            if (edge != nullptr && edge->m_weight <= agent.energy())
            {
                params.push_back((*it)->m_id);
            }
        }
    }
}



bool gotoSimulator(
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

    VertexInfos* from = graph.vertex(newAgent->position());
    VertexInfos* to = graph.vertex(param);
    EdgeInfos* path = graph.edge(newAgent->position(), param);

    if (from == nullptr || to == nullptr || path == nullptr)
    {
        return false;
    }

    std::vector<const Agent*>& v(from->m_teammates);
    v.erase(std::remove(v.begin(), v.end(), newAgent), v.end());
    to->m_teammates.push_back(newAgent);
    to->m_visited = true;
    newAgent->setPosition(param);
    newAgent->setEnergy(newAgent->energy() - path->m_weight);

    return true;
}



void gotoPerformer(Teammate& teammate, const std::string& param)
{
    teammate.setActionOrder("goto", param);
}
