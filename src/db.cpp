/*
** EPITECH PROJECT, 2020
** Untitled (Workspace)
** File description:
** db
*/

#include "../include/db.hpp"
#include "../include/exception.hpp"

database::database()
{
    std::string user = "CREATE TABLE USER("
                       "ID INTEGER PRIMARY KEY     AUTOINCREMENT,"
                       "USERNAME           TEXT    NOT NULL,"
                       "PASSWORD           TEXT    NOT NULL);";
    std::string contacts = "CREATE TABLE CONTACTS("
                           "ID               INTEGER PRIMARY KEY AUTOINCREMENT,"
                           "FIRST_USERNAME   TEXT                NOT NULL,"
                           "SECOND_USERNAME  TEXT                NOT NULL);";

    this->_print = false;
    this->_users.clear();
    if (sqlite3_open(DB_FILE, &this->_DB)) {
        std::cerr << "Error Opening " << DB_FILE << std::endl;
        throw babelException("Error Opening Database");
    } else
        std::cout << "Database Opened successfully" << std::endl;
    char *messageError;
    if (sqlite3_exec(this->_DB, user.c_str(), NULL, 0, &messageError) != SQLITE_OK ||
        sqlite3_exec(this->_DB, contacts.c_str(), NULL, 0, &messageError) != SQLITE_OK) {
        std::cerr << "Error Creating Tables in Database (already exist ?)" << std::endl;
        sqlite3_free(messageError);
        //throw (babelException("Error Creating Table in DB"));
    } else
        std::cout << "Tables created in Database" << std::endl;
    std::cout << "User list in Database :" << std::endl;
    this->getUsers(true);
}

database::~database()
{
    sqlite3_close(this->_DB);
}

bool database::add(std::string username, std::string password)
{
    char *messageError;
    std::string s = "INSERT INTO USER (USERNAME, PASSWORD) VALUES ('";
    s.append(username);
    s.append("', '");
    s.append(password);
    s.append("');");

    if (this->usernameExist(username)) {
        std::cerr << "Error : This User already exist (" << username << ")" << std::endl;
        return (false);
    }
    if (sqlite3_exec(this->_DB, s.c_str(), NULL, 0, &messageError) != SQLITE_OK) {
        std::cerr << "Error Adding User to Database" << std::endl;
        sqlite3_free(messageError);
        throw babelException("Error Adding User to Database");
        return (false);
    }
    std::cout << "User '" << username << "' added to Database" << std::endl;
    return (true);
}

bool database::del(std::string username)
{
    char *messageError;
    std::string s = "DELETE FROM USER WHERE USERNAME LIKE '";
    s.append(username);
    s.append("';");

    if (sqlite3_exec(this->_DB, s.c_str(), NULL, 0, &messageError) != SQLITE_OK) {
        std::cerr << "Error Removing User From Database" << std::endl;
        sqlite3_free(messageError);
        throw babelException("Error Removing User From Database");
        return (false);
    }
    std::cout << "User '" << username << "' removed from Database" << std::endl;
    return (true);
}

void database::appendToUsers(std::string s)
{
    this->_users.append(s);
}

void database::appendToContacts(std::string s)
{
    this->_contacts.append(s);
}

bool database::getPrint(void) const
{
    return (this->_print);
}

static int callback_userlist(void* obj, int argc, char** argv, char** azColName) 
{
    int i;
    database *db = static_cast<database *>(obj);

    for (i = 0; i < argc - 1; i++) { 
        if (db->getPrint())
            printf("%s=%s|", azColName[i], argv[i] ? argv[i] : "NULL"); 
        db->appendToUsers(azColName[i]);
        db->appendToUsers("=");
        db->appendToUsers(argv[i] ? argv[i] : "NULL");
        db->appendToUsers("|");
    }
    if (db->getPrint())
        printf("%s=%s;", azColName[argc - 1], argv[argc - 1] ? argv[argc - 1] : "NULL");
    db->appendToUsers(azColName[argc - 1]);
    db->appendToUsers("=");
    db->appendToUsers(argv[argc - 1] ? argv[argc - 1] : "NULL");
    db->appendToUsers(";");
    return 0;
}

std::string database::getUsers(bool print)
{
    std::string s = "SELECT * FROM USER;";
    this->_users.clear();

    this->_print = print;
    if (sqlite3_exec(this->_DB, s.c_str(), callback_userlist, this, NULL) != SQLITE_OK) {
        std::cerr << "Error Getting Users From DB" << std::endl;
        throw babelException("Error Getting Users From DB");
    }
    if (this->_users.empty())
        return ("\n");
    if (this->_print)
        printf("\n");
    this->_users.pop_back();
    this->_users.push_back('\n');
    return (this->_users);
}

bool database::usernameExist(std::string username)
{
    std::string s = this->getUsers(false);
    std::string cmp("USERNAME=");
    cmp.append(username);
    cmp.append("|");

    if (s.find(cmp) != std::string::npos)
        return (true);
    return (false);
}

bool database::passwordOK(std::string username, std::string password)
{
    std::string s = this->getUsers(false);
    std::string cmp("USERNAME=");
    cmp.append(username);
    cmp.append("|PASSWORD=");
    cmp.append(password);
    cmp.append(";");
    std::string cmp2("USERNAME=");
    cmp2.append(username);
    cmp2.append("|PASSWORD=");
    cmp2.append(password);
    cmp2.append("\n");

    if (s.find(cmp) != std::string::npos || s.find(cmp2) != std::string::npos)
        return (true);
    return (false);
}

static int callback_contacts(void* obj, int argc, char** argv, char** azColName)
{
    // get SECOND_USERNAME in db->appendToContact
    int i;
    database *db = static_cast<database *>(obj);

    for (i = 0; i < argc - 1; i++) { 
        //if (db->getPrint())
        //    printf("%s=%s|", azColName[i], argv[i] ? argv[i] : "NULL"); 
        db->appendToContacts(azColName[i]);
        db->appendToContacts("=");
        db->appendToContacts(argv[i] ? argv[i] : "NULL");
        db->appendToContacts("|");
    }
    //if (db->getPrint())
    //    printf("%s=%s;", azColName[argc - 1], argv[argc - 1] ? argv[argc - 1] : "NULL");
    db->appendToContacts(azColName[argc - 1]);
    db->appendToContacts("=");
    db->appendToContacts(argv[argc - 1] ? argv[argc - 1] : "NULL");
    db->appendToContacts(";");
    return (0);
}

std::string database::contacts(std::string username)
{
    std::string s = "SELECT SECOND_USERNAME FROM CONTACTS WHERE FIRST_USERNAME = '" + username + "';";
    this->_contacts.clear();

    // OU : faire une table avec juste les username et les lier entre eu : charcher le username dans FIRST et retourner les SECOND
    if (sqlite3_exec(this->_DB, s.c_str(), callback_contacts, this, NULL) != SQLITE_OK) {
        std::cerr << "Error Getting Contact List From DB" << std::endl;
        throw babelException("Error Getting Contact List From DB");
    }
    if (this->_contacts.empty())
        return ("\n");
    this->_contacts.pop_back();
    this->_contacts.push_back('\n');
    return (this->_contacts);
}

bool database::addContact(std::string firstUsername, std::string &secondUsername)
{
    if (!this->usernameExist(firstUsername) || !this->usernameExist(secondUsername))
        return (false);
    std::string s = "SECOND_USERNAME=";
    s.append(secondUsername);
    if (this->contacts(firstUsername).find(s) != std::string::npos)
        return (false);
    std::string c = "INSERT INTO CONTACTS (FIRST_USERNAME, SECOND_USERNAME) VALUES ('";
    c.append(firstUsername);
    c.append("', '");
    c.append(secondUsername);
    c.append("');");
    if (sqlite3_exec(this->_DB, c.c_str(), NULL, 0, NULL) != SQLITE_OK) {
        std::cerr << "Error Adding contact to DB : " << c << std::endl;
        throw babelException("Error Adding contact to DB");
    }
    std::cout << secondUsername << " Added to " << firstUsername << "'s Contact List" << std::endl;
    return (true);
}