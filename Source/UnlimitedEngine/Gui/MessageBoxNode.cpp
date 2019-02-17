#include "Gui/MessageBoxNode.hpp"

#include "Core/DataTables.hpp"
#include "Core/Globals.hpp"
#include "Core/TextNode.hpp"

#include "Gui/Container.hpp"
#include "Gui/Button.hpp"
#include "Gui/ResponseSelector.hpp"
namespace
{
    const auto ConvoTable = initializeConversationData; // Gives access to the data in Conversations.lua
}

MessageBoxNode::MessageBoxNode( std::string convoName, FontManager& fonts )
    : mConversation( ConvoTable[ConversationMap.at( convoName )] )
    , mFonts( &fonts )
    , mComplete( false )
{
    assert( mConversation.conversationBranches.size( ) ); // if this fires then the conversation being loaded did not get the right vector of DialogNodes and mConversations is empty

    // this will be loaded like a css style sheet but in lua (Gui.lua? MessageBox.lua?) for now this works
    mBackgroundBox = sf::RectangleShape( sf::Vector2f( WINDOW_WIDTH - 100, WINDOW_HEIGHT / 3 ) );
    mBackgroundBox.setFillColor( sf::Color( 204, 204, 255, 150 ) );
    mBackgroundBox.setOutlineThickness( 2 );
    mBackgroundBox.setOutlineColor( sf::Color::Black );

    // create text, message deffinition should be retreived from lua with what ever "Style" is being loaded like the background
    mMessage = sf::Text( "", fonts.get( FontMap.at( "Default" ) ) );
    mMessage.setPosition( mBackgroundBox.getPosition( ) + sf::Vector2f( 10, 10 ) );
    mMessage.setFillColor( sf::Color::Black );
    mMessage.setCharacterSize( 18 );

    transitionDialog( 0 );
}

void MessageBoxNode::drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const
{
    target.draw( mBackgroundBox, states );
    target.draw( mMessage, states );
    target.draw( mGUIContainer, states );
}

void MessageBoxNode::updateCurrent( sf::Time, CommandQueue& )
{
}

bool MessageBoxNode::handleEvent( const sf::Event& event )
{
    mGUIContainer.handleEvent( event );
    return true;
}

void MessageBoxNode::transitionDialog( int link )
{
    if( link < 0 ) /// Everything is working up to this point how to i get the state to pop from here!?
        mComplete = true;
    else
    {
        mGUIContainer.clear( );
        mAnswers.clear( );
        mMessage.setString( mConversation.conversationBranches[link].getDialog( ) );
        mAnswers =  mConversation.conversationBranches[link].getResponses( );
        sf::Vector2f pos =  mMessage.getPosition( ) + sf::Vector2f( 0, 100 ); // this needs to be changed!!!!
        for( unsigned int i = 0; i < mAnswers.size( ); ++i )
        {
            auto option = std::make_shared<GUI::ResponseSelector>( mAnswers[i].first.c_str( ), *mFonts );
            option->setPosition( pos.x, pos.y + i * 25 );
            option->setCallback( [this, i] ( )
            {
                transitionDialog( mAnswers[i].second );
            });
            mGUIContainer.pack( option );
        }
    }
}
