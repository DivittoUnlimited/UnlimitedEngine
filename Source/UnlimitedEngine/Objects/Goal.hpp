#ifndef GOAL_HPP
#define GOAL_HPP

#include <SFML/Graphics/RectangleShape.hpp>
#include "Core/SceneNode.hpp"

class Goal : public SceneNode
{
public:
    Goal( Category::Type category );
    virtual unsigned int  getCategory( ) const;
    virtual sf::FloatRect getBoundingRect( ) const;
    virtual bool 		  isMarkedForRemoval( ) const;
    virtual bool          isDestroyed( ) const;
    virtual void          updateCurrent( sf::Time dt, CommandQueue& commands );
    void                  drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;

private:
    Category::Type mCategory;
    sf::RectangleShape mSprite;
};

#endif // GOAL_HPP
