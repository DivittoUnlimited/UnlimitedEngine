#include "AbilitySelectMenuState.hpp"
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

AbilitySelectMenuState::AbilitySelectMenuState( States::ID id, StateStack& stack, Context context, World* world )
    : State( id, stack, context )
{
    sf::Vector2f pos = sf::Vector2f( world->mMovementGrid->mSelectedGridIndex.x * TILE_SIZE, world->mMovementGrid->mSelectedGridIndex.y * TILE_SIZE );
    pos -= world->mDeltaMousePosition;

    // Get current units abilities to name buttons
    std::vector<std::string> buttonNames;

    Unit* unit = world->mMovementGrid->mCurrentUnits.at( world->mMovementGrid->mData[world->mMovementGrid->mSelectedGridIndex.y * world->mMovementGrid->mGridWidth + world->mMovementGrid->mSelectedGridIndex.x].unitID );
    for( auto s : unit->mAbilities ) buttonNames.push_back( s.first );

    auto button1 = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    button1->setPosition( pos.x - 64, pos.y - 96 );
    button1->setText( buttonNames[0] );
    button1->setCallback( [=] ( )
    {
        // get targetSquares
        world->setSelectedUnit( unit->mID );
        unit->mSelectedAbility = buttonNames[0];
        AbilityData* ability = &unit->mAbilities.at( buttonNames[0] );
        std::vector<sf::Vector2i> targets;

        if( ability->range > 0 )
        {
            // build list of all squares within range around player (relative)
            for( int x = -1 * (ability->range); x <= static_cast<int>( ability->range ); ++x )
                for( int y = -1 * (ability->range); y <= static_cast<int>( ability->range ); ++y )
                {
                    targets.push_back( sf::Vector2i( x, y ) );
                    targets.back() += unit->mGridIndex;
                }
            world->mMovementGrid->getTartgets( targets );
            this->requestStackPop( );
        }
        else if( ability->range == 0 ) // attack originates from user
        {
            ability->origin = sf::Vector2i( unit->mGridIndex );
            if( ability->hasRotation )
            {
                // get rotation from player
                this->requestStackPop( );
                this->requestStackPush( States::RotationSelectMenuState );
            }
            else
            {
                for( auto t : ability->AOE.at( "only" ) ) // use abililty on all units inside the ability AOE[0]
                {
                    // get all units inside AOE from grid
                    int id = world->mMovementGrid->mData.at( (t.y+unit->mGridIndex.y) * (world->mMovementGrid->mGridWidth) + (t.x+unit->mGridIndex.x) ).unitID;
                    if( id > 0 && id != static_cast<int>( unit->mID ) ) unit->useAbility( buttonNames[0], world->mMovementGrid->mCurrentUnits.at( id ) );
                }
                this->requestStackPop( );
                unit->mSelectedAbility = "NONE";
                unit->mHasSpentAction = true;
                unit->mIsSelectedUnit = false;
            }
        }
    });

    auto button2 = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    button2->setPosition( pos.x + 72, pos.y );
    button2->setText( buttonNames[1] );
    button2->setCallback( [=] ( )
    {
        // get targetSquares
        world->setSelectedUnit( unit->mID );
        unit->mSelectedAbility = buttonNames[1];
        AbilityData* ability = &unit->mAbilities.at( buttonNames[1] );
        std::vector<sf::Vector2i> targets;

        if( ability->range > 0 )
        {
            // build list of all squares within range around player (relative)
            for( int x = -1 * (ability->range); x <= static_cast<int>( ability->range ); ++x )
                for( int y = -1 * (ability->range); y <= static_cast<int>( ability->range ); ++y )
                {
                    targets.push_back( sf::Vector2i( x, y ) );
                    targets.back() += unit->mGridIndex;
                }
            world->mMovementGrid->getTartgets( targets );
            this->requestStackPop( );
        }
        else if( ability->range == 0 ) // attack originates from user
        {
            ability->origin = sf::Vector2i( unit->mGridIndex );
            if( ability->hasRotation )
            {
                // get rotation from player
                this->requestStackPop( );
                this->requestStackPush( States::RotationSelectMenuState );
            }
            else
            {
                for( auto t : ability->AOE.at( "only" ) ) // use abililty on all units inside the ability AOE[0]
                {
                    // get all units inside AOE from grid
                    int id = world->mMovementGrid->mData.at( (t.y+unit->mGridIndex.y) * (world->mMovementGrid->mGridWidth) + (t.x+unit->mGridIndex.x) ).unitID;
                    if( id > 0 && id != static_cast<int>( unit->mID ) ) unit->useAbility( buttonNames[1], world->mMovementGrid->mCurrentUnits.at( id ) );
                }
                this->requestStackPop( );
                unit->mSelectedAbility = "NONE";
                unit->mHasSpentAction = true;
                unit->mIsSelectedUnit = false;
            }
        }
    });

    auto button3 = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    button3->setPosition( pos.x - 214, pos.y );
    button3->setText( buttonNames[2] );
    button3->setCallback( [=] ( )
    {
        // get targetSquares
        world->setSelectedUnit( unit->mID );
        unit->mSelectedAbility = buttonNames[2];
        AbilityData* ability = &unit->mAbilities.at( buttonNames[2] );
        std::vector<sf::Vector2i> targets;

        if( ability->range > 0 )
        {
            // build list of all squares within range around player (relative)
            for( int x = -1 * (ability->range); x <= static_cast<int>( ability->range ); ++x )
                for( int y = -1 * (ability->range); y <= static_cast<int>( ability->range ); ++y )
                {
                    targets.push_back( sf::Vector2i( x, y ) );
                    targets.back() += unit->mGridIndex;
                }
            world->mMovementGrid->getTartgets( targets );
            this->requestStackPop( );
        }
        else if( ability->range == 0 ) // attack originates from user
        {
            ability->origin = sf::Vector2i( unit->mGridIndex );
            if( ability->hasRotation )
            {
                // get rotation from player
                this->requestStackPop( );
                this->requestStackPush( States::RotationSelectMenuState );
            }
            else
            {
                for( auto t : ability->AOE.at( "only" ) ) // use abililty on all units inside the ability AOE[0]
                {
                    // get all units inside AOE from grid
                    int id = world->mMovementGrid->mData.at( (t.y+unit->mGridIndex.y) * (world->mMovementGrid->mGridWidth) + (t.x+unit->mGridIndex.x) ).unitID;
                    if( id > 0 && id != static_cast<int>( unit->mID ) ) unit->useAbility( buttonNames[2], world->mMovementGrid->mCurrentUnits.at( id ) );
                }
                this->requestStackPop( );
                unit->mSelectedAbility = "NONE";
                unit->mHasSpentAction = true;
                unit->mIsSelectedUnit = false;;
            }
        }
    });

    auto exit = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    exit->setPosition( pos.x - 64, pos.y + 72 );
    exit->setText( "Back" );
    exit->setCallback( [this] ( )
    {
        requestStackPop( );
    });

    mGUIContainer.pack( button1 );
    mGUIContainer.pack( button2 );
    mGUIContainer.pack( button3 );
    mGUIContainer.pack( exit );
}

void AbilitySelectMenuState::draw( )
{
    sf::RenderTarget& window = *getContext( ).window;
    window.setView(window.getDefaultView( ) );

    mWindow->draw( mBackground );
    mWindow->draw( mText );
    window.draw( mGUIContainer );
}

bool AbilitySelectMenuState::update( sf::Time )
{
    return false;
}

bool AbilitySelectMenuState::handleEvent( const sf::Event& event )
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
