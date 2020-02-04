#include "Building.hpp"

Building::Building( unsigned int mId, Category::Type category, BuildingData data , const TextureManager &textures )
     : mID( mId )
{
    this->mCategory = category;
    this->mSprite = sf::Sprite( textures.get( TextureMap.at( data.type ) ) );
}

unsigned int Building::getCategory( ) const
{
    return mCategory;
}
bool Building::isMarkedForRemoval( ) const
{
    return isDestroyed( );
}
bool Building::isDestroyed( ) const
{
    return false;
}
void Building::updateCurrent( sf::Time, CommandQueue& )
{

}
void Building::drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const
{
    target.draw( mSprite, states );
}
