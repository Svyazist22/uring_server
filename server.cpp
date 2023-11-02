#include "server.h"

#include <iostream>
#include <string.h>

bool Server::create ()
{
    static constexpr uint16_t maxClients = 512;

    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd <= 0)
    {
        printf ("ERROR: don't create socket!\r\n");
        return false;
    }

    memset (&sockAddress, 0, sizeof (sockAddress));
    sockAddress.sin_family = AF_INET;
    sockAddress.sin_port = htons(port);
    sockAddress.sin_addr.s_addr = INADDR_ANY;

    if (bind (sd, reinterpret_cast<sockaddr *> (&sockAddress), sizeof (sockAddress)) < 0)
    {
        printf ("ERROR: don't bind!\r\n");
        return false;
    }
    
    if (listen (sd, maxClients) == -1)
    {
        printf ("ERROR: don't listen!\r\n");
        return false;
    }
    printf ("create OK\r\n");
    return true;
}
