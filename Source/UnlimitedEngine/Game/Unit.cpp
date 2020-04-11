#include "Unit.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include "Game/DataTables.hpp"
#include "Game/Grid.hpp"


Unit::Unit( unsigned int mId, Category::Type category, UnitTypeData data, const TextureManager& textures )
    : mID( mId )
    , mGridIndex( sf::Vector2i( 0, 0 ) )
    , mHasMoved( false )
    , mHasSpentAction( false )
    , mIsSelectedUnit( false )
    , mSpeed( 2.0f ) // keep this number as a factor of 32 to get smooth movement
    , mPath( nullptr )
    , mDestination( -1.0f, -1.0f )
    , mIsVisible( true )
    , mHealthBarBorder( nullptr )
    , mHealthBar( nullptr )
{
    // define unit based on data from lua
    this->mUnitType         = static_cast<unsigned int>( UnitTypeMap[data.type] );
    this->mCategory         = category;
    this->mAttack           = static_cast<unsigned int>( data.attack );
    this->mDexterity        = static_cast<unsigned int>( data.dexterity );
    this->mConstitution     = static_cast<unsigned int>( data.constitution );
    this->mMaxConstitution  = static_cast<unsigned int>( data.constitution );
    this->mDefense          = static_cast<unsigned int>( data.defense );
    this->mPerception       = static_cast<unsigned int>( data.perception );
    this->mRange            = data.range;
    this->mSprite = sf::Sprite( textures.get( TextureMap.at( data.textureID ) ) );

    std::unique_ptr<RectangleShapeNode> hpBorder( new RectangleShapeNode( sf::IntRect( sf::Vector2i( 0,  68 ), sf::Vector2i( TILE_SIZE, 12.0f ) ) ) );
    this->mHealthBarBorder = hpBorder.get( );
    this->mHealthBarBorder->getSprite( )->setOutlineThickness( 2 );
    if( mCategory & Category::Blue )
        this->mHealthBarBorder->getSprite()->setOutlineColor( sf::Color::Blue ); // blue
    else if( mCategory & Category::Red )
        this->mHealthBarBorder->getSprite()->setOutlineColor( sf::Color::Red ); // red
    this->mHealthBarBorder->getSprite()->setFillColor( sf::Color( 0, 0, 0, 0 ) );
    this->attachChild( std::move( hpBorder ) );

    std::unique_ptr<RectangleShapeNode> hp( new RectangleShapeNode( sf::IntRect( sf::Vector2i( 2, 70 ), sf::Vector2i( TILE_SIZE-4, 8.0f ) ) ) );
    this->mHealthBar = hp.get( );
    this->mHealthBar->getSprite()->setFillColor( sf::Color::Green );
    this->attachChild( std::move( hp ) );
}

Unit::~Unit( void )
{
    mPath = nullptr;
    delete mPath;
}

unsigned int Unit::getCategory( ) const
{
    return mCategory;
}
bool Unit::isMarkedForRemoval( ) const
{
    return isDestroyed( );
}
bool Unit::isDestroyed( ) const
{
    return mConstitution <= 0;
}
void Unit::updateCurrent( sf::Time, CommandQueue& )
{
    // There is a new path to follow
    if( mPath && mPath->size() > 0 && mDestination == sf::Vector2f( -1.0f, -1.0f ) )
    {
        mDestination = sf::Vector2f( mPath->getNextMove() );
        mDestination.x *= 64;
        mDestination.y *= 64;
    }
    // is currently following a path
    if( mDestination != sf::Vector2f( -1.0f, -1.0f ) )
    {
        if( mDestination.x < getPosition().x )           move( -1 * mSpeed, 0 );
        else if( mDestination.x > getPosition().x )      move( mSpeed, 0 );
        else if( mDestination.y - 32 < getPosition().y ) move( 0, -1 * mSpeed );
        else if( mDestination.y - 32 > getPosition().y ) move( 0, mSpeed ); 

        // Has reached destination
        if( this->getPosition().x == mDestination.x && this->getPosition().y == mDestination.y - 32 )
        {
            // Anywere else to go?
            if( mPath && mPath->size() > 0 )
            {
                mDestination = sf::Vector2f( mPath->getNextMove() );
                mDestination.x *= 64;
                mDestination.y *= 64;
                if( mPath->mPath.size() == 0 ) mPath = nullptr;
            }
            else mDestination = sf::Vector2f( -1.0f, -1.0f );
        }
    }
}
void Unit::drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const
{
    if( mIsVisible )
    {
        this->mHealthBar->getSprite()->setFillColor( sf::Color::Green );
        if( mCategory & Category::Blue )
            this->mHealthBarBorder->getSprite()->setOutlineColor( sf::Color::Blue ); // blue
        else if( mCategory & Category::Red )
            this->mHealthBarBorder->getSprite()->setOutlineColor( sf::Color::Red ); // red
        target.draw( mSprite, states );
    }
    else
    {
        mHealthBar->getSprite()->setFillColor( sf::Color( 0, 0, 0, 0 ) );
        mHealthBarBorder->getSprite()->setOutlineColor( sf::Color( 0, 0, 0, 0 ) );
    }
}

void Unit::takeDamage( int amount )
{
    this->mConstitution -= amount;
    if( mConstitution >= 1 )
    {
        // set health bar proportional to constitution value
        this->mHealthBar->getSprite()->setSize( sf::Vector2f( (static_cast<float>(mConstitution) / mMaxConstitution) * (TILE_SIZE - 4), 8 ) );

    }
    else
    {
        this->mHealthBar->getSprite()->setFillColor( sf::Color( 0, 0, 0, 0 ) );
        // Death animation / sound effects?
    }
}
