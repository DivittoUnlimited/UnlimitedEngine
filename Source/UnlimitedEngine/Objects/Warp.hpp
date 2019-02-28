#ifndef WARP_HPP
#define WARP_HPP

#include <SFML/Graphics/RectangleShape.hpp>
#include "Core/SceneNode.hpp"
#include "Core/TextNode.hpp"
#include "Core/DataTables.hpp"
#include "Core/RectangleShapeNode.hpp"
#include "Core/Command.hpp"
#include "Core/CommandQueue.hpp"
#include "Tiled/TiledManager.hpp"

class Warp : public SceneNode
{
public:
    Warp(Tiled::Object data, sf::RectangleShape sprite, const TextureManager &textures, const FontManager& fonts );

    virtual unsigned int  getCategory( ) const;
    virtual sf::FloatRect getBoundingRect( ) const;
    virtual bool 		  isMarkedForRemoval( ) const;

    virtual void drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;
    virtual void updateCurrent( sf::Time dt, CommandQueue& commands );

    sf::Vector2f getNewPosition( void ) { return mNewPosition; }
private:
    std::string     mName;
    TextNode*       mLabel;
    sf::RectangleShape mSprite;
    sf::Vector2f mNewPosition;
    Command         mCreateParticleEffectCommand;
};

#endif // WARP_HPP
