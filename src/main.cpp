/*
** EPITECH PROJECT, 2020
** Untitled (Workspace)
** File description:
** main
*/

#include "../include/Server.hpp"
#include "../include/User.hpp"
#include "../include/Parser.hpp"
#include "../include/db.hpp"
#include "../include/session.hpp"
#include "../include/exception.hpp"

int main(int argc, char* argv[])
{
    try {
        if (argc != 2) {
            std::cerr << "Usage: " << argv[0] << " <port>\n";
            return 84;
        }
        boost::asio::io_context io_context;
        server s(io_context, std::atoi(argv[1]));
        io_context.run();
    }
    catch (babelException &e) {
        std::cerr << "Exception catch : " << e.what() << "\n";
    }
    catch (std::exception& e) {
        std::cerr << "Exception catch: " << e.what() << "\n";
    }
  return 0;
}