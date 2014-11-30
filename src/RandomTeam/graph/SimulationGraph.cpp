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



SimulationGraph::SimulationGraph()
{

}



SimulationGraph::SimulationGraph(const SimulationGraph& sg):
    m_graph(sg.m_graph)
{
    std::pair<BGraph::vertex_iterator, BGraph::vertex_iterator> it;

    it = boost::vertices(m_graph);

    while (it.first != it.second)
    {
        Vertex vertex = *it.first;
        VertexInfos infos = m_graph[vertex];
        m_vertices[infos.m_id] = vertex;
        it.first++;
    }
}



SimulationGraph& SimulationGraph::operator=(const SimulationGraph& sg)
{

    std::pair<BGraph::vertex_iterator, BGraph::vertex_iterator> it;

    m_graph = sg.m_graph;
    it = boost::vertices(m_graph);

    while (it.first != it.second)
    {
        Vertex vertex = *it.first;
        VertexInfos infos = m_graph[vertex];
        m_vertices[infos.m_id] = vertex;
        it.first++;
    }

    return *this;
}



SimulationGraph::~SimulationGraph()
{

}



VertexInfos* SimulationGraph::vertex(const std::string& id)
{
    if (m_vertices.find(id) == m_vertices.end())
    {
        return nullptr;
    }

    return &m_graph[m_vertices[id]];
}



EdgeInfos* SimulationGraph::edge(const std::string& n1, const std::string& n2)
{
    if (m_vertices.find(n1) == m_vertices.end())
    {
        return nullptr;
    }

    if (m_vertices.find(n2) == m_vertices.end())
    {
        return nullptr;
    }

    Vertex vertex1 = m_vertices[n1];
    Vertex vertex2 = m_vertices[n2];
    Edge edge = boost::edge(vertex1, vertex2, m_graph).first;

    return &m_graph[edge];
}



VertexInfos* SimulationGraph::addVertex(const std::string& id)
{
    if (m_vertices.find(id) == m_vertices.end())
    {
        m_vertices[id] = boost::add_vertex(VertexInfos(id), m_graph);
    }

    return &m_graph[m_vertices[id]];
}



EdgeInfos* SimulationGraph::addEdge(const std::string& n1, const std::string& n2)
{
    this->addVertex(n1);
    this->addVertex(n2);
    Edge edge = boost::add_edge(m_vertices[n1], m_vertices[n2], m_graph).first;

    return &m_graph[edge];
}
