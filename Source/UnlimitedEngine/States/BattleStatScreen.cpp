
#include "BattleStatScreen.hpp"
#include "MenuState.hpp"
#include "Gui/Button.hpp"
#include "Core/Utility.hpp"
#include "Core/ResourceManager.hpp"
#include "Game/DataTables.hpp"
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
                    case States::SinglePlayerLevel1:
                        CURRENT_LEVEL = States::SinglePlayerLevel2;
                    break;
                    case States::SinglePlayerLevel2:
                        CURRENT_LEVEL = States::SinglePlayerLevel3;
                    break;
                    case States::SinglePlayerLevel3:
                        CURRENT_LEVEL = States::SinglePlayerLevel4;
                    break;
                    case States::SinglePlayerLevel4:
                        CURRENT_LEVEL = States::SinglePlayerLevel5;
                    break;
                    case States::SinglePlayerLevel5:
                        CURRENT_LEVEL = States::SinglePlayerLevel6;
                    break;
                    case States::SinglePlayerLevel6:
                        CURRENT_LEVEL = States::SinglePlayerLevel7;
                    break;
                    case States::SinglePlayerLevel7:
                        CURRENT_LEVEL = States::SinglePlayerLevel8;
                    break;
                    case States::SinglePlayerLevel8:
                        CURRENT_LEVEL = States::SinglePlayerLevel9;
                    break;
                    case States::SinglePlayerLevel9:
                        CURRENT_LEVEL = States::SinglePlayerLevel10;
                    break;

                    default: break;
                }
                switch( CURRENT_LEVEL )
                {
                    case States::SinglePlayerLevel1:
                        requestStackPush( States::IntroCutSceneDialogState );
                    break;
                    case States::SinglePlayerLevel2:
                        requestStackPush( States::cutScene1State );
                    break;
                    case States::SinglePlayerLevel3:
                        requestStackPush( States::cutScene2State );
                    break;
                    case States::SinglePlayerLevel4:
                        requestStackPush( States::cutScene3State );
                    break;
                    case States::SinglePlayerLevel5:
                        requestStackPush( States::cutScene4State );
                    break;
                    case States::SinglePlayerLevel6:
                        requestStackPush( States::cutScene5State );
                    break;
                    case States::SinglePlayerLevel7:
                        requestStackPush( States::cutScene6State );
                    break;
                    case States::SinglePlayerLevel8:
                        requestStackPush( States::cutScene7State );
                    break;
                    case States::SinglePlayerLevel9:
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
