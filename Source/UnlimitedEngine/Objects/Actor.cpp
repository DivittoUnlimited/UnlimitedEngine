#include "Actor.hpp"
#include "Core/Utility.hpp"


namespace
{
    const auto Table = initializeActorData; // Gives access to the data in Actors.lua
}

Actor::Actor( Tiled::Object data, unsigned int texID, sf::Rect<int> texRect, const TextureManager& textures, SoundPlayer*, const FontManager& fonts )
    : mType( data.type )
    , mSprite( textures.get( texID ), texRect )
    , mName( data.name )
    , mDirectionIndex( 0 )
    , mCanSpeak( true )
    , mCanMove( true )
{
    // std::cout << "Actor name: " << mName << std::endl;
    centerOrigin( mSprite );

    std::unique_ptr<TextNode> name( new TextNode( fonts, data.name ) );
    this->mLabel = name.get( );
    this->mLabel->setColor( sf::Color( 255, 0, 255, 255 ) );
    this->mLabel->getText( )->setCharacterSize( 10 );
    this->mLabel->setPosition( 8, -40 );
    this->attachChild( std::move( name ) );

    this->setPosition( data.x, data.y );
    this->mSpeed = 2;

    // ActorsMap data here!!

    // Animation data
    /// make data structure to hold all possible animations this
    /// actor might have and then load them from the AnimationsMap/Table

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

bool Actor::isDestroyed( void ) const
{
    // By default, scene node needn't be removed
    /// return hitPoints > 0 or something........
    return false;
}

void Actor::drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const
{
    target.draw( mSprite, states );
}

void Actor::updateCurrent( sf::Time dt, CommandQueue& commands )
{
    if( mCanMove )
        updateMovementPattern( dt );
    Entity::updateCurrent( dt, commands );
}

void Actor::handleAction( void )
{
    // call actions in order of there priority in the game world
            // i.e speaking to NPC will be triggered before opening a warp


    // For now just start a DialogState if within range of npc
    std::cout << "This could have been a dialog state!" << std::endl;
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
