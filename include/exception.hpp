/*
** EPITECH PROJECT, 2020
** Untitled (Workspace)
** File description:
** Error
*/

#ifndef ERROR_HPP_
#define ERROR_HPP_

#include <iomanip>
#include <iostream>
#include <exception>

class babelException : public std::exception {
    public:
        babelException(std::string m) {
            this->_msg = m;
        };
        std::string what(void) {
            return this->_msg;
        };
    private:
        std::string _msg;
};

#endif /* !ERROR_HPP_ */
