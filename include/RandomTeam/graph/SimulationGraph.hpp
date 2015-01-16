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
#include <vector>
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
        const Agent* teammate(const std::string& id) const;
        const Agent* opponent(const std::string& id) const;
        const VertexInfos* vertex(const std::string& id) const;
        const EdgeInfos* edge(
                const std::string& n1,
                const std::string& n2
                ) const;
        void getNeighbors(
                const std::string& id,
                std::vector<const VertexInfos*>& neighbors
                ) const;
        Agent* teammate(const std::string& id);
        Agent* opponent(const std::string& id);
        VertexInfos* vertex(const std::string& id);
        EdgeInfos* edge(const std::string& n1, const std::string& n2);
        VertexInfos* addVertex(
                const std::string& id,
                long long int deadline
                );
        EdgeInfos* addEdge(
                const std::string& n1,
                const std::string& n2
                );
        void setAgents(
                const std::vector<Teammate>& teammates,
                const std::vector<Agent>& opponents
                );
        void setAgents(
                const std::vector<Agent>& teammates,
                const std::vector<Agent>& opponents
                );
        void setupAgents();
        float graphFitness() const;
        float teammatesFitness() const;
        float opponentsFitness() const;
        float positionFitness() const;
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
        typedef BGraph::vertex_iterator VertexIterator;
        typedef BGraph::edge_descriptor Edge;
        typedef BGraph::edge_iterator EdgeIterator;
        typedef std::pair<VertexIterator, VertexIterator> VerticesIterator;
        typedef std::pair<EdgeIterator, EdgeIterator> EdgesIterator;
        typedef std::map<std::string, Vertex> VerticesMap;

        float vertexScore(Vertex v) const;

        BGraph m_graph;
        VerticesMap m_vertices;
        std::vector<Agent> m_teammates;
        std::vector<Agent> m_opponents;

};



#endif
