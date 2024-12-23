#include <SFML/Graphics.hpp>
#include "tiles.h"
#include "vector"

enum PLAYER_ANIMATION_STATES {IDLE = 0, MOVING_LEFT, MOVING_RIGHT, JUMPING};

class Player{
    private:
        int id=-1;
        sf::Vector2f pos;
        sf::Vector2f coords;
        sf::Vector2f vel = sf::Vector2f(0,0);
        sf::Vector2f acc = sf::Vector2f(0,0);
        sf::Vector2u dim = sf::Vector2u(0,0);
        float speed = 2.0f;
        float runacc=4;
        float maxspeed=12;
        bool on_ground=true;
        bool facing_right=true;
        float prev_x_vel=0;
        std::vector<Shell* > shells;
        std::vector<PowerUp* > powerups;
        bool boostActive=false;
        bool onPlatform=false;
        long long boostStart;
        
    public:
        sf::Texture movingTexture;
        sf::Texture idleTexture;
        sf::Texture jumpTexture;

        //Animation
        short animState;
        sf::IntRect currentFrameIdle;
        sf::IntRect currentFrameMove;
        sf::IntRect currentFrameJump;
        float initialTextureIdlePos;
        float initialTextureMovePos;
        float initialTextureJumpPos;
        float jumpFrameWidth;
        bool animationSwitch;
        sf::Clock animationTimer;

        sf::Sprite sprite;
        long long count=0;
        sf::Vector2u getDim();
        Player(float x, float y, int id);
        int get_id();
        void resetAnimationTimer();
        void updateAnimation();
        const bool getAnimSwitch();
        void set_id(int id);
        sf::Vector2f getPos();
        void setPos(float x, float y);
        void setCoords(float x, float y);
        void moveCam(float x_shift, float y_shift);
        void setSprite(int id);
        void addShell(Shell* shell);
        void applyPowerUp(long long time);
        void addPowerUps(PowerUp* powerup);

    friend class Level;
};