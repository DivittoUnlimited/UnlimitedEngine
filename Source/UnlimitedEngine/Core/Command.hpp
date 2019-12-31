#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Category.hpp"
#include "Graphics/ParticleNode.hpp"

#include <SFML/System/Time.hpp>

#include <functional>
#include <cassert>


class SceneNode;

struct Command
{
    Command();

    ///
    /// \brief action
    /// The method to be applied to each SceneNode that matches the category
	std::function<void(SceneNode&, sf::Time)>	action;
    ///
    /// \brief category
    /// The category of SceneNodes that this command will apply to.
	unsigned int								category;
};

template <typename GameObject, typename Function>
std::function<void( SceneNode&, sf::Time )> derivedAction( Function fn )
{
    return [=] ( SceneNode& node, sf::Time dt )
	{
		// Check if cast is safe
        assert( dynamic_cast<GameObject*>( &node ) != nullptr );

		// Downcast node and invoke function on it
        fn( static_cast<GameObject&>( node ), dt );
	};
}

#endif // COMMAND_HPP

