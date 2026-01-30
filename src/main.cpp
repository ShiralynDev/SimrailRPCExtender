#include <iostream>
#include <steam_api.h>
#include "Discord/discordAPI.hpp"
#include <unistd.h>
#include <sys/wait.h>
#include <vector>

int main(int argc, char** argv) {

    if (argc < 2) {
        std::cerr << "No game command provided by Steam\n";
        return 1;
    }

    char** gameCmd = &argv[1];
    pid_t pid = fork();
    if (pid == 0) {
        execvp(gameCmd[0], gameCmd); 
        perror("Failed to launch game");
        return 1;
    } else if (pid < 0) {
        perror("something failed");
        return 1;
    }

    if (!SteamAPI_Init()) {
        return 1;
    }

    DiscordAPIClient discordAPI;
    discordAPI.init();

    CSteamID steamID = SteamUser()->GetSteamID();

    /*
    steam_display
    steam_player_group
    server
    vehicle
    next_point
    steam_player_group_size
    */

    while (true) { 
        int status;
        pid_t result = waitpid(pid, &status, WNOHANG);
        if (result == pid) {
            std::cout << "Game closed\n";
            break;
        }

        std::string steamDisplay = SteamFriends()->GetFriendRichPresence(steamID, "steam_display");
        std::string server = SteamFriends()->GetFriendRichPresence(steamID, "server");

        discordAPI.setActivityDetails(steamDisplay);
        discordAPI.setActivityState(server);
        discordAPI.updateActivity();

        SteamAPI_RunCallbacks();
        discordAPI.runCallbacks();
        sleep(5);
    }

    SteamAPI_Shutdown();

    return 0;
}