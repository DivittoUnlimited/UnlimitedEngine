#ifndef WALL_HPP
#define WALL_HPP

#include "Core/SceneNode.hpp"
#include "Tiled/TiledManager.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

class Wall : public SceneNode
{
public:
    Wall( Tiled::Object data, sf::RectangleShape sprite );
    virtual unsigned int  getCategory( ) const;
    virtual sf::FloatRect getBoundingRect( ) const;
    virtual bool 		  isMarkedForRemoval( ) const;
    virtual void drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;
    virtual void updateCurrent( sf::Time dt, CommandQueue& commands );
    std::string type( void ) { return mType; }

private:
    std::string mType;
    sf::RectangleShape mSprite;
};

#endif // WALL_HPP
