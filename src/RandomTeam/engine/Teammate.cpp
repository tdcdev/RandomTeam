// RandomTeam - Teammate.cpp

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



#include <RandomTeam/engine/Teammate.hpp>
#include <RandomTeam/engine/World.hpp>
#include <RandomTeam/engine/Clock.hpp>
#include <RandomTeam/RandomTeam/tools.hpp>



Teammate::Teammate(
        World* world,
        const std::string& id,
        const std::string& password
        ):
    Agent(id),
    m_world(world),
    m_password(password),
    m_order(""),
    m_connected(false),
    m_requestId("")
{

}



Teammate::Teammate(const Teammate& teammate):
    Agent(teammate),
    m_world(teammate.m_world),
    m_password(teammate.m_password),
    m_order(teammate.m_order),
    m_connected(teammate.m_connected),
    m_requestId(teammate.m_requestId)
{

}



Teammate& Teammate::operator=(const Teammate& teammate)
{
    Agent::operator=(teammate);
    m_world = teammate.m_world;
    m_password = teammate.m_password;
    m_order = teammate.m_order;
    m_connected = teammate.m_connected;
    m_requestId = teammate.m_requestId;

    return *this;
}



Teammate::~Teammate()
{

}



const std::string& Teammate::password() const
{
    return m_password;
}



const std::string& Teammate::order() const
{
    return m_order;
}



bool Teammate::connected() const
{
    return m_connected;
}



const std::string& Teammate::requestId() const
{
    return m_requestId;
}



void Teammate::authrequest()
{
    m_order = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>";
    m_order += "<message type=\"auth-request\"><authentication password=\"";
    m_order += m_password;
    m_order += "\" username=\"";
    m_order += m_id;
    m_order += "\"/></message>";
    m_order += '\0';
}



void Teammate::setActionOrder(const std::string& type, const std::string& param)
{
    m_order = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>";
    m_order += "<message type=\"action\">";
    m_order += "<action ";
    m_order += "id=\"" + m_requestId + "\" ";
    m_order += "type=\"" + type + "\" ";

    if (!param.empty())
    {
        m_order += "param=\"" + m_requestId + "\"";
    }

    m_order += "/>";
    m_order += "</message>";
    m_order += '\0';
}



bool Teammate::performPlayout(unsigned int index)
{
    if (index >= m_playouts.size())
    {
        error("Agent " + m_id + ": index playout out of range");
        return false;
    }

    Playout playout = m_playouts[index];
    ActionPerformer perf = std::get<2>(playout.first);

    perf(*this, playout.second);

    return true;
}



void Teammate::read(const std::string& msg)
{
    tinyxml2::XMLDocument doc;
    std::string msgtrim(msg);

    std::replace(msgtrim.begin(), msgtrim.end(), '\0', ' ');

    if (doc.Parse(msgtrim.c_str()) != tinyxml2::XML_NO_ERROR)
    {
        error("Agent " + m_id + ": parse xml failure");
        return;
    }

    tinyxml2::XMLNode* root = doc.FirstChildElement("message");

    if (root == nullptr)
    {
        error("Agent " + m_id + ": parse xml failure");
        return;
    }

    tinyxml2::XMLElement* message = root->ToElement();

    while (message)
    {
        std::vector<std::string> attributes = {"timestamp", "type"};
        std::vector<std::string> values;

        if (getXMLAttributes(message, attributes, values))
        {
            std::string type(values[1]);

            if(type == "auth-response")
            {
                this->authresponse(message);
            }
            else if(type == "sim-start")
            {
                this->simstart(message);
            }
            else if(type == "sim-end")
            {
                this->simend(message);
            }
            else if(type == "bye")
            {
                this->bye(message);
            }
            else if(type == "request-action")
            {
                this->requestaction(message);
            }
            else
            {
                error("Agent " + m_id + ": unknown message type: " + type);
            }
        }

        message = message->NextSiblingElement("message");
    }
}



void Teammate::authresponse(tinyxml2::XMLElement* message)
{
    debug("Agent " + m_id + ": read auth-response message");

    tinyxml2::XMLElement* authentication;

    if (getXMLElement(message, "authentication", &authentication))
    {
        std::vector<std::string> attributes = {"result"};
        std::vector<std::string> values;

        if (getXMLAttributes(authentication, attributes, values))
        {
            std::string result(values[0]);

            if (result == "ok")
            {
                debug("Agent " + m_id + ": connexion authorized");
                m_connected = true;
            }
            else
            {
                error("Agent " + m_id + ": auth-response=\"" + result + "\"");
                return;
            }
        }
    }
}



void Teammate::simstart(tinyxml2::XMLElement* message)
{
    debug("Agent " + m_id + ": read sim-start message");

    tinyxml2::XMLElement* simulation;

    if (getXMLElement(message, "simulation", &simulation))
    {
        std::vector<std::string> attributes = {
            "id",
            "edges",
            "vertices",
            "steps",
            "role"
        };
        std::vector<std::string> values;

        if (getXMLAttributes(simulation, attributes, values))
        {
            m_world->startSimulation(
                    values[0],
                    std::stoi(values[1]),
                    std::stoi(values[2]),
                    std::stoi(values[3])
                    );

            if (values[4] == "Explorer")
            {
                m_role = Role::EXPLORER;
            }
            else if (values[4] == "Repairer")
            {
                m_role = Role::REPAIRER;
            }
            else if (values[4] == "Saboteur")
            {
                m_role = Role::SABOTEUR;
            }
            else if (values[4] == "Sentinel")
            {
                m_role = Role::SENTINEL;
            }
            else if (values[4] == "Inspector")
            {
                m_role = Role::INSPECTOR;
            }
            else
            {
                m_role = Role::NONE;
                error("Unknown role: " + values[4]);
            }
        }
    }
}



void Teammate::simend(tinyxml2::XMLElement* message)
{
    debug("Agent " + m_id + ": read sim-end message");

    tinyxml2::XMLElement* result;

    if (getXMLElement(message, "sim-result", &result))
    {
        std::vector<std::string> attributes = {"ranking", "score"};
        std::vector<std::string> values;

        if (getXMLAttributes(result, attributes, values))
        {
            m_world->endSimulation(values[0], values[1]);
        }
    }
}



void Teammate::bye(tinyxml2::XMLElement* message)
{
    debug("Agent " + m_id + ": read bye message");

    m_world->setRunning(false);
}



void Teammate::requestaction(tinyxml2::XMLElement* message)
{
    debug("Agent " + m_id + ": read request-action message");

    tinyxml2::XMLElement* perception;

    if (getXMLElement(message, "perception", &perception))
    {
        std::vector<std::string> attributes = {"deadline", "id"};
        std::vector<std::string> values;

        if (getXMLAttributes(perception, attributes, values))
        {
            m_deadline = std::stoll(values[0]);
            m_requestId = values[1];
        }

        this->requestactionSimulation(perception);
        this->requestactionSelf(perception);
        this->requestactionVisibleVertices(perception);
        this->requestactionVisibleEdges(perception);
        this->requestactionVisibleentities(perception);
        this->requestactionProbedVertices(perception);
        this->requestactionSurveyedEdges(perception);
        this->requestactionInspectedEntities(perception);
    }
}



void Teammate::requestactionSimulation(tinyxml2::XMLElement* perception)
{
    tinyxml2::XMLElement* simulation;

    if (getXMLElement(perception, "simulation", &simulation))
    {
        std::vector<std::string> attributes = {"step"};
        std::vector<std::string> values;

        if (getXMLAttributes(simulation, attributes, values))
        {
            m_world->setStep(std::stoi(values[0]));
        }
    }
}



void Teammate::requestactionSelf(tinyxml2::XMLElement* perception)
{
    tinyxml2::XMLElement* self;

    if (getXMLElement(perception, "self", &self))
    {
        std::vector<std::string> attributes = {
            "lastAction",
            "lastActionParam",
            "lastActionResult",
            "position",
            "energy",
            "health",
            "maxEnergy",
            "maxEnergyDisabled",
            "maxHealth",
            "strength",
            "visRange",
            "zoneScore"
        };
        std::vector<std::string> values;

        if (getXMLAttributes(self, attributes, values))
        {
            m_position = values[3];
            m_energy = std::stoi(values[4]);
            m_health = std::stoi(values[5]);
            m_maxEnergy = std::stoi(values[6]);
            m_maxEnergyDisabled = std::stoi(values[7]);
            m_maxHealth = std::stoi(values[8]);
            m_strength = std::stoi(values[9]);
            m_visRange = std::stoi(values[10]);
            m_zoneScore = std::stoi(values[11]);

            debug("Agent " + m_id + ": last " + values[0] + " " + values[2]);
        }
    }
}



void Teammate::requestactionVisibleVertices(tinyxml2::XMLElement* perception)
{
    tinyxml2::XMLElement* vertices;

    if (getXMLElement(perception, "visibleVertices", &vertices))
    {
        tinyxml2::XMLElement* v = vertices->FirstChildElement("visibleVertex");

        while (v)
        {
            std::vector<std::string> attributes = {"name", "team"};
            std::vector<std::string> values;

            if (getXMLAttributes(v, attributes, values))
            {
                m_world->addVertex(values[0], values[1]);
            }

            v = v->NextSiblingElement("visibleVertex");
        }
    }
}



void Teammate::requestactionVisibleEdges(tinyxml2::XMLElement* perception)
{
    tinyxml2::XMLElement* edges;

    if (getXMLElement(perception, "visibleEdges", &edges))
    {
        tinyxml2::XMLElement* e = edges->FirstChildElement("visibleEdge");

        while (e)
        {
            std::vector<std::string> attributes = {"node1", "node2"};
            std::vector<std::string> values;

            if (getXMLAttributes(e, attributes, values))
            {
                m_world->addEdge(values[0], values[1]);
            }

            e = e->NextSiblingElement("visibleEdge");
        }
    }
}



void Teammate::requestactionVisibleentities(tinyxml2::XMLElement* perception)
{
    tinyxml2::XMLElement* entities;

    if (getXMLElement(perception, "visibleEntities", &entities))
    {
        tinyxml2::XMLElement* e = entities->FirstChildElement("visibleEntity");

        while (e)
        {
            std::vector<std::string> attributes = {
                "name",
                "team",
                "node",
                "status"
            };
            std::vector<std::string> values;

            if (getXMLAttributes(e, attributes, values))
            {
                Agent agent(values[0]);

                agent.setTeam(values[1]);
                agent.setPosition(values[2]);
                agent.setDeadline(m_deadline);

                m_world->seeAgent(agent);
            }

            e = e->NextSiblingElement("visibleEntity");
        }
    }
}



void Teammate::requestactionProbedVertices(tinyxml2::XMLElement* perception)
{
    tinyxml2::XMLElement* vertices;

    if (getXMLElement(perception, "probedVertices", &vertices, false))
    {
        tinyxml2::XMLElement* v = vertices->FirstChildElement("probedVertex");

        while (v)
        {
            std::vector<std::string> attributes = {"name", "value"};
            std::vector<std::string> values;

            if (getXMLAttributes(v, attributes, values))
            {
                /* TODO */
            }

            v = v->NextSiblingElement("probedVertex");
        }
    }
}



void Teammate::requestactionSurveyedEdges(tinyxml2::XMLElement* perception)
{
    tinyxml2::XMLElement* edges;

    if (getXMLElement(perception, "surveyedEdges", &edges, false))
    {
        tinyxml2::XMLElement* e = edges->FirstChildElement("surveyedEdge");

        while (e)
        {
            std::vector<std::string> attributes = {"node1", "node2", "weight"};
            std::vector<std::string> values;

            if (getXMLAttributes(e, attributes, values))
            {
                /* TODO */
            }

            e = e->NextSiblingElement("surveyedEdge");
        }
    }
}



void Teammate::requestactionInspectedEntities(tinyxml2::XMLElement* perception)
{
    /* TODO */
}
