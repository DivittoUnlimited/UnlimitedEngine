#include "ActionMenuState.hpp"

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

ActionMenuState::ActionMenuState( States::ID id, StateStack& stack, Context context, World* world )
    : State( id, stack, context )
{
    sf::Vector2f pos = sf::Vector2f( world->mMovementGrid->mSelectedGridIndex.x * TILE_SIZE, world->mMovementGrid->mSelectedGridIndex.y * TILE_SIZE );
    pos -= world->mDeltaMousePosition;

    auto Move = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    Move->setPosition( pos.x - 64, pos.y - 96 );
    Move->setText( "Move" );
    Move->setCallback( [this, world] ( )
    {
        world->mMovementGrid->selectUnit( world->mMovementGrid->mSelectedGridIndex.x, world->mMovementGrid->mSelectedGridIndex.y );
        this->requestStackPop( );
    });

    auto Actions = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    Actions->setPosition( pos.x + 72, pos.y );
    Actions->setText( "Actions" );
    Actions->setCallback( [this, world] ( )
    {
        this->requestStackPop( );
        Unit* unit = world->mMovementGrid->mCurrentUnits.at( world->mMovementGrid->mData[world->mMovementGrid->mSelectedGridIndex.y * world->mMovementGrid->mGridWidth + world->mMovementGrid->mSelectedGridIndex.x].unitID );
        if( !unit->mHasSpentAction )
            this->requestStackPush( States::AbilitySelectMenuState );
    });

    auto Wait = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    Wait->setPosition( pos.x - 214, pos.y );
    Wait->setText( "Wait" );
    Wait->setCallback( [this] ( )
    {
        this->requestStackPop( );
        this->requestStackPush( States::WaitMenuState );
    });

    auto none = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    none->setPosition( pos.x - 64, pos.y + 72 );
    none->setText( "Exit" );
    none->setCallback( [this] ( )
    {
        requestStackPop( );
    });

    mGUIContainer.pack( Move );
    mGUIContainer.pack( Actions );
    mGUIContainer.pack( Wait );
    mGUIContainer.pack( none );
}

void ActionMenuState::draw( )
{
    sf::RenderTarget& window = *getContext( ).window;
    window.setView(window.getDefaultView( ) );
    window.draw( mGUIContainer );
}

bool ActionMenuState::update( sf::Time )
{
    return false;
}

bool ActionMenuState::handleEvent( const sf::Event& event )
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
