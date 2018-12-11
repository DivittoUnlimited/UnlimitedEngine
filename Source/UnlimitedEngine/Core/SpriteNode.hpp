#ifndef SPRITENODE_HPP
#define SPRITENODE_HPP

#include "SceneNode.hpp"
#include <SFML/Graphics/Sprite.hpp>

///
/// \brief The SpriteNode class
/// The graphics only child of the SceneNode class lacks features such as event handleing and frame
/// updates.
class SpriteNode : public SceneNode
{
public:
    explicit SpriteNode( const sf::Texture& texture );
             SpriteNode( const sf::Texture& texture, const sf::IntRect& textureRect );
             sf::Sprite getSprite() { return mSprite; }
protected:
    //## Attributes
    sf::Sprite mSprite;

    virtual void drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;
};

#endif // SPRITENODE_HPP
