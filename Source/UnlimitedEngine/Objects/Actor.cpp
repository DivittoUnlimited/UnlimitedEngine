#include "Actor.hpp"
#include "Core/Utility.hpp"


namespace
{
    const auto Table = initializeActorData; // Gives access to the data in Actors.lua
}

Actor::Actor( Tiled::Object data, sf::Sprite sprite, const TextureManager& textures, SoundPlayer* soundPlayer, const FontManager& fonts )
    : mSprite( sprite )
    , mType( data.type )
    , mName( data.name )
    , mDirectionIndex( 0 )
{
    // std::cout << "Actor name: " << mName << std::endl;
    centerOrigin( mSprite );
    // Build object using resource manager and data from tiled map
    this->setPosition( data.x, data.y );

    this->mSpeed = 20;

    std::unique_ptr<TextNode> name( new TextNode( fonts, data.name ) );
    this->mLabel = name.get( );
    this->mLabel->setColor( sf::Color( 255, 0, 255, 255 ) );
    this->mLabel->getText( )->setCharacterSize( 10 );
    this->mLabel->setPosition( 8, -10 );
    this->attachChild( std::move( name ) );

    // ActorsMap data here!!

    // Path
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
    updateMovementPattern( dt );
    Entity::updateCurrent( dt, commands );
}

void Actor::updateMovementPattern( sf::Time dt )
{
    // Enemy airplane: Movement pattern
    const std::vector<Direction>& directions = Table[ActorMap.at( mName )].directions;
    if( !directions.empty( ) )
    {
        // Moved long enough in current direction: Change direction
        if( mTravelledDistance > directions[mDirectionIndex].distance )
        {
            mDirectionIndex = ( mDirectionIndex + 1 ) % directions.size( );
            mTravelledDistance = 0.f;
        }

        // Compute velocity from direction
        float radians = toRadian( directions[mDirectionIndex].angle );
        float vx = speed( ) * std::cos( radians );
        float vy = speed( ) * std::sin( radians );

        setVelocity( vx, vy );

        mTravelledDistance += speed( ) * dt.asSeconds( );
    }
}