#include "Game_State.hpp"
#include "ResourcePath.hpp"

using namespace std;
using namespace constants;

Game_State::Game_State(Game & game, std::unique_ptr<Player> & player) :
game(game), player(player) {}

/*__  __   _   ___ _  _   __  __ ___ _  _ _   _
 |  \/  | /_\ |_ _| \| | |  \/  | __| \| | | | |
 | |\/| |/ _ \ | || .` | | |\/| | _|| .` | |_| |
 |_|  |_/_/ \_\___|_|\_| |_|  |_|___|_|\_|\___/
 */

Main_Menu::Main_Menu(Game & game, std::unique_ptr<Player> & player) :
Game_State(game, player)
{
    if(!background.loadFromFile(resourcePath() + "background0.png"))
        throw invalid_argument("Background not loaded!");
    background_sprite.setTexture(background);
    
    if(!font.loadFromFile(resourcePath() + "Powerful.ttf"))
        throw invalid_argument("Font not loaded!");
    
    title = sf::Text("GEJM", font, 150);
    title.setOrigin(title.getGlobalBounds().width/2, title.getGlobalBounds().height/2);
    title.setPosition(window_width/2, 200);
    title.setFillColor(sf::Color::Black);
}

void Main_Menu::handleInput(sf::Event &)
{
    
}

void Main_Menu::update(sf::Time &)
{
    
}

void Main_Menu::draw(sf::RenderWindow & window)
{
    window.draw(background_sprite);
    window.draw(title);
}

/*___ _  _    ___   _   __  __ ___
 |_ _| \| |  / __| /_\ |  \/  | __|
  | || .` | | (_ |/ _ \| |\/| | _|
 |___|_|\_|  \___/_/ \_\_|  |_|___|
 */

In_Game::In_Game(Game & game, std::unique_ptr<Player> & player) :
Game_State(game, player)
{
    if(!world.loadFromFile(resourcePath() + "Overworld.png"))
        throw invalid_argument("World texture not loaded!");
    
    if(!other.loadFromFile(resourcePath() + "objects.png"))
        throw invalid_argument("World texture not loaded!");

    background_sprite.setTexture(world);
    background_sprite.setScale(2.0, 2.0);
    
    playerinfo_sprite.setTexture(other);
    playerinfo_sprite.setScale(3.0, 3.0);
    playerinfo_sprite.setTextureRect(sf::IntRect(0, 226, 78, 53));
    
    player->setPosition(sf::Vector2f(window_width/2 - 16,
                                     window_height/2));
    
    constructObjects();
}

void In_Game::handleInput(sf::Event & event)
{
    player->handleInput(event);
}

void In_Game::update(sf::Time & delta)
{
    player->update(delta);
    player->handleCollision(game);
    
    for(auto && object : objects)
    {
        object->update(delta);
        
        Entrance * temp = dynamic_cast<Entrance*>(object.get());
        
        if(object->canCollide() && temp == nullptr)
            object->handleCollision(player);
        
        //player->collideWith(object);
        
        if(temp != nullptr && player->getSize().intersects(object->getSize()))
        {
            game.changeState(2);
            player->setPosition(sf::Vector2f(window_width/2  - 16,
                                             window_height/2 + 100));
        }
    }
}

void In_Game::draw(sf::RenderWindow & window)
{
    drawWorld(window);
    window.draw(playerinfo_sprite);
    player->draw(window);
}

void In_Game::drawWorld(sf::RenderWindow & window)
{
    // Grass background
    background_sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
    
    for(int columns{}; columns < window_height/32; columns++)
    {
        for(int rows{}; rows < window_width/32; rows++)
        {
            background_sprite.setPosition(32*rows, 32*columns);
            window.draw(background_sprite);
        }
    }

    for(auto && object : objects)
        object->draw(window);
}

void In_Game::constructObjects()
{
    // House
    objects.push_back(make_unique<House>(sf::Vector2f(320, 320)));
    
    // Entrance
    objects.push_back(make_unique<Entrance>(sf::Vector2f(378, 440)));
    
    // Grave
    objects.push_back(make_unique<Grave>(sf::Vector2f(500, 320)));
    
    // Living Tree
    objects.push_back(make_unique<Living_Tree>(sf::Vector2f(700, 520)));
}

/*___ _  _   _  _  ___  _   _ ___ ___
 |_ _| \| | | || |/ _ \| | | / __| __|
  | || .` | | __ | (_) | |_| \__ \ _|
 |___|_|\_| |_||_|\___/ \___/|___/___|
 */

In_House::In_House(Game & game, std::unique_ptr<Player> & player) :
Game_State(game, player)
{
    if(!interior.loadFromFile(resourcePath() + "Inner.png"))
        throw invalid_argument("World texture not loaded!");
    
    if(!other.loadFromFile(resourcePath() + "objects.png"))
        throw invalid_argument("World texture not loaded!");
    
    background_sprite.setTexture(interior);
    background_sprite.setScale(2.0, 2.0);
    
    playerinfo_sprite.setTexture(other);
    playerinfo_sprite.setScale(3.0, 3.0);
    playerinfo_sprite.setTextureRect(sf::IntRect(0, 226, 78, 53));
    
    player->setPosition(sf::Vector2f(window_width/2 - 16,
                                     window_height/2 + 100));
    
    constructObjects();
}

void In_House::handleInput(sf::Event & event)
{
    player->handleInput(event);
}

void In_House::update(sf::Time & delta)
{
    player->update(delta);
    player->handleCollision(game);
    
    for(auto && object : objects)
    {
        object->update(delta);
        
        Entrance * temp = dynamic_cast<Entrance*>(object.get());
        
        if(object->canCollide() && temp == nullptr)
            object->handleCollision(player);
        
        if(temp != nullptr && player->getSize().intersects(object->getSize()))
        {
            game.changeState(1);
            player->setPosition(sf::Vector2f(320+74-16, 320+160-16));
        }
    }
}

void In_House::draw(sf::RenderWindow & window)
{
    drawHouse(window);
    window.draw(playerinfo_sprite);
    player->draw(window);
}

void In_House::drawHouse(sf::RenderWindow & window)
{
    // Floor
    background_sprite.setTextureRect(sf::IntRect(0, 16, 16, 16));
    
    for(int columns{}; columns < window_height/32/3; columns++)
    {
        for(int rows{}; rows < window_width/32/3; rows++)
        {
            background_sprite.setPosition(window_width/3  + 32*rows,
                                          window_height/3 + 32*columns);
            window.draw(background_sprite);
        }
    }
    
    for(auto && object : objects)
        object->draw(window);
}

void In_House::constructObjects()
{
    // Entrance
    objects.push_back(make_unique<Entrance>(sf::Vector2f(window_width/2 - 16,
                                                         window_height/2 + 160)));
    
    // Door Mat
    objects.push_back(make_unique<Door_Mat>(sf::Vector2f(window_width/2 - 16,
                                                         window_height/2 + 128)));
}

/*___  _  _   _ ___ ___   __  __ ___ _  _ _   _
 | _ \/_\| | | / __| __| |  \/  | __| \| | | | |
 |  _/ _ \ |_| \__ \ _|  | |\/| | _|| .` | |_| |
 |_|/_/ \_\___/|___/___| |_|  |_|___|_|\_|\___/
 */

Pause_Menu::Pause_Menu(Game & game, std::unique_ptr<Player> & player) :
Game_State(game, player)
{
    
}

void Pause_Menu::handleInput(sf::Event &)
{
    
}

void Pause_Menu::update(sf::Time &)
{
    
}

void Pause_Menu::draw(sf::RenderWindow &)
{
    
}
