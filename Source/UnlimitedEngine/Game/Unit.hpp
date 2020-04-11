#ifndef UNIT_HPP
#define UNIT_HPP

#include "Graphics/SceneNode.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include "Graphics/RectangleShapeNode.hpp"

#include "Core/PathFinder.hpp"
#include "Game/DataTables.hpp"

class Square;

class Unit : public SceneNode
{
public:
    Unit(unsigned int mId, Category::Type category, UnitTypeData data , const TextureManager &textures );
    ~Unit( void );
    virtual unsigned int	getCategory( ) const;
    virtual bool			isMarkedForRemoval( ) const;
    virtual bool			isDestroyed( ) const;
    virtual void			updateCurrent( sf::Time dt, CommandQueue& commands );
    virtual void			drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;

    void                    takeDamage( int amount );

    //## Attributes
    unsigned int mID;
    sf::Sprite mSprite;
    unsigned int mUnitType;
    Category::Type mCategory;
    sf::Vector2i mGridIndex;

    unsigned int mAttack;
    unsigned int mDexterity;
    int mConstitution;
    int mMaxConstitution;
    unsigned int mDefense;
    sf::Vector2i mRange;
    unsigned int mPerception;
    bool mHasMoved;
    bool mHasSpentAction;
    bool mIsSelectedUnit;
    float mSpeed; // used to controll how fast the unit moves around the map
    PathFinder<Square>* mPath;
    sf::Vector2f mDestination;
    bool mIsVisible;

    RectangleShapeNode* mHealthBarBorder;
    RectangleShapeNode* mHealthBar;
};

#endif // UNIT_HPP
