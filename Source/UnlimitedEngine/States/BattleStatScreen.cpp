
#include "BattleStatScreen.hpp"
#include "MenuState.hpp"
#include "Gui/Button.hpp"
#include "Core/Utility.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/DataTables.hpp"
#include "Core/MusicPlayer.hpp"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>
#include "Core/Globals.hpp"
#include "Core/Utility.hpp"

#include <fstream>

BattleStatScreen::BattleStatScreen( States::ID id, StateStack& stack, Context context )
: State( id, stack, context )
, mGUIContainer( )
{
    // mContext.music->setVolume( 30 );
    // mContext.music->play( MusicMap.at( "MenuTheme" ) );

    mTitleText = sf::Text( "Battle Results!", mContext.fonts->get( FontMap.at( "Default" ) ), 100 );
    mTitleText.setFillColor( sf::Color( 181, 182, 228, 255 ) );
    centerOrigin( mTitleText );
    mTitleText.setPosition( WINDOW_WIDTH / 2, 200 );
    mTitleText.setOutlineColor( sf::Color( 79, 67, 174, 255 ) );
    mTitleText.setOutlineThickness( 5 );
    mTitleText.setStyle( sf::Text::Bold );

    auto exitButton = std::make_shared<GUI::Button>( *context.fonts, *context.textures );
    exitButton->setPosition( WINDOW_WIDTH / 2 - 100, 450 );
    exitButton->setText( "DONE" );
    exitButton->setCallback( [this] ( )
    {
        requestStackPop( );
        // need to request next state based on game mode....
        switch( GAME_MODE )
        {
            case GameModes::QuickBattle: requestStackPush( States::Menu ); break;
            case GameModes::StoryMode:
                switch( CURRENT_LEVEL )
                {
                    case States::Level1:
                        requestStackPush( States::IntroDialogState );
                    break;
                    case States::Level2:
                        requestStackPush( States::cutScene1State );
                    break;
                    case States::Level3:
                        requestStackPush( States::cutScene2State );
                    break;
                    case States::Level4:
                        requestStackPush( States::cutScene3State );
                    break;
                    case States::Level5:
                        requestStackPush( States::cutScene4State );
                    break;
                    case States::Level6:
                        requestStackPush( States::cutScene5State );
                    break;
                    case States::Level7:
                        requestStackPush( States::cutScene6State );
                    break;
                    case States::Level8:
                        requestStackPush( States::cutScene7State );
                    break;
                    case States::Level9:
                        requestStackPush( States::cutScene8State );
                    break;

                    default: break;
                }
            break;
            case GameModes::Conquest:    requestStackPush( States::Menu ); break;
            default: break;
        }


    });

    mGUIContainer.pack( exitButton );
}

BattleStatScreen::~BattleStatScreen()
{
   // mContext.music->stop( );
}

void BattleStatScreen::draw( )
{
    sf::RenderTarget& window = *getContext( ).window;

    window.setView( window.getDefaultView( ) );
    window.clear( );
    window.draw( mTitleText );
    window.draw( mGUIContainer );
}

bool BattleStatScreen::update( sf::Time )
{
    return true;
}

bool BattleStatScreen::handleEvent( const sf::Event& event )
{
    mGUIContainer.handleEvent( event );
    return false;
}
