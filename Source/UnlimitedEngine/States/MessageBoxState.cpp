#include "MessageBoxState.hpp"

#include "Gui/Button.hpp"
#include "Core/Utility.hpp"
#include "Core/ResourceManager.hpp"
#include "Game/DataTables.hpp"
#include "Core/MusicPlayer.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include "Core/Globals.hpp"
#include "Core/Utility.hpp"
#include "Gui/MessageBoxNode.hpp"

/*
MessageBoxState::MessageBoxState( States::ID id, StateStack& stack, Context context )
: State( id, stack, context )
, mMessageBox( new MessageBoxNode( "CowboyBobIntro", *context.fonts ) )
{
    mMessageBox->setPosition( 50, 400 );
}
*/

MessageBoxState::MessageBoxState(States::ID id, StateStack& stack, Context context, std::string convoName )
    : State( id, stack, context )
    , mMessageBox( new MessageBoxNode( convoName, *context.fonts ) )
{
    mMessageBox->setPosition( 50, 400 );
}

MessageBoxState::~MessageBoxState()
{
}

void MessageBoxState::draw( )
{
    sf::RenderTarget& window = *getContext( ).window;
    window.setView( window.getDefaultView( ) );
    window.draw( *mMessageBox );
}

bool MessageBoxState::update( sf::Time )
{
    if( mMessageBox->complete( ) )
        requestStackPop( );
    return false;
}

bool MessageBoxState::handleEvent( const sf::Event& event )
{
    mMessageBox->handleEvent( event );
    return false;
}

