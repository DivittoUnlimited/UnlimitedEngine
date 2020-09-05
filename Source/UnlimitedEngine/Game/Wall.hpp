#ifndef WALL_HPP
#define WALL_HPP

#include "Graphics/SceneNode.hpp"
#include "Graphics/SpriteNode.hpp"
#include "Core/Globals.hpp"

class Wall : public SceneNode
{
public:
    Wall( float speed, unsigned int xPos, sf::Texture texture );
    
    virtual unsigned int	getCategory( ) const;
    virtual bool			isMarkedForRemoval( ) const;
    virtual bool			isDestroyed( ) const;
    virtual void			updateCurrent( sf::Time dt, CommandQueue& commands );
    virtual void			drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;
    
    
    
    // Attributes
    SpriteNode* mSprite;
    float mSpeed;
    
};

#endif // WALL_HPP
