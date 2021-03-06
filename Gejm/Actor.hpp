#ifndef ACTOR_H
#define ACTOR_H

#include <SFML/Graphics.hpp>
#include "Controllers.hpp"
#include "Object.hpp"
#include <algorithm>

class Object;
class Entrance;
class Game;

class Actor
{
public:
    Actor() = default;
    virtual ~Actor() = default;
    virtual void handleInput(sf::Event &) = 0;
    virtual void handleCollision(Game &) = 0;
    virtual void update(sf::Time &) = 0;
    virtual void draw(sf::RenderWindow &);
    void setPosition(sf::Vector2f);
    sf::Vector2f getPosition() const;
    sf::FloatRect getSize() const;
protected:
    sf::Sprite sprite{};
    sf::Vector2f position{};
};

class Player : public Actor
{
    friend class Object;
public:
    Player(Game &);
    ~Player() = default;
    void handleInput(sf::Event &);
    void handleCollision(Game &);
    void update(sf::Time &);
    int updateHealth(int);
    float updateEnergy(float);
private:
    void handleSlashing();
    void animate();
    
    std::unique_ptr<Controllers> controllers{};
    sf::Texture player{};
    sf::Clock slashClock{};
    sf::Clock movingClock{};
    sf::Clock energyClock{};
    bool slash{};
    char movementDirection{};
    float movingSpeed{};
    
    int health{5};
    float energy{10.0};
};

#endif
