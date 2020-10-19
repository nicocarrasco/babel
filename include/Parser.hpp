/*
** EPITECH PROJECT, 2020
** Untitled (Workspace)
** File description:
** Parser
*/

#ifndef PARSER_HPP_
#define PARSER_HPP_

#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <utility>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>

#include "../include/User.hpp"
#include "../include/db.hpp"

#define COMMAND_LOGIN "login"  // username for login
#define COMMAND_ADD_USER "add"       // add a new user
#define COMMAND_DEL_USER "del"       // del a user
#define COMMAND_CALL "call"          // call a user
#define COMMAND_CALL_RESP "resp" // answer a call
#define COMMAND_CONTACTS "contacts"  // ask for list of contact
#define COMMAND_ADD_CONTACT "addContact" // add a contact to your contactlist

#define ERROR_LOG_COMMAND_UNKNOWN "Error : Unknown command for logging\n"
#define ERROR_LOG_NOT_ENOUGH_PARAMETERS "Error : Not enough parameter for this function\n"
#define ERROR_LOG_USER_DONT_EXIST "Error : This user Doesn't exist\n"
#define ERROR_LOG_BAD_PASSWORD "Error : Wrong password for this username\n"
#define ERROR_UNKNOWN_COMMAND "Error : Unknown command\n"
#define ERROR_CALL "Error : couldn't find user\n"
#define ERROR_ADD_CONTACT "Error : User doesn't exist or already in contact\n"
#define ERROR_CONTACT_IS_YOU "Error : you can't add yourself in your contact list\n"
#define COMMAND_OK "OK\n"
#define LOG_OK "OK\n"

// LOGIN function
// login <username> <password>
// OK

// LOOP functions
// add <username> <password>                                          add <username> to DB
// --> OK
// del <username>                                                     delete <username> from DB
// --> OK
// call <usernameTarget> <port>                                       call <username> on port <port>
// --> to Caller : OK
// --> to Target : call <usernameCaller> <port> <ip>
// resp <usernameTarget> <response> [<port>]                          reponse : Yes/No
// --> to Caller : OK
// --> to Target : callresp <usernameCaller> <OK/KO> [<port> <ip>]
// contacts <username>                                                contact list of <username>
// --> SECOND_USERNAME=user1;SECOND_USERNAME=user2
// addContact <username>                                              add <username> to your contact list
// --> OK

class Parser
{
    public:
        Parser();
        ~Parser();

        size_t run(User &u, char *data_);
        size_t handleLogin(User &u, char *data_);
        size_t handleCommands(User &u, std::vector<std::string> &cont, char *data_);

        size_t commandAdd(User &u, std::vector<std::string> &cont, char *data_);
        size_t commandDel(User &u, std::vector<std::string> &cont, char *data_);
        size_t commandCall(User &u, std::vector<std::string> &cont, char *data_);
        size_t commandRespcall(User &u, std::vector<std::string> &cont, char *data_);
        size_t commandContacts(User &u, std::vector<std::string> &cont, char *data_);
        size_t commandAddContact(User &u, std::vector<std::string> &contact, char *data_);
    private:
        database _db;
};

#endif /* !PARSER_HPP_ */
