#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include <common/base/constant_ptr_list.h>
#include <string>
#include <list>
#include <map>
#include <game_room.h>
#include <mutex>

namespace sails {

class Server;

class GameWorld {
public:
    GameWorld(std::string gameCode, Server* server);

    std::string getGameCode() { return gameCode;}

    GameRoom* getGameRoom(std::string& roomCode);
    GameRoom* createGameRoom(std::string& roomCode);


    bool connectPlayer(uint32_t playerId, std::string& roomCode);
    void disConnectPlayer(uint32_t playerId, std::string& roomCode);
    
    std::list<std::string> getRoomList();

    std::string getRoomHostMac(std::string& roomCode);

    void spreadMessage(std::string& roomCode, std::string& message);

    void transferMessage(std::string& roomCode, std::string&ip, std::string& mac, std::string& message);
    
    Server* getServer();
private:
    std::string gameCode;
    int roomNum;
    std::map<std::string, GameRoom*> roomMap;
    std::mutex roomMutex;
    int playerNum;

    Server* server;
};


} // namespace sails
#endif /* GAME_WORLD_H */












