/*
** EPITECH PROJECT, 2020
** Untitled (Workspace)
** File description:
** Server
*/

#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>

#include "session.hpp"

class server
{
    public:
        server(boost::asio::io_context& io_context, short port) : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
            this->_p = new Parser;
            do_accept();
        }
        ~server() { delete this->_p; }
        static std::string call(std::vector<std::string> &cont, User &sender);
        static std::string resp(std::vector<std::string> &cont, User &sender);

    private:
        void do_accept();
        size_t reg_connection(std::weak_ptr<session> wp, tcp::endpoint);
        size_t broadcast(std::string const& msg);
        tcp::acceptor acceptor_;
        Parser *_p;
};

#endif /* !SERVER_HPP_ */
