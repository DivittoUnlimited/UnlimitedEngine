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
    , mWorld( world )
{
    Unit* unit;
    for( auto u : mWorld->mCurrentUnits ) if( u->mID == world->mGrid[world->mSelectedGridIndex.y * world->mGridWidth + world->mSelectedGridIndex.x].unitID ) { unit = u;  break; }

    AbilityData* ability = &unit->mAbilities.at( unit->mSelectedAbility );

    sf::Vector2f pos = sf::Vector2f( world->mWorldView.getViewport().left, world->mWorldView.getViewport().top );
    pos.x += WINDOW_WIDTH / 2;
    pos.y += WINDOW_HEIGHT / 2;

    auto button1 = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    button1->setPosition( pos.x - 64, pos.y - 96 );
    button1->setText( "NORTH" );
    button1->setCallback( [=] ( )
    {
        bool flag = false;
        for( auto t : ability->AOE.at( "north" ) ) // use abililty on all units inside the ability AOE[0]
        {
            // get all units inside AOE from grid
            std::string id = world->mGrid.at( (t.y+unit->mGridIndex.y) * (world->mGridWidth) + (t.x+unit->mGridIndex.x) ).unitID;
            for( auto u : mWorld->mCurrentUnits ) if( u->mID == id )
            {
                unit->useAbility( unit->mSelectedAbility, u );
                flag = true;
                break;
            }
        }
        this->requestStackPop( );
        world->mWaitingForPlayer = false;
    });

    auto button2 = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    button2->setPosition( pos.x + 72, pos.y );
    button2->setText( "EAST" );
    button2->setCallback( [=] ( )
    {
        bool flag = false;
        for( auto t : ability->AOE.at( "east" ) ) // use abililty on all units inside the ability AOE[0]
        {
            // get all units inside AOE from grid
            std::string id = world->mGrid.at( (t.y+unit->mGridIndex.y) * (world->mGridWidth) + (t.x+unit->mGridIndex.x) ).unitID;
            for( auto u : mWorld->mCurrentUnits ) if( u->mID == id )
            {
                unit->useAbility( unit->mSelectedAbility, u );
                flag = true;
                break;
            }
        }
        this->requestStackPop( );
        world->mWaitingForPlayer = false;
    });

    auto button3 = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    button3->setPosition( pos.x - 64, pos.y + 72 );
    button3->setText( "SOUTH" );
    button3->setCallback( [=] ( )
    {
        bool flag = false;
        for( auto t : ability->AOE.at( "south" ) ) // use abililty on all units inside the ability AOE[0]
        {
            // get all units inside AOE from grid
            std::string id = world->mGrid.at( (t.y+unit->mGridIndex.y) * (world->mGridWidth) + (t.x+unit->mGridIndex.x) ).unitID;
            for( auto u : mWorld->mCurrentUnits ) if( u->mID == id )
            {
                unit->useAbility( unit->mSelectedAbility, u );
                flag = true;
                break;
            }
        }
        this->requestStackPop( );
        world->mWaitingForPlayer = false;
    });

    auto button4 = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    button4->setPosition( pos.x - 214, pos.y );
    button4->setText( "WEST" );
    button4->setCallback( [=] ( )
    {
        bool flag = false;
        for( auto t : ability->AOE.at( "west" ) ) // use abililty on all units inside the ability AOE[0]
        {
            // get all units inside AOE from grid
            std::string id = world->mGrid.at( (t.y+unit->mGridIndex.y) * (world->mGridWidth) + (t.x+unit->mGridIndex.x) ).unitID;
            for( auto u : mWorld->mCurrentUnits ) if( u->mID == id )
            {
                unit->useAbility( unit->mSelectedAbility, u );
                flag = true;
                break;
            }
        }
        this->requestStackPop( );
        world->mWaitingForPlayer = false;
    });

    auto exit = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    exit->setPosition( pos.x + 200, pos.y + 200 );
    exit->setText( "Exit" );
    exit->setCallback( [this] ( )
    {
        requestStackPop( );
        requestStackPush( States::AbilitySelectMenuState );
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
    // Update the view
    if( sf::Keyboard::isKeyPressed( sf::Keyboard::W ) ) // || sf::Mouse::getPosition().y < 100 )
    {
        this->mGUIContainer.move( 0, mWorld->mCameraPanSpeed );
    }
    else if( sf::Keyboard::isKeyPressed( sf::Keyboard::S  ) ) // || sf::Mouse::getPosition().y > WINDOW_HEIGHT - 100 )
    {
        this->mGUIContainer.move( 0, -1 * mWorld->mCameraPanSpeed );
    }
    else if( sf::Keyboard::isKeyPressed( sf::Keyboard::A  ) ) //  || sf::Mouse::getPosition().x < 100 )
    {
        this->mGUIContainer.move( mWorld->mCameraPanSpeed, 0 );
    }
    else if( sf::Keyboard::isKeyPressed( sf::Keyboard::D ) ) // || sf::Mouse::getPosition().x > WINDOW_WIDTH - 100 )
    {
        this->mGUIContainer.move( -1 * mWorld->mCameraPanSpeed, 0 );
    }
    return true;
}

bool RotationSelectMenuState::handleEvent( const sf::Event& event )
{
    //if( event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape )
    {
        // Escape pressed, remove itself to return to the game
        //requestStackPop( );
    }
    //else
    {
        mGUIContainer.handleEvent( event );
    }
    return false;
}
