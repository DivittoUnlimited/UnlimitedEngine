#include "Unit.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include "Core/DataTables.hpp"

Unit::Unit( unsigned int mId, UnitTypeData data, const TextureManager& textures )
    : mID( mId )
{
    // define unit based on data from lua
    this->mUnitType     = static_cast<unsigned int>( UnitTypeMap[data.type] );
    this->mStrength     = static_cast<unsigned int>( data.strength );
    this->mDexterity    = static_cast<unsigned int>( data.dexterity );
    this->mConstitution = static_cast<unsigned int>( data.constitution );
    this->mDefense      = static_cast<unsigned int>( data.defense );

    this->mSprite = sf::Sprite( textures.get( TextureMap.at( data.type ) ) );

}

unsigned int Unit::getCategory( ) const
{
    return Category::Unit;
}
bool Unit::isMarkedForRemoval( ) const
{
    return false;
}
bool Unit::isDestroyed( ) const
{
    return false;
}
void Unit::updateCurrent( sf::Time, CommandQueue& )
{

}
void Unit::drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const
{
    target.draw( mSprite, states );
}
