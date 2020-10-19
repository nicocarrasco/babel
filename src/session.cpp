/*
** EPITECH PROJECT, 2020
** Untitled (Workspace)
** File description:
** session
*/

#include "../include/session.hpp"

session::~session()
{
    std::cout << "Connection closed" << std::endl;
}

void session::start(Parser *p)
{
    this->_p = p;
    do_read();
}

void session::send(std::string msg)
{
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(msg, msg.length()),
    [this, self](boost::system::error_code ec, std::size_t /*length*/) {
    });
}

void session::do_read()
{
    auto self(shared_from_this());

    socket_.async_read_some(boost::asio::buffer(data_, MAX_LENGTH),
    [this, self](boost::system::error_code ec, std::size_t length) {
        if (!ec) {
            data_[length] = 0;
            do_write(this->_p->run(this->_user, data_));
        }
    });
}

void session::do_write(std::size_t length)
{
    auto self(shared_from_this());

    std::cout << "response : [" << data_ << "]" << std::endl;
    boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
    [this, self](boost::system::error_code ec, std::size_t /*length*/) {
        if (!ec) {
            do_read();
        }
    });
}