#include<bits/stdc++.h>
#include "gamestructs.h"
#include "message_generated.h"
#include <boost/asio.hpp>
#include "player.h"
#include "tiles.h"

class Level{
   
    private:
        int gameStart=0;
        sf::Vector2f tileSize = sf::Vector2f(50, 50);
        sf::RenderWindow* display_surface;
        long long count=0;
        int self_id;
        float x_shift=0;
        float y_shift=0;
        std::vector<Tile> tiles;
        float gravity = 1;
        float right_calibration;
        float left_calibration=0;
        int level_width;
        int level_height;
        bool shifted=false;
        float start_ypos=0;
        std::vector<std::string> level_map={"                   ",
                                            "                   ",
                                            "                   ",
                                            "                   ",
                                            " P                 ",
                                            "XXXXXXXXXXXX  XXXXX",
                                            "                   ",
                                            "                   ",
                                            "XXXX  XXXXXXXXXXXXX",
                                            "                   ",
                                            "                   ",
                                            "                   ",
                                            "                   ",
                                            "                   ",
                                            "XXXXXXXXXXXX  XXXXX",
                                            "                   ",
                                            "                   ",
                                            "                   ",
                                            "                   ",
                                            "                   ",
                                            "XXXXXXXXXXXXXXXXXXX",
                                            };
        
        // vector<Tile> tiles;

    public:
        Level(sf::RenderWindow* window, int screen_width);
        Level();
        void set_id(int id);
        long long setCurrentTimestamp();
        void setup_level(int screen_width);
        void scroll_x();
        void scroll_y();
        void x_collisions();
        void y_collisions();
        void applyLocalInput(std::vector<bool> &this_move);
        void processPendingUpdates();
        void updatePlayer();
        void InterpolateEntity(Player* player);
        void render();
        void run();
        bool colliding(sf::RectangleShape& rect1, sf::RectangleShape& rect2, sf::Vector2f coord1, sf::Vector2f coord2);
};