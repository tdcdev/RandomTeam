// RandomTeam - Teammate.hpp

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



#ifndef _TEAMMATE_HPP_
#define _TEAMMATE_HPP_



#include <RandomTeam/engine/Agent.hpp>
#include <algorithm>
#include <string>



namespace tinyxml2
{

    class XMLElement;

}



class World;



class Teammate: public Agent
{

    public:

        Teammate(World* world,
                const std::string& id,
                const std::string& password
                );
        Teammate(const Teammate& teammate);
        Teammate& operator=(const Teammate& teammate);
        virtual ~Teammate();
        const std::string& password() const;
        const std::string& order() const;
        bool connected() const;
        const std::string& requestId() const;
        void authrequest();
        void setActionOrder(const std::string& type, const std::string& order);
        bool performPlayout(unsigned int index);
        void read(const std::string& msg);

    private:

        void authresponse(tinyxml2::XMLElement* message);
        void simstart(tinyxml2::XMLElement* message);
        void simend(tinyxml2::XMLElement* message);
        void bye(tinyxml2::XMLElement* message);
        void requestaction(tinyxml2::XMLElement* message);
        void requestactionSimulation(tinyxml2::XMLElement* perception);
        void requestactionSelf(tinyxml2::XMLElement* perception);
        void requestactionVisibleVertices(tinyxml2::XMLElement* perception);
        void requestactionVisibleEdges(tinyxml2::XMLElement* perception);
        void requestactionVisibleEntities(tinyxml2::XMLElement* perception);
        void requestactionProbedVertices(tinyxml2::XMLElement* perception);
        void requestactionSurveyedEdges(tinyxml2::XMLElement* perception);
        void requestactionInspectedEntities(tinyxml2::XMLElement* perception);

        World* m_world;
        std::string m_password;
        std::string m_order;
        bool m_connected;
        std::string m_requestId;

};



#endif
