#include "RotationSelectMenuState.hpp"

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

RotationSelectMenuState::RotationSelectMenuState( States::ID id, StateStack& stack, Context context, World* world )
    : State( id, stack, context )
{
    sf::Vector2f pos = sf::Vector2f( world->mMovementGrid->mSelectedGridIndex.x * TILE_SIZE, world->mMovementGrid->mSelectedGridIndex.y * TILE_SIZE );
    pos -= world->mDeltaMousePosition;

    // Get current units abilities to name buttons
    std::vector<std::string> buttonNames;

    Unit* unit = world->mMovementGrid->mCurrentUnits.at( world->mMovementGrid->mData[world->mMovementGrid->mSelectedGridIndex.y * world->mMovementGrid->mGridWidth + world->mMovementGrid->mSelectedGridIndex.x].unitID );
    AbilityData ability = unit->mAbilities.at( unit->mSelectedAbility );

    auto button1 = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    button1->setPosition( pos.x - 64, pos.y - 96 );
    button1->setText( "NORTH" );
    button1->setCallback( [=] ( )
    {
        requestStackPop( );
    });

    auto button2 = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    button2->setPosition( pos.x + 72, pos.y );
    button2->setText( "EAST" );
    button2->setCallback( [=] ( )
    {
        requestStackPop( );
    });

    auto button3 = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    button3->setPosition( pos.x - 64, pos.y + 72 );
    button3->setText( "SOUTH" );
    button3->setCallback( [=] ( )
    {
        requestStackPop( );
    });

    auto button4 = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    button4->setPosition( pos.x - 214, pos.y );
    button4->setText( "WEST" );
    button4->setCallback( [=] ( )
    {
        requestStackPop( );
    });

    auto exit = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    exit->setPosition( pos.x + 200, pos.y + 200 );
    exit->setText( "Exit" );
    exit->setCallback( [this] ( )
    {
        requestStackPop( );
    });

    mGUIContainer.pack( button1 );
    mGUIContainer.pack( button2 );
    mGUIContainer.pack( button3 );
    mGUIContainer.pack( button4 );
    mGUIContainer.pack( exit );
}

void RotationSelectMenuState::draw( )
{
    sf::RenderTarget& window = *getContext( ).window;
    window.setView(window.getDefaultView( ) );
    window.draw( mGUIContainer );
}

bool RotationSelectMenuState::update( sf::Time )
{
    return false;
}

bool RotationSelectMenuState::handleEvent( const sf::Event& event )
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
