// RandomTeam - TCPClient.cpp

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



#include <RandomTeam/network/TCPClient.hpp>
#include <RandomTeam/network/TCPNode.hpp>
#include <RandomTeam/network/TCPException.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>



TCPClient::TCPClient()
{

}



TCPClient::~TCPClient()
{

}



const TCPNode& TCPClient::node() const
{
    if (m_node == nullptr)
    {
        throw TCPException("TCPClient are not connected");
    }

    return *m_node.get();
}



void TCPClient::setNodeMsgToSend(const std::string& msg)
{
    if (m_node == nullptr)
    {
        throw TCPException("TCPClient are not connected");
    }

    m_node->setMsgToSend(msg);
}



bool TCPClient::connect(const std::string& ip, unsigned short port)
{
    try
    {
        std::unique_ptr<boost::asio::ip::tcp::socket> socket;
        boost::asio::ip::tcp::resolver resolver(m_ios);
        boost::asio::ip::tcp::resolver::query query(ip, std::to_string(port));
        boost::asio::ip::tcp::resolver::iterator it(resolver.resolve(query));

        socket.reset(new boost::asio::ip::tcp::socket(m_ios));
        boost::asio::connect(*socket, it);
        m_node.reset(new TCPNode(std::move(socket)));
    }
    catch (...)
    {
        m_node.reset(nullptr);

        return false;
    }

    return true;
}



bool TCPClient::read(int ms)
{
    if (m_node == nullptr)
    {
        throw TCPException("TCPClient are not connected");
    }

    boost::asio::io_service ios;
    boost::asio::deadline_timer timer(ios);

    timer.expires_from_now(
            boost::posix_time::milliseconds(ms)
            );

    timer.async_wait(boost::bind(
                &TCPClient::cancelNodeHandler,
                this,
                boost::asio::placeholders::error
                ));

    boost::thread thread(boost::bind(&boost::asio::io_service::run, &ios));

    m_node->asyncRead();
    m_ios.run();
    timer.cancel();
    thread.join();
    m_ios.reset();

    return !m_node->input().empty();
}




bool TCPClient::write(int ms)
{
    if (m_node == nullptr)
    {
        throw TCPException("TCPClient are not connected");
    }

    boost::asio::io_service ios;
    boost::asio::deadline_timer timer(ios);

    timer.expires_from_now(
            boost::posix_time::milliseconds(ms)
            );

    timer.async_wait(boost::bind(
                &TCPClient::cancelNodeHandler,
                this,
                boost::asio::placeholders::error
                ));

    boost::thread thread(boost::bind(&boost::asio::io_service::run, &ios));

    m_node->asyncWrite();
    m_ios.run();
    timer.cancel();
    thread.join();
    m_ios.reset();

    return m_node->msgSent();
}



void TCPClient::cancelNodeHandler(const boost::system::error_code& e)
{
    if (!e)
    {
        if (m_node == nullptr)
        {
            throw TCPException("TCPClient are not connected");
        }

        m_node->cancel();
    }
}
