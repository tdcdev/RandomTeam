// RandomTeam - TCPClient.hpp

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



#ifndef _TCP_CLIENT_H_
#define _TCP_CLIENT_H_



#include <memory>
#include <boost/asio.hpp>



class TCPNode;



class TCPClient
{

    public:

        TCPClient();
        virtual ~TCPClient();
        const TCPNode& node() const;
        void setNodeMsgToSend(const std::string& msg);
        bool connect(const std::string& ip, unsigned short port);
        bool read(int ms);
        bool write(int ms);

    private:

        void cancelNodeHandler(const boost::system::error_code& e);

        boost::asio::io_service m_ios;
        std::unique_ptr<TCPNode> m_node;

};



#endif
