#include "Warp.hpp"
#include "Core/Utility.hpp"

#include "Core/EmitterNode.hpp"

namespace
{
    const auto Table = initializeWarpData; // Gives access to the data in Warps.lua
}

Warp::Warp( Tiled::Object data, sf::RectangleShape sprite, const TextureManager& textures, const FontManager& fonts )
    : mName( data.name )
    , mSprite( sprite )
{
    // Build object using resource manager and data from tiled map
    this->setPosition( data.x, data.y );
    mSprite.setFillColor( sf::Color( 0, 0, 255, 100 ) );

    std::unique_ptr<TextNode> name( new TextNode( fonts, data.name ) );
    this->mLabel = name.get( );
    this->mLabel->setColor( sf::Color( 255, 0, 0, 255 ) );
    this->mLabel->getText( )->setCharacterSize( 10 );
    this->mLabel->setPosition( 27, 20 );
    this->attachChild( std::move( name ) );

    mCreateParticleEffectCommand.category = Category::ParticleLayer;
    mCreateParticleEffectCommand.action = [this, &textures] ( SceneNode& node, sf::Time )
    {
        /// \brief propellant
        /// This shit causes major frame rate issues!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        std::unique_ptr<EmitterNode> propellant( new EmitterNode( ParticleMap.at( "PortalEffect" ) ) );
        propellant->setPosition( this->getBoundingRect().left, this->getBoundingRect().top  );
        node.attachChild( std::move( propellant ) );
    };

    // WarpsMap data here!! i.e where to warp to...
    mNewPosition = sf::Vector2f( Table[WarpMap.at( mName )].x, Table[WarpMap.at( mName )].y );
}

unsigned int Warp::getCategory( ) const
{
    return Category::Warp;
}

sf::FloatRect Warp::getBoundingRect( ) const
{
    return getWorldTransform( ).transformRect( mSprite.getGlobalBounds( ) );
}

bool Warp::isMarkedForRemoval( ) const
{
    return false;
}

void Warp::drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const
{
    target.draw( mSprite, states );
}

void Warp::updateCurrent( sf::Time dt, CommandQueue& commands )
{
    this->move( 10 * dt.asSeconds(), 0 );
    commands.push( mCreateParticleEffectCommand );
}

