/*
** EPITECH PROJECT, 2020
** Untitled (Workspace)
** File description:
** session
*/

#ifndef SESSION_HPP_
#define SESSION_HPP_

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

#define MAX_LENGTH 1024
using boost::asio::ip::tcp;

#include "User.hpp"
#include "Parser.hpp"

class session : public std::enable_shared_from_this<session>
{
    public:
        session(tcp::socket socket) : socket_(std::move(socket)) {}
        ~session();
        void start(Parser *p);
        void send(std::string msg);
        User &get_user(void) { return (this->_user);}

    private:
        void do_read();
        void do_write(std::size_t length);

        tcp::socket socket_;
        char data_[MAX_LENGTH] = {0};
        User _user;
        Parser *_p;
};

#endif /* !SESSION_HPP_ */
