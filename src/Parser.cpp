/*
** EPITECH PROJECT, 2020
** Untitled (Workspace)
** File description:
** Parser
*/

#include "../include/Parser.hpp"
#include "../include/Server.hpp"

Parser::Parser()
{
}

Parser::~Parser()
{
}

size_t Parser::handleLogin(User &u, char *data_)
{
    std::string str(data_);
    std::vector<std::string> cont;
    if (str.length() > 0)
        str.pop_back();
    boost::split(cont, str, boost::is_any_of(" "));

    if (cont[0] != COMMAND_LOGIN && cont[0] != COMMAND_ADD_USER) {
        std::strcpy(data_, ERROR_LOG_COMMAND_UNKNOWN);
        return (std::strlen(ERROR_LOG_COMMAND_UNKNOWN));
    }
    if (cont.size() != 3) {
        std::strcpy(data_, ERROR_LOG_NOT_ENOUGH_PARAMETERS);
        return (std::strlen(ERROR_LOG_NOT_ENOUGH_PARAMETERS));
    }
    if (cont[0] == COMMAND_ADD_USER)
        return (this->commandAdd(u, cont, data_));
    if (this->_db.usernameExist(cont[1])) {
        u.setUsername(cont[1]);
        std::cout << "Username set to " << cont[1] << std::endl;
        if (this->_db.passwordOK(cont[1], cont[2])) {
            u.setLogged(true);
            std::strcpy(data_, LOG_OK);
            return (std::strlen(LOG_OK));
        } else {
            std::strcpy(data_, ERROR_LOG_BAD_PASSWORD);
            return (std::strlen(ERROR_LOG_BAD_PASSWORD));
        }
    } else {
        std::strcpy(data_, ERROR_LOG_USER_DONT_EXIST);
        return (std::strlen(ERROR_LOG_USER_DONT_EXIST));
    }
    return (str.length());
}

size_t Parser::commandAdd(User &u, std::vector<std::string> &cont, char *data_)
{
    if (cont.size() < 3) {
        std::strcpy(data_, ERROR_LOG_NOT_ENOUGH_PARAMETERS);
        return (std::strlen(data_));
    }
    this->_db.add(cont[1], cont[2]);
    std::strcpy(data_, COMMAND_OK);
    return (std::strlen(data_));
}

size_t Parser::commandDel(User &u, std::vector<std::string> &cont, char *data_)
{
    if (cont.size() < 2) {
        std::strcpy(data_, ERROR_LOG_NOT_ENOUGH_PARAMETERS);
        return (std::strlen(data_));
    }
    this->_db.del(cont[1]);
    std::strcpy(data_, COMMAND_OK);
    return (std::strlen(data_));
}

size_t Parser::commandCall(User &u, std::vector<std::string> &cont, char *data_)
{
    if (cont.size() < 3) {
        std::strcpy(data_, ERROR_LOG_NOT_ENOUGH_PARAMETERS);
        return (std::strlen(data_));
    }
    std::strcpy(data_, server::call(cont, u).c_str());
    return (std::strlen(data_));
}

size_t Parser::commandRespcall(User &u, std::vector<std::string> &cont, char *data_)
{
    //if (cont.size() < 4) {
    //    std::strcpy(data_, ERROR_LOG_NOT_ENOUGH_PARAMETERS);
    //    return (std::strlen(data_));
    //}
    std::strcpy(data_, server::resp(cont, u).c_str());
    return (std::strlen(data_));
}

size_t Parser::commandContacts(User &u, std::vector<std::string> &cont, char *data_)
{
    if (cont.size() < 2) {
        std::strcpy(data_, ERROR_LOG_NOT_ENOUGH_PARAMETERS);
        return (std::strlen(data_));
    }
    std::strcpy(data_, this->_db.contacts(cont[1]).c_str());
    return (std::strlen(data_));
}

size_t Parser::commandAddContact(User &u, std::vector<std::string> &cont, char *data_)
{
    if (cont.size() < 2) {
        std::strcpy(data_, ERROR_LOG_NOT_ENOUGH_PARAMETERS);
        return (std::strlen(data_));
    }
    if (cont[1] == u.getUsername()) {
        std::strcpy(data_, ERROR_CONTACT_IS_YOU);
        return (std::strlen(data_));
    }
    if (this->_db.addContact(u.getUsername(), cont[1])) {
        std::strcpy(data_, COMMAND_OK);
        return (std::strlen(data_));
    } else {
        std::strcpy(data_, ERROR_ADD_CONTACT);
        return (std::strlen(data_));
    }
}

size_t Parser::handleCommands(User &u, std::vector<std::string> &cont, char *data_)
{
    std::vector<std::pair<std::string, size_t (Parser::*)(User &, std::vector<std::string> &, char *)>> commands;

    commands.push_back(std::pair<std::string, size_t (Parser::*)(User &, std::vector<std::string> &, char *)>(COMMAND_ADD_USER, &Parser::commandAdd));
    commands.push_back(std::pair<std::string, size_t (Parser::*)(User &, std::vector<std::string> &, char *)>(COMMAND_DEL_USER, &Parser::commandDel));
    commands.push_back(std::pair<std::string, size_t (Parser::*)(User &, std::vector<std::string> &, char *)>(COMMAND_CALL, &Parser::commandCall));
    commands.push_back(std::pair<std::string, size_t (Parser::*)(User &, std::vector<std::string> &, char *)>(COMMAND_CALL_RESP, &Parser::commandRespcall));
    commands.push_back(std::pair<std::string, size_t (Parser::*)(User &, std::vector<std::string> &, char *)>(COMMAND_CONTACTS, &Parser::commandContacts));
    commands.push_back(std::pair<std::string, size_t (Parser::*)(User &, std::vector<std::string> &, char *)>(COMMAND_ADD_CONTACT, &Parser::commandAddContact));

    for (auto it = commands.begin(); it != commands.end(); it++) {
        if ((*it).first == cont[0]) {
            size_t (Parser::*ptr)(User &, std::vector<std::string> &, char *) = it->second;
            return ((this->*ptr)(u, cont, data_));
        }
    }
    std::strcpy(data_, ERROR_UNKNOWN_COMMAND);
    return (std::strlen(ERROR_UNKNOWN_COMMAND));
}

size_t Parser::run(User &u, char *data_)
{
    std::string str(data_);
    std::vector<std::string> cont;
    size_t length = str.length();

    std::cout << "reveiced : [" << str << "]" << std::endl;
    if (!u.isLogged())
        return (this->handleLogin(u, data_));
    if (str.length() > 0)
        str.pop_back();
    boost::split(cont, str, boost::is_any_of(" "));
    if (cont[0] == "help") {
        std::cout << "help called" << std::endl;
        std::strcpy(data_, "man google\n");
        length = std::strlen("man google\n");
    }
    return (this->handleCommands(u, cont, data_));
}