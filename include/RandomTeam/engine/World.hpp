// RandomTeam - World.hpp

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



#ifndef _WORLD_HPP_
#define _WORLD_HPP_



#include <iostream>
#include <memory>
#include <vector>
#include <RandomTeam/graph/SimulationGraph.hpp>



class Agent;



class World
{

    public:

        World();
        virtual ~World();
        unsigned int numAgents() const;
        std::shared_ptr<Agent> agent(unsigned int index) const;
        bool running() const;
        bool started() const;
        const std::string& simulationId() const;
        int maxEdges() const;
        int maxVertices() const;
        int maxSteps() const;
        int step() const;
        void setRunning(bool on);
        void setStep(int step);
        void initSimulation(
                const std::string& id,
                int maxEdges,
                int maxVertices,
                int maxSteps
                );
        void endSimulation(const std::string& rank, const std::string& score);
        void addVertex(const std::string& id, const std::string& team);
        void addEdge(const std::string& n1, const std::string& n2);
        bool loadTeam(const std::string& file);
        int remainingTime() const;
        void think();

    private:

        SimulationGraph m_graph;
        std::vector< std::shared_ptr<Agent> > m_agents;
        bool m_running;
        bool m_started;
        std::string m_simulationId;
        int m_maxEdges;
        int m_maxVertices;
        int m_maxSteps;
        int m_step;

};



#endif
