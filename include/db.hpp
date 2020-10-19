/*
** EPITECH PROJECT, 2020
** Untitled (Workspace)
** File description:
** db
*/

#ifndef DB_HPP_
#define DB_HPP_

#include <iostream>
#include <sqlite3.h>

#define DB_FILE "babel.db"

class database {
    public:
        database();
        ~database();

        bool add(std::string username, std::string password);
        bool del(std::string username);
        std::string contacts(std::string username);
        std::string getUsers(bool print);
        void appendToUsers(std::string);
        void appendToContacts(std::string);
        bool getPrint(void) const;
        bool usernameExist(std::string username);
        bool passwordOK(std::string username, std::string password);
        bool addContact(std::string firstUsername, std::string &secondUsername);
    private:
        sqlite3 *_DB;
        std::string _users;
        std::string _contacts;
        bool _print;
};

#endif /* !DB_HPP_ */
