#define DISCORDPP_IMPLEMENTATION
#include <discordpp.h>

#include "discordAPI.hpp"
#include <iostream>

void DiscordAPIClient::init() {
    client->AddLogCallback([](auto message, auto severity) {
        std::cout << "[" << EnumToString(severity) << "] " << message << std::endl;
    }, discordpp::LoggingSeverity::Info);

    client->SetStatusChangedCallback([this](discordpp::Client::Status status, discordpp::Client::Error error, int32_t errorDetail) {
        std::cout << "Status changed: " << discordpp::Client::StatusToString(status) << std::endl;

        if (error != discordpp::Client::Error::None) {
            std::cerr << "Connection Error: " << discordpp::Client::ErrorToString(error) << " - Details: " << errorDetail << std::endl;
        }
    });

    client->SetApplicationId(1133852631979282432);

    activity.SetType(discordpp::ActivityTypes::Playing);

    client->UpdateRichPresence(activity, [](discordpp::ClientResult result) {
        if(!result.Successful()) {
            std::cerr << "Rich Presence update failed";
        }
    });
}

void DiscordAPIClient::runCallbacks() {
    discordpp::RunCallbacks();
}

void DiscordAPIClient::setActivityState(std::string stateText) {
    activity.SetState(stateText);
}

void DiscordAPIClient::setActivityDetails(std::string detailsText) {
    activity.SetDetails(detailsText);
}

void DiscordAPIClient::updateActivity() {
    client->UpdateRichPresence(activity, [](discordpp::ClientResult result) {});
}