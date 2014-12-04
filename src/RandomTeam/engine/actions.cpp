// RandomTeam - actions.cpp

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



#include <RandomTeam/engine/actions.hpp>
#include <RandomTeam/engine/Teammate.hpp>



void rechargeGenerator(
        const Agent& agent,
        const SimulationGraph& graph,
        std::vector<std::string>& params
        )
{
    params.push_back("");
}



void gotoGenerator(
        const Agent& agent,
        const SimulationGraph& graph,
        std::vector<std::string>& params
        )
{
    std::vector<const VertexInfos*> vertices;
    const VertexInfos* vertex;

    vertex = graph.vertex(agent.position());

    if (vertex != nullptr)
    {
        graph.getNeighbors(vertex->m_id, vertices);

        for (
            std::vector<const VertexInfos*>::iterator it = vertices.begin();
            it != vertices.end();
            ++it
            )
        {
            params.push_back((*it)->m_id);
        }
    }
}



bool rechargeSimulator(
        const Agent& agent,
        const std::string& param,
        const SimulationGraph& graph,
        SimulationGraph& result
        )
{
    /* TODO */

    return true;
}



bool gotoSimulator(
        const Agent& agent,
        const std::string& param,
        const SimulationGraph& graph,
        SimulationGraph& result
        )
{
    /* TODO */

    return true;
}



void rechargePerformer(Teammate& teammate, const std::string& param)
{
    teammate.setActionOrder("recharge", param);
}



void gotoPerformer(Teammate& teammate, const std::string& param)
{
    teammate.setActionOrder("goto", param);
}
