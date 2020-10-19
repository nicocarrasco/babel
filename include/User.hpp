/*
** EPITECH PROJECT, 2020
** Untitled (Workspace)
** File description:
** User
*/

#ifndef USER_HPP_
#define USER_HPP_

#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>

class User
{
    public:
        User() {
            _username = "Anonymous";
            _logged = false;
        }
        ~User() {}
        std::string getUsername(void) const { return this->_username; };
        void setUsername(std::string &s) { this->_username.assign(s); };
        bool isLogged(void) const { return (this->_logged); };
        void setLogged(bool v) { this->_logged = v;};

    private:
        std::string _username;
        bool _logged;
};

#endif /* !USER_HPP_ */
