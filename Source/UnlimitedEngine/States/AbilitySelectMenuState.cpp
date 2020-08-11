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
    , mWorld( world )
{
    sf::Vector2f pos = sf::Vector2f( world->mWorldView.getViewport().left, world->mWorldView.getViewport().top );
    //sf::Vector2f pos = sf::Vector2f( world->mMovementGrid->mSelectedGridIndex.x * TILE_SIZE, world->mMovementGrid->mSelectedGridIndex.y * TILE_SIZE );
    pos.x += WINDOW_WIDTH / 2;
    pos.y += WINDOW_HEIGHT / 2;

    // Standard buttons
    auto Move = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    Move->setPosition( pos.x - 200, pos.y + 64 );
    Move->setText( "Move" );
    Move->setCallback( [this, world] ( )
    {
        Unit* unit;
        for( auto u : mWorld->mCurrentUnits ) if( u->mID == mWorld->mSelectedUnit ) { unit = u;  break; }
        if( !unit->mHasMoved )
        {
            this->requestStackPop( );
            auto temp = unit->mGridIndex;
            world->showUnitMovement( temp.x, temp.y );
        }
    });

    auto Wait = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    Wait->setPosition( pos.x + 64, pos.y + 64 );
    Wait->setText( "Wait" );
    Wait->setCallback( [this] ( )
    {
        this->requestStackPop( );
        this->requestStackPush( States::WaitMenuState );
    });

    // Get current units abilities to name buttons
    std::vector<std::string> buttonNames;
    Unit* unit;
    for( auto u : mWorld->mCurrentUnits ) if( u->mID == mWorld->mSelectedUnit ) { unit = u;  break; }
    for( auto s : unit->mAbilities ) buttonNames.push_back( s.first );

    auto button1 = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    button1->setPosition( pos.x - 234, pos.y - 32 );
    button1->setText( buttonNames[0] );
    button1->setCallback( [=] ( )
    {
        // get targetSquares
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
            world->getTartgets( targets );
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
                bool flag = false;
                for( auto t : ability->AOE.at( "only" ) ) // use abililty on all units inside the ability AOE[0]
                {
                    // get all units inside AOE from grid
                    std::string id = world->mGrid.at( (t.y+unit->mGridIndex.y) * (world->mGridWidth) + (t.x+unit->mGridIndex.x) ).unitID;
                    for( auto u : mWorld->mCurrentUnits )
                        if( u->mID == id )
                        {
                            unit->useAbility( buttonNames[1], u );
                            flag = true;
                            break;
                        }
                }
                if( flag )
                {
                    this->requestStackPop( );
                    world->mWaitingForPlayer = false;
                }
            }

        }
    });

    auto button2 = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    button2->setPosition( pos.x - 64, pos.y - 100 );
    button2->setText( buttonNames[1] );
    button2->setCallback( [=] ( )
    {
        // get targetSquares
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
            this->mWorld->getTartgets( targets );
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
                bool flag = false;
                for( auto t : ability->AOE.at( "only" ) ) // use abililty on all units inside the ability AOE[0]
                {
                    // get all units inside AOE from grid
                    std::string id = this->mWorld->mGrid.at( (t.y+unit->mGridIndex.y) * (this->mWorld->mGridWidth) + (t.x+unit->mGridIndex.x) ).unitID;
                    for( auto u : mWorld->mCurrentUnits )
                        if( u->mID == id )
                        {
                            unit->useAbility( buttonNames[1], u );
                            flag = true;
                            break;
                        }
                }
                if( flag )
                {
                    this->requestStackPop( );
                    world->mWaitingForPlayer = false;
                }
            }          
        }
    });

    auto button3 = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    button3->setPosition( pos.x + 100, pos.y - 32 );
    button3->setText( buttonNames[2] );
    button3->setCallback( [=] ( )
    {
        // get targetSquares
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
            this->mWorld->getTartgets( targets );
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
                bool flag = false;
                for( auto t : ability->AOE.at( "only" ) ) // use abililty on all units inside the ability AOE[0]
                {
                    // get all units inside AOE from grid
                    std::string id = this->mWorld->mGrid.at( (t.y+unit->mGridIndex.y) * (this->mWorld->mGridWidth) + (t.x+unit->mGridIndex.x) ).unitID;
                    for( auto u : mWorld->mCurrentUnits )
                        if( u->mID == id )
                        {
                            unit->useAbility( buttonNames[1], u );
                            flag = true;
                            break;
                        }
                }
                if( flag )
                {
                    this->requestStackPop( );
                    world->mWaitingForPlayer = false;
                }
            }          
        }
    });

    mGUIContainer.pack( Move );
    mGUIContainer.pack( Wait );
    mGUIContainer.pack( button1 );
    mGUIContainer.pack( button2 );
    mGUIContainer.pack( button3 );
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
