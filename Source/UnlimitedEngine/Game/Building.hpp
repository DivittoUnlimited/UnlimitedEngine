#ifndef BUILDING_HPP
#define BUILDING_HPP

#include "Graphics/SceneNode.hpp"
#include "Core/Category.hpp"
#include <SFML/Graphics/Sprite.hpp>

#include "Game/DataTables.hpp"
#include <string>

class Building : public SceneNode
{
public:
    Building( unsigned int mId, Category::Type category, BuildingData data , const TextureManager &textures );
    virtual unsigned int	getCategory( ) const;
    virtual bool			isMarkedForRemoval( ) const;
    virtual bool			isDestroyed( ) const;
    virtual void			updateCurrent( sf::Time dt, CommandQueue& commands );
    virtual void			drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;

    // Attributes
    unsigned int mID;
    Category::Type mCategory;
    std::string mType;
    unsigned int mGoldPerTurn;
    std::string mTextureID;
    sf::Sprite mSprite;
    sf::Vector2i mGridIndex;
    float mCapturePercentage;
};

#endif // BUILDING_HPP
