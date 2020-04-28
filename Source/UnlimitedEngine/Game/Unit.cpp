#include "Unit.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include "Game/DataTables.hpp"
#include "Game/Grid.hpp"
#include "Game/DataTables.hpp"

namespace
{
    const std::vector<AbilityData> Abilities = initializeAbilityData;
}

Unit::Unit( unsigned int mId, Category::Type category, UnitTypeData data, const TextureManager& textures )
    : mID( mId )
    , mGridIndex( sf::Vector2i( 0, 0 ) )
    , mInitiative( 0 )
    , mHasMoved( false )
    , mHasSpentAction( false )
    , mIsSelectedUnit( false )
    , mBoardSpeed( 2.0f ) // keep this number as a factor of 32 to get smooth movement
    , mPath( nullptr )
    , mDestination( -1.0f, -1.0f )
    , mIsVisible( true )
    , mMorale( 100.0f )
    , mAbilities( )
    , mHealthBarBorder( nullptr )
    , mHealthBar( nullptr )
{
    // define unit based on data from lua
    this->mUnitType      = static_cast<unsigned int>( UnitTypeMap[data.type] );
    this->mCategory      = category;
    this->mAttack        = static_cast<unsigned int>( data.attack );
    this->mSpeed         = static_cast<unsigned int>( data.speed );
    this->mHealth        = static_cast<unsigned int>( data.health );
    this->mMaxHealth     = static_cast<unsigned int>( data.health );
    this->mArmour        = static_cast<unsigned int>( data.armour );
    this->mPerception    = static_cast<unsigned int>( data.perception );
    this->mStamina       = static_cast<unsigned int>( data.stamina );
    this->mSprite = sf::Sprite( textures.get( TextureMap.at( data.textureID ) ) );

    for( auto a : data.abilities )
        mAbilities.insert( std::pair<std::string, AbilityData>( a, Abilities.at( AbilityMap.at( a ) ) ) );

    std::unique_ptr<RectangleShapeNode> hpBorder( new RectangleShapeNode( sf::IntRect( sf::Vector2i( 0,  68 ), sf::Vector2i( TILE_SIZE, 22.0f ) ) ) );
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

    std::unique_ptr<RectangleShapeNode> morale( new RectangleShapeNode( sf::IntRect( sf::Vector2i( 2, 80 ), sf::Vector2i( TILE_SIZE-4, 8.0f ) ) ) );
    this->mMoraleBar = morale.get( );
    this->mMoraleBar->getSprite()->setFillColor( sf::Color( 255, 215, 0, 255 ) );
    this->attachChild( std::move( morale ) );
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
    return mHealth <= 0;
}
void Unit::updateCurrent( sf::Time, CommandQueue& )
{
    if( mBeginingOfTurn )
    {
        mBeginingOfTurn = false;
        StatModifier mod;
        std::vector<StatModifier> newMods;
        while( mCurrentModifiers.size( ) )
        {
            mod = mCurrentModifiers.back();
            mCurrentModifiers.pop_back();
            if( mod.stat == "health" )              modHealth( mod.power.roll( ) ); // Dif because they must also update the graphics bars representign them to the player
            else if( mod.stat == "morale" )         modMorale( mod.power.roll( ) );
            else if( mod.stat == "armour" )         this->mArmour += mod.power.roll();
            else if( mod.stat == "stamina" )        this->mStamina += mod.power.roll();
            else if( mod.stat == "initiative" )     this->mInitiative += mod.power.roll();
            else if( mod.stat == "perception" )     this->mPerception += mod.power.roll();
            else
            {
                std::cout << "Attempting to modifiy a unit stat that does not exist!" << std::endl;
                assert( mod.stat == "health" );
            }
            mod.duration--;
            if( mod.duration > 0 )
            {
                newMods.push_back( mod );
            }
        }
        mCurrentModifiers = newMods;
    }

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
        if( mDestination.x < getPosition().x )           move( -1 * mBoardSpeed, 0 );
        else if( mDestination.x > getPosition().x )      move( mBoardSpeed, 0 );
        else if( mDestination.y - 32 < getPosition().y ) move( 0, -1 * mBoardSpeed );
        else if( mDestination.y - 32 > getPosition().y ) move( 0, mBoardSpeed );

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
        this->mMoraleBar->getSprite()->setFillColor( sf::Color( 255, 215, 0, 255 ) );
        if( mCategory & Category::Blue )
            this->mHealthBarBorder->getSprite()->setOutlineColor( sf::Color::Blue ); // blue
        else if( mCategory & Category::Red )
            this->mHealthBarBorder->getSprite()->setOutlineColor( sf::Color::Red ); // red
        target.draw( mSprite, states );
    }
    else
    {
        mMoraleBar->getSprite()->setFillColor( sf::Color( 0, 0, 0, 0 ) );
        mHealthBar->getSprite()->setFillColor( sf::Color( 0, 0, 0, 0 ) );
        mHealthBarBorder->getSprite()->setOutlineColor( sf::Color( 0, 0, 0, 0 ) );
    }
}

void Unit::useAbility( std::string abilityID, Unit* target )
{
    if( mAbilities.find( abilityID ) != mAbilities.end() )
    {
        for( auto t : mAbilities.at( abilityID ).targetMods )
            target->addModifier( t );
        for( auto t : mAbilities.at( abilityID ).userMods )
            this->addModifier( t );
    }
}

void Unit::addModifier( StatModifier mod )
{
    if( mod.stat == "health" )              modHealth( mod.power.roll( ) ); // Dif because they must also update the graphics bars representign them to the player
    else if( mod.stat == "morale" )         modMorale( mod.power.roll( ) );
    else if( mod.stat == "armour" )         this->mArmour += mod.power.roll();
    else if( mod.stat == "stamina" )        this->mStamina += mod.power.roll();
    else if( mod.stat == "initiative" )     this->mInitiative += mod.power.roll();
    else if( mod.stat == "perception" )     this->mPerception += mod.power.roll();
    else
    {
        std::cout << "Attempting to modifiy a unit stat that does not exist!" << std::endl;
        assert( mod.stat == "health" );
    }
    mod.duration--;
    if( mod.duration > 0 )
        mCurrentModifiers.push_back( mod );
}

void Unit::modHealth( int amount )
{
    this->mHealth += amount;
    if( mHealth >= 1 && mHealth <= mMaxHealth )
    {
        // set health bar proportional to constitution value
        this->mHealthBar->getSprite()->setSize( sf::Vector2f( (static_cast<float>(mHealth) / mMaxHealth) * (TILE_SIZE - 4), 8 ) );
    }
    else
    {
        this->mHealthBar->getSprite()->setFillColor( sf::Color( 0, 0, 0, 0 ) );
        // Death animation / sound effects?
    }
}
void Unit::modMorale( int amount )
{
    this->mMorale += amount;
    if( mMorale >= 1 && mMorale <= 100 )
    {
        // set morale bar proportional to mMoral value
        this->mMoraleBar->getSprite()->setSize( sf::Vector2f( (static_cast<float>(mMorale) / 100) * (TILE_SIZE - 4), 8 ) );
    }
    else
    {
        this->mMoraleBar->getSprite()->setFillColor( sf::Color( 0, 0, 0, 0 ) );
        // sleep / strike animation  --  sound effects?
    }
}
