#include "client.h"

#include <iostream>
#include <ctime>

Client* ClientControl::createClient (int sd)
{
    // Поиск элемента (вдруг уже создан)
    auto cl = findClient (sd);
    // Если не создан, то создаем
    if (!cl)
    {
        cl = new Client;
        cl->setSd (sd);
        clients.push_back (cl);
    }
    return cl;
}

Client* ClientControl::findClient (int sd)
{
    for (size_t i = 0; i < clients.size (); i++)
    {
        if (clients[i]->getSd () == sd)
            return clients[i];
    }

    return nullptr;
}

void ClientControl::deleteClient (int sd)
{
    for (size_t i = 0; i < clients.size (); i++)
    {
        if (clients[i]->getSd () == sd)
        {
            delete clients[i];
            clients.erase (clients.begin() + i);
        }
    }
}
