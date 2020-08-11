#include "Container.hpp"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "Core/Globals.hpp"

namespace GUI
{

Container::Container( )
: mChildren( )
, mSelectedChild( -1 )
{
}

void Container::pack( Component::Ptr component )
{
    mChildren.push_back( component );
    mChildren.back()->setIndex( mChildren.size() - 1 );
    if( !hasSelection( ) && component->isSelectable( ) )
        select( mChildren.size( ) - 1 );
}

bool Container::isSelectable( void ) const
{
    return false;
}

void Container::handleEvent( const sf::Event& event )
{

    if( event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left )
    {
        for( auto c : mChildren )
            if( c->contains( sf::Mouse::getPosition(*mWindow).x, sf::Mouse::getPosition(*mWindow).y ) )
                    c->activate();
    }
    else if( event.type == sf::Event::MouseMoved )
    {
        // pass event to children to see if this event affects them.
        for( auto c : mChildren )
        {
            if( c->contains( sf::Mouse::getPosition(*mWindow).x, sf::Mouse::getPosition(*mWindow).y ) )
            {
                this->mSelectedChild = static_cast<int>( c->getIndex( ) );
                c->select( );
            }
            else
                c->deselect( );
        }
    }
    // Keyboard input
    else if( hasSelection( ) && mChildren[static_cast<unsigned int>(mSelectedChild)]->isActive( ) )
        mChildren[static_cast<unsigned int>(mSelectedChild)]->handleEvent( event );
    else if( event.type == sf::Event::KeyReleased )
	{
        if( event.key.code == sf::Keyboard::Up )
            selectPrevious( );
        else if( event.key.code == sf::Keyboard::Down )
            selectNext( );
        else if( event.key.code == sf::Keyboard::Return || event.key.code == sf::Keyboard::Space )
            if( hasSelection( ) )
                mChildren[static_cast<unsigned int>(mSelectedChild)]->activate( );
	}
    // Game pad input
    else if( event.type == sf::Event::JoystickMoved )
    {
        if( event.joystickMove.axis == sf::Joystick::Y && event.joystickMove.position > 0 ) // up
            selectNext( );
        else if( event.joystickMove.axis == sf::Joystick::Y && event.joystickMove.position < 0 )
            selectPrevious( );
    }
    else if( event.type == sf::Event::JoystickButtonPressed )
        if( hasSelection( ) )
            mChildren[static_cast<unsigned int>(mSelectedChild)]->activate( );
}

void Container::draw( sf::RenderTarget& target, sf::RenderStates states ) const
{
    states.transform *= getTransform( );

    for( const Component::Ptr& child : mChildren )
		target.draw(*child, states);
}

bool Container::hasSelection( ) const
{
	return mSelectedChild >= 0;
}

void Container::select( std::size_t index )
{
    if( mChildren[index]->isSelectable( ) )
	{
        if( hasSelection( ) )
            mChildren[static_cast<unsigned int>(mSelectedChild)]->deselect( );

        mChildren[index]->select( );
		mSelectedChild = index;
	}
}

void Container::selectNext( void )
{
    if( !hasSelection( ) )
		return;

	// Search next component that is selectable, wrap around if necessary
    unsigned int next = static_cast<unsigned int>(mSelectedChild);
	do
        next = (next + 1) % mChildren.size( );
    while( !mChildren[next]->isSelectable( ) );

	// Select that component
    select( next );
}

void Container::selectPrevious( void )
{
    if( !hasSelection( ) )
		return;

	// Search previous component that is selectable, wrap around if necessary
    unsigned int prev = static_cast<unsigned int>(mSelectedChild);
	do
        prev = ( prev + mChildren.size() - 1) % mChildren.size( );
    while( !mChildren[prev]->isSelectable( ) );

	// Select that component
    select( prev );
}

void Container::clear( void )
{
    this->mChildren.clear( );
    this->mSelectedChild = -1;
}

}
