#include "Unit.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include "Game/DataTables.hpp"

Unit::Unit( unsigned int mId, Category::Type category, UnitTypeData data, const TextureManager& textures )
    : mID( mId )
    , mHasMoved( false )
    , mHasSpentAction( false )
{
    // define unit based on data from lua
    this->mUnitType     = static_cast<unsigned int>( UnitTypeMap[data.type] );
    this->mCategory     = category;
    this->mStrength     = static_cast<unsigned int>( data.strength );
    this->mDexterity    = static_cast<unsigned int>( data.dexterity );
    this->mConstitution = static_cast<unsigned int>( data.constitution );
    this->mDefense      = static_cast<unsigned int>( data.defense );
    this->mRange        = data.range;

    this->mSprite = sf::Sprite( textures.get( TextureMap.at( data.textureID ) ) );


    if( mCategory & Category::Blue )
        this->mSprite.setColor( sf::Color::Blue );
    else if( mCategory & Category::Red )
        this->mSprite.setColor( sf::Color::Red );

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

}
void Unit::drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const
{
    target.draw( mSprite, states );
}
