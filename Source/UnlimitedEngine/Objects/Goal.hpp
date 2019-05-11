#ifndef GOAL_HPP
#define GOAL_HPP

#include <SFML/Graphics/RectangleShape.hpp>
#include "Core/SceneNode.hpp"

enum Owner {
      TeamA = 0
    , TeamB
};

class Goal : public SceneNode
{
public:
    Goal( Owner owner );
    virtual unsigned int  getCategory( ) const;
    virtual sf::FloatRect getBoundingRect( ) const;
    virtual bool 		  isMarkedForRemoval( ) const;
    virtual bool          isDestroyed( ) const;
    virtual void          updateCurrent( sf::Time dt, CommandQueue& commands );
    void                  drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;

private:
    Owner mOwner;
    sf::RectangleShape mSprite;
};

#endif // GOAL_HPP
