<img width="476" height="198" alt="image" src="https://github.com/user-attachments/assets/1a3692cb-8e90-457e-9926-14593047dce0" />

# Deps
[extern/discord_social_sdk](https://discord.com/developers/applications/select/social-sdk/downloads) (use the standalone c++ version) <br>
[extern/steam](https://partner.steamgames.com/downloads/list) <br>
[extern/steam_bin](https://partner.steamgames.com/downloads/list)

# Building
`$ mkdir build` <br>
`$ cd build` <br>
`$ cmake .. -DCMAKE_BUILD_TYPE=Release` <br>
`$ cmake --build . --config Release -j$(nproc)`

# Installing
Copy `steam_appid.txt` `libdiscord_partner_sdk.so` `libsteam_api.so` `DiscordRPCExtender` from the build folder to the games root folder<br>
In steam game->properties->launch options enter: `"/mnt/extra/SteamLibrary/steamapps/common/SimRail/DiscordRPCExtender" %command%` replacing the path with the correct one for your case

## Modifying to work with other games
Modify the steam_appid.txt and replace the appid with the one your game uses. <br>
In `discordAPI.cpp` change the `client->SetApplicationId(ID);` to use your games ID, this can be found trough discord if you somehow enter the game context and press the ... and press copy app ID <br>
In `main.cpp` change the key in `GetFriendRichPresence(ID, key)` to use the key you want to use, these can be found `GetFriendRichPresenceKeyCount(ID);` `GetFriendRichPresenceKeyByIndex(ID, iKey)` [read more about Steam API](https://partner.steamgames.com/doc/api/ISteamFriends)
