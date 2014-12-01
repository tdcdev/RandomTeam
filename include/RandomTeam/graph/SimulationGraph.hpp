// RandomTeam - SimulationGraph.hpp

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



#ifndef _SIMULATION_GRAPH_HPP_
#define _SIMULATION_GRAPH_HPP_



#include <map>
#include <boost/graph/adjacency_list.hpp>
#include <RandomTeam/graph/VertexInfos.hpp>
#include <RandomTeam/graph/EdgeInfos.hpp>



class SimulationGraph
{

    public:

        SimulationGraph();
        SimulationGraph(const SimulationGraph& sg);
        SimulationGraph& operator=(const SimulationGraph& sg);
        virtual ~SimulationGraph();
        VertexInfos* vertex(const std::string& id);
        EdgeInfos* edge(const std::string& n1, const std::string& n2);
        VertexInfos* addVertex(const std::string& id);
        EdgeInfos* addEdge(const std::string& n1, const std::string& n2);
        float fitness() const;

    private:

        typedef boost::adjacency_list<
            boost::setS,
            boost::vecS,
            boost::undirectedS,
            VertexInfos,
            EdgeInfos
                > BGraph;
        typedef BGraph::vertex_descriptor Vertex;
        typedef BGraph::edge_descriptor Edge;
        typedef std::map<std::string, Vertex> VerticesMap;

        BGraph m_graph;
        VerticesMap m_vertices;

};



#endif
