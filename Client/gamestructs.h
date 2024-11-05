#include <SFML/Graphics.hpp>

struct InterpolationData{
    sf::Vector2f pos;
    long timestamp;
};

struct Move{
    long long seq_num;
    sf::Vector2f pos;
    std::vector<bool> thisMove; // Just declare the vector here
    Move() : thisMove(5) {}
};

struct SelfData{
    sf::Vector2f pos;
    long long timestamp;
    int last_processed_seq_num;
};