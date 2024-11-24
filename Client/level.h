#include<bits/stdc++.h>
#include "gamestructs.h"
#include "message_generated.h"
#include <boost/asio.hpp>
#include "player.h"

using namespace std;


class Level{
   
    private:
        int gameStart=0;
        sf::Vector2u tileSize = sf::Vector2u(16, 16);
        sf::RenderWindow* display_surface;
        std::map<int, sf::Vector2f> movemap;
        long long count=0;
        int self_id;
        float x_shift=0;
        float y_shift=0;
        std::vector<Tile> tiles;
        float gravity = 1.4;
        float right_calibration;
        float left_calibration=0;
        int level_width;
        int level_height;
        bool shifted=false;
        float start_ypos=0;
        sf::View gameView;
        float cameraOffsetX = 100.0f;
        const int MAX_ROWS = 200;
        const int MAX_COLS = 400;
        int level[200][400];
        sf::Texture tileSheet; 
        string tileSetPath = "../Sprites/terrain_tiles.png";
        std::vector<Collectibles* > collictibles;
        long long powerUpControl;
        sf::Clock clock1;
        int pUp=0;
    public:
        Level(sf::RenderWindow* window, int screen_width);
        Level();
        void set_id(int id);
        long long setCurrentTimestamp();
        void updateCamera();
        void setup_level(string path);
        void scroll_x();
        void scroll_y();
        void x_collisions();
        void y_collisions();
        void applyLocalInput(std::vector<bool> &this_move, int camFlag);
        void processPendingUpdates();
        void updatePlayer();
        void InterpolateEntity(Player* player);
        void render();
        void run();
        bool colliding(sf::Sprite& sp1, sf::Sprite& sp2, sf::Vector2f coord1, sf::Vector2f coord2);
};