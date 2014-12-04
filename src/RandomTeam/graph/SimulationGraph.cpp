// RandomTeam - SimulationGraph.cpp

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



#include <RandomTeam/graph/SimulationGraph.hpp>
#include <RandomTeam/engine/Teammate.hpp>



SimulationGraph::SimulationGraph()
{

}



SimulationGraph::SimulationGraph(const SimulationGraph& sg):
    m_graph(sg.m_graph),
    m_teammates(sg.m_teammates),
    m_opponents(sg.m_opponents)
{
    for (
            VerticesMapIterator it = boost::vertices(m_graph);
            it.first != it.second;
            ++it.first
        )
    {
        Vertex vertex = *it.first;
        VertexInfos infos = m_graph[vertex];
        m_vertices[infos.m_id] = vertex;
    }

    this->setupAgents();
}



SimulationGraph& SimulationGraph::operator=(const SimulationGraph& sg)
{
    m_graph = sg.m_graph;
    m_teammates = sg.m_teammates;
    m_opponents = sg.m_opponents;

    for (
            VerticesMapIterator it = boost::vertices(m_graph);
            it.first != it.second;
            ++it.first
        )
    {
        Vertex vertex = *it.first;
        VertexInfos infos = m_graph[vertex];
        m_vertices[infos.m_id] = vertex;
    }

    this->setupAgents();

    return *this;
}



SimulationGraph::~SimulationGraph()
{

}



const VertexInfos* SimulationGraph::vertex(const std::string& id) const
{
    if (m_vertices.find(id) == m_vertices.end())
    {
        return nullptr;
    }

    return &m_graph[m_vertices.at(id)];
}



const EdgeInfos* SimulationGraph::edge(
        const std::string& n1,
        const std::string& n2
        ) const
{
    if (m_vertices.find(n1) == m_vertices.end())
    {
        return nullptr;
    }

    if (m_vertices.find(n2) == m_vertices.end())
    {
        return nullptr;
    }

    Vertex vertex1 = m_vertices.at(n1);
    Vertex vertex2 = m_vertices.at(n2);
    Edge edge = boost::edge(vertex1, vertex2, m_graph).first;

    return &m_graph[edge];
}



void SimulationGraph::getNeighbors(
                const std::string& id,
                std::vector<const VertexInfos*>& neighbors
                ) const
{
    if (m_vertices.find(id) == m_vertices.end())
    {
        return;
    }

    std::pair<BGraph::adjacency_iterator, BGraph::adjacency_iterator> it;
    Vertex vertex = m_vertices.at(id);

    it = boost::adjacent_vertices(vertex, m_graph);

    while (it.first != it.second)
    {
        neighbors.push_back(&m_graph[*it.first]);
        it.first++;
    }
}



VertexInfos* SimulationGraph::vertex(const std::string& id)
{
    return const_cast<VertexInfos*>(
            static_cast<const SimulationGraph&>(*this).vertex(id)
            );
}



EdgeInfos* SimulationGraph::edge(const std::string& n1, const std::string& n2)
{
    return const_cast<EdgeInfos*>(
            static_cast<const SimulationGraph&>(*this).edge(n1, n2)
            );
}



VertexInfos* SimulationGraph::addVertex(const std::string& id)
{
    if (m_vertices.find(id) == m_vertices.end())
    {
        m_vertices[id] = boost::add_vertex(VertexInfos(id), m_graph);
    }

    return &m_graph[m_vertices[id]];
}



EdgeInfos* SimulationGraph::addEdge(
        const std::string& n1,
        const std::string& n2
        )
{
    this->addVertex(n1);
    this->addVertex(n2);
    Edge edge = boost::add_edge(m_vertices[n1], m_vertices[n2], m_graph).first;

    return &m_graph[edge];
}



void SimulationGraph::setAgents(
        const std::vector<Teammate>& teammates,
        const std::vector<Agent>& opponents
        )
{
    std::vector<Agent> tmp;

    for (
            std::vector<Teammate>::const_iterator it = teammates.begin();
            it != teammates.end();
            ++it
        )
    {
        tmp.push_back(Agent(*it));
    }

    this->setAgents(tmp, opponents);
}



void SimulationGraph::setAgents(
        const std::vector<Agent>& teammates,
        const std::vector<Agent>& opponents
        )
{
    m_teammates = teammates;
    m_opponents = opponents;

    this->setupAgents();
}



void SimulationGraph::setupAgents()
{
    for (
            VerticesMapIterator it = boost::vertices(m_graph);
            it.first != it.second;
            ++it.first
        )
    {
        m_graph[*it.first].clear();
    }

    for (
            std::vector<Agent>::const_iterator it = m_teammates.begin();
            it != m_teammates.end();
            ++it
        )
    {
        VertexInfos* vertex = this->vertex(it->position());

        if (vertex != nullptr)
        {
            vertex->m_teammates.push_back(&(*it));
            vertex->m_visited = true;
        }
    }

    for (
            std::vector<Agent>::const_iterator it = m_opponents.begin();
            it != m_opponents.end();
            ++it
        )
    {
        VertexInfos* vertex = this->vertex(it->position());

        if (vertex != nullptr)
        {
            vertex->m_opponents.push_back(&(*it));
        }
    }
}



float SimulationGraph::fitness() const
{
    float count = 0.f;

    for (
            VerticesMapIterator it = boost::vertices(m_graph);
            it.first != it.second;
            ++it.first
        )
    {
        if (m_graph[*it.first].m_visited)
        {
            count += 1.f;
        }
    }

    return count;
}
