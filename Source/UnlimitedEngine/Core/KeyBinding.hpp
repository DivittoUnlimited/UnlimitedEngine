#ifndef KEYBINDING_HPP
#define KEYBINDING_HPP

#include <SFML/Window/Keyboard.hpp>

#include <map>
#include <vector>


namespace PlayerAction
{
	enum Type
	{
        LeftClick,
        RightClick,
		Count
	};
}

class KeyBinding
{
public:
    typedef PlayerAction::Type Action;


public:
    explicit			KeyBinding( int controlPreconfiguration );

    void				assignKey( Action action, sf::Keyboard::Key key );
    sf::Keyboard::Key	getAssignedKey( Action action ) const;

    bool				checkAction( sf::Keyboard::Key key, Action& out ) const;
    std::vector<Action>	getRealtimeActions( void ) const;


private:
    void initializeActions( void );


private:
		std::map<sf::Keyboard::Key, Action>		mKeyMap;
};

bool isRealtimeAction( PlayerAction::Type action );

#endif // KEYBINDING_HPP
