#include "server.h"
#include "process_control.h"

#include <iostream>

int main (int argc, char *argv[]) 
{
    if (argc != 2)
    {
        printf ("ERROR: incorrect argc!");
        return 1;
    }

    const int port = atoi (argv[1]);     
    if (port <= 0)
    {
        printf ("ERROR: incorrect port!");
        return 2;
    }

    Server server (port);
    if (!server.create ())
        return 3;

    auto addr = server.getSockAddress ();
    socklen_t len = sizeof (addr);
    ProcessControl pc (server.getSd (), reinterpret_cast<sockaddr*>(&addr), &len);

    pc.setFilename (std::to_string (port));
    pc.run ();

    return 0;
}
