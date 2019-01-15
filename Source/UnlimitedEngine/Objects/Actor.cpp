#include "Actor.hpp"
#include "Core/Utility.hpp"

Actor::Actor( Tiled::Object data, sf::Sprite sprite, const TextureManager& textures, SoundPlayer* soundPlayer, const FontManager& fonts )
    : mSprite( sprite )
    , mType( data.type )
{
    centerOrigin( mSprite );
    // Build object using resource manager and data from tiled map
    this->setPosition( data.x, data.y );

    if( getCategory( ) == Category::Player )
        this->mSpeed = 32;
    else
        this->mSpeed = 16;

    std::unique_ptr<TextNode> name( new TextNode( fonts, data.name ) );
    this->mLabel = name.get();
    this->mLabel->setColor( sf::Color( 255, 0, 255, 255 ) );
    this->mLabel->getText()->setCharacterSize( 10 );
    this->mLabel->setPosition( 8, -10 );
    this->attachChild( std::move( name ) );
}

unsigned int Actor::getCategory( ) const
{
    if( mType == "Player" )
        return Category::Player;
    else
        return Category::NPC;
}

sf::FloatRect Actor::getBoundingRect( ) const
{
    return getWorldTransform( ).transformRect( mSprite.getGlobalBounds( ) );
}

bool Actor::isMarkedForRemoval( ) const
{
    return false;
}

void Actor::drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const
{
    target.draw( mSprite, states );
}

void Actor::updateCurrent( sf::Time dt, CommandQueue& commands )
{
    //if( getCategory( ) == Category::Player )
        //this->move( 0, -1 * dt.asSeconds( ) );
    Entity::updateCurrent( dt, commands );
}
