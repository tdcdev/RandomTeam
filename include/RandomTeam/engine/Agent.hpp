// RandomTeam - Agent.hpp

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



#ifndef _AGENT_HPP_
#define _AGENT_HPP_



#include <string>



class Agent
{

    public:

        enum Role
        {
            NONE,
            EXPLORER,
            REPAIRER,
            SABOTEUR,
            SENTINEL,
            INSPECTOR
        };

        Agent(const std::string& id);
        Agent(const Agent& agent);
        Agent& operator=(const Agent& agent);
        virtual ~Agent();
        const std::string& id() const;
        const std::string& team() const;
        const std::string& position() const;
        Agent::Role role() const;
        int energy() const;
        int health() const;
        int maxEnergy() const;
        int maxEnergyDisabled() const;
        int maxHealth() const;
        int strength() const;
        int visRange() const;
        int zoneScore() const;
        long long int deadline() const;
        int remainingTime() const;
        void setTeam(const std::string& team);
        void setPosition(std::string& position);
        void setRole(Agent::Role role);
        void setEnergy(int energy);
        void setHealth(int health);
        void setMaxEnergy(int maxEnergy);
        void setMaxEnergyDisabled(int maxEnergyDisabled);
        void setMaxHealth(int maxHealth);
        void setStrength(int strength);
        void setVisRange(int visRange);
        void setZoneScore(int zoneScore);
        void setDeadline(long long int deadline);

    protected:

        std::string m_id;
        std::string m_team;
        std::string m_position;
        Role m_role;
        int m_energy;
        int m_health;
        int m_maxEnergy;
        int m_maxEnergyDisabled;
        int m_maxHealth;
        int m_strength;
        int m_visRange;
        int m_zoneScore;
        long long int m_deadline;

};



#endif
