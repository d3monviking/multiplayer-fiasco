#include<bits/stdc++.h>
#include "gamestructs.h"
#include "message_generated.h"
#include <boost/asio.hpp>
#include "player.h"

class Level{
   
    private:
        int gameStart=0;
        sf::Vector2f tileSize = sf::Vector2f(50, 50);
        sf::RenderWindow* display_surface;
        long long count=0;
        int self_id;
        
        // vector<Tile> tiles;

    public:
        Level(sf::RenderWindow* window);
        Level();
        void set_id(int id);
        long long setCurrentTimestamp();
        void applyLocalInput(std::vector<bool> &this_move);
        void processPendingUpdates();
        void updatePlayer();
        void InterpolateEntity(Player* player);
        void render();
        void run();
};