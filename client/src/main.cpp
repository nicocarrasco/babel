/*
** EPITECH PROJECT, 2020
** B-CPP-500-MAR-5-1-babel-thibault.schmitt
** File description:
** main
*/

#include <QtWidgets/QApplication>
#include "../include/login.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <server-ip> <port>\n";
        return 84;
    }
    QString ip(argv[1]);
    QString port(argv[2]);
    Login w(port, ip);
    w.show();
    return app.exec();
}
