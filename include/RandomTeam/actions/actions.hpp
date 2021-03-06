// RandomTeam - actions.hpp

// Authors:

//     Thomas Da Costa <tdc.input@gmail.com>
//     Pierre Luce-Vayrac <p.luce.vayrac@gmail.com>

// Copyright (C) 2014-2015 Thomas Da Costa and Pierre Luce-Vayrac

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



#ifndef _ACTIONS_HPP_
#define _ACTIONS_HPP_



#include <RandomTeam/graph/SimulationGraph.hpp>
#include <RandomTeam/actions/recharge.hpp>
#include <RandomTeam/actions/goto.hpp>
#include <RandomTeam/actions/survey.hpp>
#include <RandomTeam/actions/probe.hpp>
#include <RandomTeam/actions/attack.hpp>
#include <RandomTeam/actions/parry.hpp>
#include <RandomTeam/actions/repair.hpp>
#include <RandomTeam/actions/inspect.hpp>



typedef void (*ActionGenerator)(
        const Agent&,
        const SimulationGraph&,
        std::vector<std::string>&
        );
typedef bool (*ActionSimulator)(
        const Agent&,
        const std::string&,
        SimulationGraph&
        );
typedef void (*ActionPerformer)(Teammate&, const std::string&);
typedef std::tuple<ActionGenerator, ActionSimulator, ActionPerformer> Action;
typedef std::vector<Action> ActionsVector;
typedef std::pair<Action, std::string> Playout;
typedef std::vector<Playout> PlayoutsVector;



#endif
