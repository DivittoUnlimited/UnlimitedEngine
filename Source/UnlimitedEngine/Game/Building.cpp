#include "Building.hpp"

Building::Building( unsigned int mId, Category::Type category, BuildingData data , const TextureManager &textures )
     : mID( mId )
{
    // define building based on data from lua
    this->mType     = data.type;
    this->mCategory     = category;
    this->mGoldPerTurn     = static_cast<unsigned int>( data.goldPerTurn );
    this->mTextureID    = data.textureID;

    this->mSprite = sf::Sprite( textures.get( TextureMap.at( data.textureID ) ) );

    if( mCategory & Category::Blue )
        this->mSprite.setColor( sf::Color::Blue );
    else if( mCategory & Category::Red )
        this->mSprite.setColor( sf::Color::Red );
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
