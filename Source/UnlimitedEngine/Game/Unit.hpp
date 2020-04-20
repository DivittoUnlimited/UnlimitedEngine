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

    void                    modHealth( int amount );
    void                    modMorale( int amount );

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
    /// \brief mRange
    /// The range of the unit's attack in squares
    sf::Vector2i mRange;
    /// \brief mPerception
    /// How far the unit breaks the fog of war so that the player can see the world around it.
    unsigned int mPerception;
    /// \brief mHasMoved
    /// this unit has moved already this turn.
    bool mHasMoved;
    /// \brief mHasSpentAction
    /// This unit has attacked or used it's ability already this turn.
    bool mHasSpentAction;
    /// \brief mIsSelectedUnit
    /// this is the unit the player is manipulating
    bool mIsSelectedUnit;
    /// \brief mSpeed
    /// used to controll how fast the unit moves around the map
    float mSpeed;
    /// \brief mPath
    /// Calculates movement from square to square.
    PathFinder<Square>* mPath;
    sf::Vector2f mDestination;
    /// \brief mIsVisible
    /// If this unit can be seen under the fog of war
    bool mIsVisible;
    /// \brief morale
    /// Works like stamina. Goes up when their team is winning or does well and vice - versa
    /// also used to calculate the player's currency.
    float mMorale;
    /// \brief mHealthBarBorder
    /// The fancy border around the health bar
    RectangleShapeNode* mHealthBarBorder;
    /// \brief mHealthBar
    /// The best indication of health :)
    RectangleShapeNode* mHealthBar;
    /// \brief mMoraleBar
    /// The best indication of the units morale
    RectangleShapeNode* mMoraleBar;
};

#endif // UNIT_HPP
