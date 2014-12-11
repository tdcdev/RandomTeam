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



#include <vector>
#include <random>
#include <RandomTeam/engine/Teammate.hpp>
#include <RandomTeam/graph/SimulationGraph.hpp>



class World
{

    public:

        World();
        virtual ~World();
        unsigned int nbTeammates() const;
        Teammate* teammate(unsigned int index);
        bool running() const;
        bool started() const;
        const std::string& simulationId() const;
        int maxEdges() const;
        int maxVertices() const;
        int maxSteps() const;
        int step() const;
        const std::string& team() const;
        int remainingTime() const;
        void setRunning(bool on);
        void setStep(int step);
        void setTeam(const std::string& team);
        void startSimulation(
                const std::string& simulationId,
                int maxEdges,
                int maxVertices,
                int maxSteps
                );
        void endSimulation(const std::string& rank, const std::string& score);
        void seeAgent(const Agent& agent);
        void addVertex(const std::string& id, const std::string& team);
        void addEdge(const std::string& n1, const std::string& n2);
        void updateVertex(const std::string& id, int v);
        void updateEdge(const std::string& n1, const std::string& n2, int w);
        bool loadTeam(const std::string& file);
        void clear();
        void generateAllPlayouts();
        void thinkThread(const std::vector<int>& solution, float& score) const;
        void think(unsigned int nbThreads);
        void perform();
        static void mutate(
                const std::vector<int>& solution,
                const std::vector<Teammate>& teammates,
                std::vector<int>& mutate
                );

    private:

        static std::mt19937 s_randGen;

        SimulationGraph m_graph;
        std::vector<Teammate> m_teammates;
        std::vector<Agent> m_opponents;
        bool m_running;
        bool m_started;
        std::string m_simulationId;
        int m_maxEdges;
        int m_maxVertices;
        int m_maxSteps;
        int m_step;
        std::string m_team;
        std::vector<int> m_solution;
        float m_fitness;

};



#endif
