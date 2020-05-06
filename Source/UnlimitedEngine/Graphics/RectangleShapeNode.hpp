#ifndef RECTANGLESHAPENODE_HPP
#define RECTANGLESHAPENODE_HPP

#include "SceneNode.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

///
/// \brief The SpriteNode class
/// The graphics only child of the SceneNode class lacks features such as event handleing and frame
/// updates.
class RectangleShapeNode : public SceneNode
{
public:
             RectangleShapeNode( const sf::IntRect& rect );
             sf::RectangleShape* getSprite() { return &mSprite; }
protected:
    //## Attributes
    sf::RectangleShape mSprite;

    virtual void drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;
};
#endif // RECTANGLESHAPENODE_HPP
