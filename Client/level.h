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
        float gravity = 1.4;
        float right_calibration;
        float left_calibration=0;
        int level_width;
        int level_height;
        bool shifted=false;
        float start_ypos=0;
        sf::View gameView;
        float cameraOffsetX = 100.0f;
        const int MAX_ROWS = 400;
        const int MAX_COLS = 1200;
        int level[400][1200];
        int collectiblesPos[400][1200];
        int movingPlatformPos[400][1200];
        sf::Texture tileSheet; 
        sf::Texture movingPlatImage;
        sf::Texture treeImage;
        sf::Texture rocksImage;
        sf::Texture spikeImage;
        sf::Texture finishTexture;

        string tileSetPath = "../Sprites/terrain_tiles.png";
        string movingPlatPath = "../Sprites/platform.png";
        string treeImagePath = "../Sprites/Green-Tree.png";
        string rocksImagePath = "../Sprites/Props-Rocks.png";
        string spikeImagePath = "../Sprites/spike.png";
        string finishLinePath = "../Sprites/finishLine.png";

        //Sprite Vectors
        std::vector<Tile> tiles;
        std::vector<MovingPlatform*> movingPlatforms; 
        std::vector<Tile> background;
        std::vector<Tile> killingThings;
        long long powerUpControl;
        sf::Clock clock1;
        int pUp=0;
    public:
        Level(sf::RenderWindow* window, int screen_width);
        std::vector<Collectibles* > collictibles;
        Level();
        void set_id(int id);
        long long setCurrentTimestamp();
        void updateCamera();
        void setup_level(string terrainPath, string collectiblePath, string movingPlatformPath, string rocksPath, string treePath, string waterPath, string backgroundPath, string spikePath);
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