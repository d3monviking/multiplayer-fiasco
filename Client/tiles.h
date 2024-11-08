#include<SFML/Graphics.hpp>
class Tile{
    private:
        sf::Vector2f pos;
        sf::Vector2f coords;
        sf::Vector2f vel;
        sf::Vector2f acc;
    public:
        sf::RectangleShape surface;
        Tile(sf::Vector2f pos, sf::Vector2f tileSize);

        void update(float x_shit, float y_shift);

    friend class Level;
};