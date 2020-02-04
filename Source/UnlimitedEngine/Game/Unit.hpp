#ifndef UNIT_HPP
#define UNIT_HPP

#include "Graphics/SceneNode.hpp"
#include <SFML/Graphics/Sprite.hpp>

#include "Game/DataTables.hpp"



class Unit : public SceneNode
{
public:
    Unit(unsigned int mId, Category::Type category, UnitTypeData data , const TextureManager &textures );

    virtual unsigned int	getCategory( ) const;
    virtual bool			isMarkedForRemoval( ) const;
    virtual bool			isDestroyed( ) const;
    virtual void			updateCurrent( sf::Time dt, CommandQueue& commands );
    virtual void			drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;

    //## Attributes
    unsigned int mID;
    sf::Sprite mSprite;
    unsigned int mUnitType;
    Category::Type mCategory;

    unsigned int mStrength;
    unsigned int mDexterity;
    unsigned int mConstitution;
    unsigned int mDefense;
    sf::Vector2i mRange;
    bool mHasMoved;
    bool mHasSpentAction;

};

#endif // UNIT_HPP
