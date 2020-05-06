#include "TeamStats.hpp"
#include "Game/DataTables.hpp"
#include "Core/Globals.hpp"

TeamStats::TeamStats( Category::Type teamColor, FontManager& fonts )
    : SceneNode(  )
    , mDisplay( false )
    , mMoralPoints( 10 )
    , mSkillPoints( 0 )
    , mMoralePercentage( 0.0f )
    , mNumUnits( 0 )
    , mNumScouts( 0 )
    , mTeamColor( teamColor )
    , mIsDisplaying( false )
{
    if( mTeamColor & Category::Red )
        this->mColor = sf::Color::Red;
    else
        this->mColor = sf::Color::Blue;

    std::unique_ptr<RectangleShapeNode> bg( new RectangleShapeNode( sf::IntRect( 10, 10, WINDOW_WIDTH - 50, 30 ) ) );
    mBackgroundRect = bg.get();
    mBackgroundRect->getSprite( )->setFillColor( sf::Color( 181, 182, 228, 100 ) );
    mBackgroundRect->getSprite( )->setOutlineThickness( 1 );
    mBackgroundRect->getSprite( )->setOutlineColor( sf::Color::Black );
    this->attachChild( std::move( bg ) );

    std::unique_ptr<TextNode> unitMorale( new TextNode( fonts, "Morale: " + std::to_string( mSkillPoints ) ) );
    this->mMoralePercentageText = unitMorale.get( );
    this->mMoralePercentageText->getText()->setCharacterSize( 20 );
    this->mMoralePercentageText->setColor( sf::Color( 0, 0, 0, 0 ) );
    this->mMoralePercentageText->setPosition( WINDOW_WIDTH / 2, 25 );
    this->attachChild( std::move( unitMorale ) );

    std::unique_ptr<TextNode> mMorale( new TextNode( fonts, "Gold: " + std::to_string( mMoralPoints ) ) );
    this->mMoralePointsText = mMorale.get( );
    this->mMoralePointsText->getText()->setCharacterSize( 20 );
    this->mMoralePointsText->setColor( sf::Color( 0, 0, 0, 0 ) );
    this->mMoralePointsText->setPosition( 75, 25 );
    this->attachChild( std::move( mMorale ) );

    std::unique_ptr<TextNode> skill( new TextNode( fonts, "Exp: " + std::to_string( mSkillPoints ) ) );
    this->mSkillPointsText = skill.get( );
    this->mSkillPointsText->getText()->setCharacterSize( 20 );
    this->mSkillPointsText->setColor( sf::Color( 0, 0, 0, 0 ) );
    this->mSkillPointsText->setPosition( WINDOW_WIDTH - 125, 25 );
    this->attachChild( std::move( skill ) );
/*
    std::unique_ptr<TextNode> numUnits( new TextNode( fonts, "Units: " + std::to_string( mSkillPoints ) ) );
    this->mNumUnitsText = numUnits.get( );
    this->mNumUnitsText->getText()->setCharacterSize( 20 );
    this->mNumUnitsText->setColor( sf::Color( 0, 0, 0, 0 ) );
    this->mNumUnitsText->setPosition( WINDOW_WIDTH - 100, 25 );
    this->attachChild( std::move( numUnits ) );
*/

}

unsigned int TeamStats::getCategory( ) const
{
    return mTeamColor;
}

bool TeamStats::isMarkedForRemoval( ) const
{
    return false;
}

bool TeamStats::isDestroyed( ) const
{
    return false;
}

void TeamStats::updateCurrent( sf::Time, CommandQueue& )
{
    if( !mIsDisplaying && mDisplay )
    {
        mIsDisplaying = true;
        // Activate graphics
        mMoralePointsText->setColor( this->mColor );
        mSkillPointsText->setColor( this->mColor );
        mMoralePercentageText->setColor( this->mColor );
        //this->mNumUnitsText->setColor( this->mColor );
    }
    else if( mIsDisplaying && !mDisplay )
    {
        mIsDisplaying = false;
        // Deactivate Graphics
        mMoralePointsText->setColor( sf::Color( 0, 0, 0, 0 ) );
        mSkillPointsText->setColor( sf::Color( 0, 0, 0, 0 ) );
        mMoralePercentageText->setColor( sf::Color( 0, 0, 0, 0 ) );
        //this->mNumUnitsText->setColor( sf::Color( 0, 0, 0, 0 ) );
    }
}

void TeamStats::drawCurrent( sf::RenderTarget&, sf::RenderStates ) const
{

}

void TeamStats::changeStat( Stats stat, int amount )
{
    switch( stat )
    {
    case Stats::Gold:
        mMoralPoints += amount;
        mMoralePointsText->setString( "Gold: " + std::to_string( mMoralPoints ) );
        break;
    case Stats::Exp:
        mSkillPoints += amount;
        mSkillPointsText->setString( "Exp: " + std::to_string( mSkillPoints ) );
        break;
    case Stats::Morale:
        mMoralePercentage += amount;
        mMoralePercentageText->setString( "Morale: " + std::to_string( mMoralePercentage ) );
        break;
    case Stats::Units:
        mNumUnits += amount;
        //mNumUnitsText->setString( "Units: " + std::to_string( mNumUnits ) );
        break;
    default:
        // Impossible Case
        break;
    }
}

void TeamStats::setStat( Stats stat, int amount )
{
    switch( stat )
    {
    case Stats::Gold:
        mMoralPoints = amount;
        mMoralePointsText->setString( "Gold: " + std::to_string( mMoralPoints ) );
        break;
    case Stats::Exp:
        mSkillPoints = amount;
        mSkillPointsText->setString( "Exp: " + std::to_string( mSkillPoints ) );
        break;
    case Stats::Morale:
        mMoralePercentage = amount;
        mMoralePercentageText->setString( "Morale: " + std::to_string( mMoralePercentage ) );
        break;
    case Stats::Units:
        mNumUnits = amount;
        //mNumUnitsText->setString( "Units: " + std::to_string( mNumUnits ) );
        break;
    default:
        // Impossible Case
        break;
    }
}
