// RandomTeam - TCPNode.hpp

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



#ifndef _TCP_NODE_H_
#define _TCP_NODE_H_



#define TCP_NODE_BUFFER_SIZE 65536



#include <memory>
#include <boost/asio.hpp>



namespace boost
{

    class thread;

}



class TCPNode
{

    public:

        TCPNode(std::unique_ptr<boost::asio::ip::tcp::socket> socket);
        virtual ~TCPNode();
        bool alive() const;
        const std::string& input() const;
        const std::string& msgToSend() const;
        bool msgSent() const;
        void setMsgToSend(const std::string& msg);
        void asyncRead();
        void asyncWrite();
        void cancel();

    private:

        void asyncReadHandler(size_t len, const boost::system::error_code& e);
        void asyncWriteHandler(size_t len, const boost::system::error_code& e);

        std::unique_ptr<boost::asio::ip::tcp::socket> m_socket;
        char m_buffer[TCP_NODE_BUFFER_SIZE];
        bool m_alive;
        std::string m_input;
        std::string m_msgToSend;
        bool m_msgSent;

};



#endif
