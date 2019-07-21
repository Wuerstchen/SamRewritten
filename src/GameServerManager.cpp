#include "GameServerManager.h"
#include "MyGameServer.h"

// TODO: shouldn't really need MySteam inside here
#include "MySteam.h"
#include "globals.h"
#include <signal.h>
#include <iostream>
#include <unistd.h>

void handle_sigint_gameserv(int signum) {
	g_steam->quit_game();
	exit(0);
}

MyClientSocket*
GameServerManager::quick_server_create(AppId_t appid)
{
    pid_t pid;
    if((pid = fork()) == 0) {
        // Son's process: server role
        
        MyGameServer server(appid);
        server.run();

        // The server will stop running when the client sends a quit request.
        exit(EXIT_SUCCESS);
    }
    else if (pid == -1) {
        std::cerr << "Could not fork in GameServerManager." << std::endl;
        exit(EXIT_FAILURE);
    }
    else {
        // Parent process
        // TODO watch out for zombie processes
        // TODO: don't use signal; use sigaction
        signal(SIGCHLD, SIG_IGN);
        signal(SIGINT, handle_sigint_gameserv);
        return new MyClientSocket(appid);
    }
}