#pragma once

#include "discordpp.h"
#include <string.h>

class DiscordAPIClient {
    private:
        std::shared_ptr<discordpp::Client> client = std::make_shared<discordpp::Client>();
        discordpp::Activity activity;
    public:
        void init();
        void runCallbacks();
        void setActivityState(std::string stateText);
        void setActivityDetails(std::string detailsText);
        void updateActivity();
};