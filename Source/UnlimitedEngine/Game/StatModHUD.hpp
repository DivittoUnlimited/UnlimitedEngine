#ifndef STATMODHUD_HPP
#define STATMODHUD_HPP

#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Time.hpp>

#include "Game/DataTables.hpp"
#include <Graphics/SceneNode.hpp>

class StatModHUD : public SceneNode
{
public:
    StatModHUD(const FontManager &fonts, std::string text , sf::Vector2i pos );
    virtual unsigned int	getCategory( ) const;
    virtual bool			isMarkedForRemoval( ) const;
    virtual bool			isDestroyed( ) const;
    virtual void			updateCurrent( sf::Time dt, CommandQueue& commands );
    virtual void			drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;

    sf::Time mTimer;
    sf::Text mText;
    int mStartX;
    int mStartY;
};

#endif // STATMODHUD_HPP
