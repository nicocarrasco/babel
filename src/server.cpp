/*
** EPITECH PROJECT, 2020
** Untitled (Workspace)
** File description:
** server
*/

#include "../include/Server.hpp"
#include "../include/session.hpp"

std::vector<std::pair<std::weak_ptr<session>, tcp::endpoint>> _registered;
std::mutex _mx;

// call <username> <port>                    call <username> on port <port>
// --> OK
// --> call <usernameCaller> <port> <ip>
// callresp <username> <response/port>       reponse (Yes/No) to call with port (for Yes)
// --> OK
// --> callresp <OK/KO> [<port> <ip>]

std::string server::call(std::vector<std::string> &cont, User &sender)
{
    std::vector<std::pair<std::shared_ptr<session>, tcp::endpoint>> active;
    std::lock_guard<std::mutex> lk(_mx);
    tcp::endpoint ep;
    bool sent = false;

    for (auto &w : _registered)
        if (auto c = w.first.lock())
            active.push_back(std::pair<std::shared_ptr<session>, tcp::endpoint>(c, w.second));
    for (auto& c : active)
        if (c.first->get_user().getUsername() == sender.getUsername())
            ep = c.second;
    for (auto& c : active) {
        if (c.first->get_user().getUsername() == cont[1]) {
            std::cout << "Sending call request to " << c.first->get_user().getUsername() << std::endl;
            std::string toSend = cont[0] + " " + sender.getUsername() + " " + cont[2] + " " + ep.address().to_string() + "\n";
            c.first->send(toSend);
            sent = true;
        }
    }
    if (!sent)
        return (ERROR_CALL);
    return (COMMAND_OK);
}

std::string server::resp(std::vector<std::string> &cont, User &sender)
{
    std::vector<std::pair<std::shared_ptr<session>, tcp::endpoint>> active;
    std::lock_guard<std::mutex> lk(_mx);
    tcp::endpoint ep;
    bool sent = false;

    for (auto &w : _registered)
        if (auto c = w.first.lock())
            active.push_back(std::pair<std::shared_ptr<session>, tcp::endpoint>(c, w.second));
    for (auto& c : active)
        if (c.first->get_user().getUsername() == sender.getUsername())
            ep = c.second;
    for (auto& c : active) {
        if (c.first->get_user().getUsername() == cont[1]) {
            std::cout << "Sending resp request to " << c.first->get_user().getUsername() << std::endl;
            std::string toSend = cont[0] + " " + sender.getUsername() + " " + cont[2] + " " + cont[3] + " " + ep.address().to_string() + "\n";
            c.first->send(toSend);
            sent = true;
        }
    }
    if (!sent)
        return (ERROR_CALL);
    return (COMMAND_OK);
}

void server::do_accept()
{
    acceptor_.async_accept(
    [this](boost::system::error_code ec, tcp::socket socket)
    {
        if (!ec) {
            std::cout << "new Connection accepted" << std::endl;
            tcp::endpoint ep = socket.remote_endpoint();
            std::cout << "remote endpoint : " << boost::lexical_cast<std::string>(socket.remote_endpoint()) << std::endl;
            std::cout << "local endpoint  : " << boost::lexical_cast<std::string>(socket.local_endpoint()) << std::endl;
            auto s = std::make_shared<session>(std::move(socket));
            s->start(this->_p);
            auto n = reg_connection(s, ep);
            //broadcast("player #" + std::to_string(n) + " has entered the game\n");
        }
        do_accept();
    });
}

size_t server::reg_connection(std::weak_ptr<session> wp, tcp::endpoint ep)
{
    std::lock_guard<std::mutex> lk(_mx);
    _registered.push_back(std::pair<std::weak_ptr<session>, tcp::endpoint>(wp, ep));
    return _registered.size();
}

size_t server::broadcast(std::string const &msg)
{
    std::vector<std::shared_ptr<session>> active;
    std::lock_guard<std::mutex> lk(_mx);
    for (auto& w : _registered)
        if (auto c = w.first.lock())
            active.push_back(c);

    for (auto& c : active)
        c->send(msg);

    return active.size();
}