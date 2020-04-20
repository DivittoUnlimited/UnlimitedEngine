#ifndef TEAMSTATS_HPP
#define TEAMSTATS_HPP

#include "Graphics/SceneNode.hpp"
#include "Graphics/TextNode.hpp"
#include "Graphics/RectangleShapeNode.hpp"

enum Stats
{
      Gold
    , Exp
    , Morale
    , Units
};

class TeamStats : public SceneNode
{
public:
    TeamStats( Category::Type teamColor, FontManager& fonts );

    virtual unsigned int	getCategory( ) const;
    virtual bool			isMarkedForRemoval( ) const;
    virtual bool			isDestroyed( ) const;
    virtual void			updateCurrent( sf::Time dt, CommandQueue& commands );
    virtual void			drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;

    void changeStat( Stats stat, int amount );
    void setStat( Stats stat, int amount );

    //## Attributes
    bool         mDisplay;
private:
    unsigned int mMoralPoints;
    unsigned int mSkillPoints;
    float        mMoralePercentage;
    unsigned int mNumUnits;
    unsigned int mNumScouts;

    // Gui
    Category::Type      mTeamColor;
    sf::Color           mColor;
    RectangleShapeNode* mBackgroundRect;
    TextNode*           mMoralePointsText;
    TextNode*           mSkillPointsText;
    TextNode*           mMoralePercentageText;
    // TextNode*           mNumUnitsText;

    bool                mIsDisplaying;
};

#endif // TEAMSTATS_HPP
