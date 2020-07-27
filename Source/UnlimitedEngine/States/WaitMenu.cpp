#include "WaitMenu.hpp"
#include "Core/Utility.hpp"
#include "Core/ResourceManager.hpp"
#include "Game/DataTables.hpp"
#include "Core/Globals.hpp"
#include "Gui/Button.hpp"
#include "Game/World.hpp"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include "Game/Dice.hpp"

WaitMenu::WaitMenu( States::ID id, StateStack& stack, Context context, World* world )
    : State( id, stack, context )
{
    sf::Vector2f pos = sf::Vector2f( world->mWorldView.getViewport().left, world->mWorldView.getViewport().top );
    pos.x += WINDOW_WIDTH / 2;
    pos.y += WINDOW_HEIGHT / 2;

    auto rest = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    rest->setPosition( pos.x - 64, pos.y - 96 );
    rest->setText( "Recover" );
    rest->setCallback( [this, world] ( )
    {
        Unit* unit;
        for( auto u : world->mCurrentUnits ) if( u->mID == world->mGrid[world->mSelectedGridIndex.y * world->mGridWidth + world->mSelectedGridIndex.x].unitID ) { unit = u;  break; }

        unit->addModifier( StatModifier( "stamina", "10+20", 1 ) );
        unit->addModifier( StatModifier( "morale",  "10+15", 1 ) );
        unit->mHasMoved = true;
        unit->mHasSpentAction = true;
        this->requestStackPop( );
    });

    auto evade = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    evade->setPosition( pos.x + 72, pos.y );
    evade->setText( "Evade" );
    evade->setCallback( [this, world] ( )
    {
        Unit* unit;
        for( auto u : world->mCurrentUnits ) if( u->mID == world->mGrid[world->mSelectedGridIndex.y * world->mGridWidth + world->mSelectedGridIndex.x].unitID ) { unit = u;  break; }
        unit->addModifier( StatModifier( "stamina", "10+20", 1 ) );
        unit->addModifier( StatModifier( "morale",  "10+15", 1 ) );
        unit->mHasMoved = true;
        unit->mHasSpentAction = true;
        this->requestStackPop( );
    });

    auto defend = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    defend->setPosition( pos.x - 214, pos.y );
    defend->setText( "Take Guard" );
    defend->setCallback( [this, world] ( )
    {
        Unit* unit;
        for( auto u : world->mCurrentUnits ) if( u->mID == world->mGrid[world->mSelectedGridIndex.y * world->mGridWidth + world->mSelectedGridIndex.x].unitID ) { unit = u;  break; }
        unit->addModifier( StatModifier( "stamina", "10+20", 1 ) );
        unit->addModifier( StatModifier( "morale",  "10+15", 1 ) );
        unit->mHasMoved = true;
        unit->mHasSpentAction = true;
        this->requestStackPop( );
    });

    auto none = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    none->setPosition( pos.x - 64, pos.y + 72 );
    none->setText( "Exit" );
    none->setCallback( [this] ( )
    {
        this->requestStackPop( );
        requestStackPush( States::AbilitySelectMenuState );
    });

    mGUIContainer.pack( rest );
    mGUIContainer.pack( evade );
    mGUIContainer.pack( defend );
    mGUIContainer.pack( none );
}

void WaitMenu::draw( )
{
    sf::RenderTarget& window = *getContext( ).window;
    window.setView(window.getDefaultView( ) );
    window.draw( mGUIContainer );
}

bool WaitMenu::update( sf::Time )
{
    return false;
}

bool WaitMenu::handleEvent( const sf::Event& event )
{
    if( event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape )
    {
        // Escape pressed, remove itself to return to the game
        requestStackPop( );
    }
    else
    {
        mGUIContainer.handleEvent( event );
    }
    return false;
}
