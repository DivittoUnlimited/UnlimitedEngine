#ifndef UNIT_HPP
#define UNIT_HPP

#include "queue"
#include "Graphics/SceneNode.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include "Graphics/RectangleShapeNode.hpp"
#include "Core/Utility.hpp"
#include "Core/PathFinder.hpp"
#include "Game/DataTables.hpp"
#include "Game/StatModHUD.hpp"

class Square;

class Unit : public SceneNode
{
public:
    Unit(unsigned int mId, Category::Type category, UnitTypeData data , const TextureManager &textures , const FontManager &fonts);
    ~Unit( void );
    virtual unsigned int	getCategory( ) const;
    virtual bool			isMarkedForRemoval( ) const;
    virtual bool			isDestroyed( ) const;
    virtual void			updateCurrent( sf::Time dt, CommandQueue& commands );
    virtual void			drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;

    /// \brief useAbility
    /// Called from the Action/useAbility menu this method adds modifiers from the named ability to both
    /// the target unit provided as param and this unit
    /// \param abilityID
    /// \param target
    ///
    void                    useAbility( std::string abilityID, Unit* target );

    /// \brief addModifier
    /// Used to add effects to list of effects that are used on this unit during the update method (at the beggining of the turn)
    /// \param mod
    ///
    void                    addModifier( StatModifier mod );

    void                    modHealth( int amount );
    void                    modMorale( int amount );

    //## Attributes
    unsigned int mID;
    sf::Sprite mSprite;
    unsigned int mUnitType;
    Category::Type mCategory;
    sf::Vector2i mGridIndex;

    unsigned int mAttack;
    unsigned int mSpeed;
    int mHealth;
    int mMaxHealth;
    unsigned int mArmour;

    /// \brief mInitiative
    /// Used to determine the turn of order of all the units on the field.
    unsigned int mInitiative;

    bool mBeginingOfTurn;

    /// \brief mPerception
    /// How far the unit breaks the fog of war so that the player can see the world around it.
    unsigned int mPerception;
    /// \brief mStamina
    ///
    int mStamina;
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
    float mBoardSpeed;
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
    /// \brief mAbilities
    /// List of all the available abilities the unit has.
    std::map<std::string, AbilityData> mAbilities;
    /// \brief mSelectedAbility
    /// Used to hold the current ability until a target ca nbe selected by the grid class
    std::string mSelectedAbility;
    /// \brief mCurrentModifiers
    /// list of increments and decrements that occur to unit at the beggining of the units turn
    std::vector<StatModifier> mCurrentModifiers;
    /// \brief mHealthBarBorder
    /// The fancy border around the health bar
    RectangleShapeNode* mHealthBarBorder;
    /// \brief mHealthBar
    /// The best indication of health :)
    RectangleShapeNode* mHealthBar;
    /// \brief mMoraleBar
    /// The best indication of the units morale
    RectangleShapeNode* mMoraleBar;
    /// \brief mFonts
    /// reference to the fontManager used throughout the game to create the StatModHUD objs when an ability is used on this obj.
    const FontManager& mFonts;
    /// \brief mCurrentStatModHUD
    /// Pointer to the text that floats above the unit when something changes one of it's stats
    StatModHUD* mCurrentStatModHUD;
    /// \brief mInitiativeHUD
    /// Heads up display of this units current turn order
    sf::Text mInitiativeHUD;
    sf::CircleShape mInitiativeHUDBackground;

};

#endif // UNIT_HPP
