#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Command.hpp"
#include <SFML/Window/Event.hpp>
#include <map>
#include "Gui/Container.hpp"

class CommandQueue;

class Player
{
public:
    /// \brief The InputMode enum
    /// The seperate schemes of input that the player uses to control the game/world.
    enum InputMode
    {
        AvatarController,
        MenuNavogator,
        MessageBoxInput,
        InputCount
    };
    /// \brief The Action enum
    /// All possible Actions the user can make in the game world. These are linked with the button map the user uses as well as the engine
    /// event system.
    enum Action
    {
        MoveLeft,
        MoveRight,
        MoveUp,
        MoveDown,
        ActionButton,
        ActionCount
    };

    struct InputData {
        std::map<sf::Keyboard::Key, Action>	mKeyBinding;    // user modifiable map of key inputs and their corresponding game action
        std::map<Action, Command>			mActionBinding; // a map that links game actions and the Commands (events) they invoke.
    };

    Player( void );
    ///
    /// \brief handleEvent
    /// Translate user input into game useable commands
    /// \param event
    /// a system event such as a key press or window resize
    /// \param commands
    /// The list of game commands to be handled this frame. Gets generated from system events.
    ///
    void handleEvent( const sf::Event& event, CommandQueue& commands );
    ///
    /// \brief handleRealtimeInput
    /// Utilizes the SFML real time input checking to test the current state of inputs and builds
    /// this frames commands based on results
    /// \param commands
    /// The list of game commands to be handled this frame.
    void handleRealtimeInput( CommandQueue& commands );
    ///
    /// \brief assignKey
    /// Backend to allow the user to assign there own controls. Made easier because input and commands are seperated.
    /// \param action
    /// The action that requires a new key
    /// \param key
    /// the new key that will invoke the given action
    void assignKey(Action action, sf::Keyboard::Key key);
    ///
    /// \brief getAssignedKey
    /// returns the current key assigned to the given action if any or else sf::Keyboard::Unknown
    /// \param action
    /// the action in query
    /// \return
    /// sf::Keyboard enum
    sf::Keyboard::Key getAssignedKey( Action action ) const;
    ///
    /// \brief changeInputMode
    /// Switch between the controller setup for moving the avatar or a menu etc..
    /// \param input
    /// the type of input to switch to all possible inputs listed in the InputMode ennum above.
    void changeInputMode( InputMode input = InputMode::AvatarController );

    InputMode getInputMode( void ) const { return mSchemeName; }
private:
    ///
    /// \brief initializeActions
    /// Links Game commands to the players keybinded actions. i.e MoveLeft: AircraftMover( -speed, 0 )
    void initializeActions( InputMode mode, InputData& data );
    ///
    /// \brief isRealtimeAction
    /// Checks if the given action can be read in real time and returns result.
    /// \param action
    /// the action in query
    /// \return
    /// bool result
    bool isRealtimeAction( Action action );

    //## Attributes
    InputData*              mCurrentScheme;
    std::vector<InputData>  mSchemes;
    InputMode               mSchemeName;
};

#endif // PLAYER_HPP
