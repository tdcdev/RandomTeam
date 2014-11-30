// RandomTeam - TCPNode.cpp

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



#include <RandomTeam/network/TCPNode.hpp>
#include <boost/bind.hpp>



TCPNode::TCPNode(std::unique_ptr<boost::asio::ip::tcp::socket> socket):
    m_socket(std::move(socket)),
    m_alive(true),
    m_msgSent(false)
{

}



TCPNode::~TCPNode()
{

}



bool TCPNode::alive() const
{
    return m_alive;
}



const std::string& TCPNode::input() const
{
    return m_input;
}


const std::string& TCPNode::msgToSend() const
{
    return m_msgToSend;
}



bool TCPNode::msgSent() const
{
    return m_msgSent;
}



void TCPNode::setMsgToSend(const std::string& msg)
{
    m_msgToSend = msg;
}



void TCPNode::asyncRead()
{
    m_socket->async_read_some(
            boost::asio::buffer(
                m_buffer,
                TCP_NODE_BUFFER_SIZE
                ),
            boost::bind(
                &TCPNode::asyncReadHandler,
                this,
                boost::asio::placeholders::bytes_transferred,
                boost::asio::placeholders::error
                )
            );
}



void TCPNode::asyncWrite()
{
    m_msgSent = false;

    m_socket->async_write_some(
            boost::asio::buffer(
                m_msgToSend,
                TCP_NODE_BUFFER_SIZE
                ),
            boost::bind(
                &TCPNode::asyncWriteHandler,
                this,
                boost::asio::placeholders::bytes_transferred,
                boost::asio::placeholders::error
                )
            );
}



void TCPNode::cancel()
{
    m_socket->cancel();
}



void TCPNode::asyncReadHandler(size_t len, const boost::system::error_code& e)
{
    if (!e)
    {
        m_input.assign(m_buffer, len);
    }
    else
    {
        m_input.clear();

        if (e == boost::asio::error::eof)
        {
            m_alive = false;
        }
    }
}



void TCPNode::asyncWriteHandler(size_t len, const boost::system::error_code& e)
{
    if (!e)
    {
        m_msgSent = true;
    }
}
