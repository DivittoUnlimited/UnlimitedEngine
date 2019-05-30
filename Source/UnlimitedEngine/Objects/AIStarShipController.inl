
#include "Objects/AIStarShipController.hpp"

template <class T>
void IdleState<T>::update( sf::Time, CommandQueue&, T* owner )
{
    owner->rotateRight( );
}

template<class T>
void IdleState<T>::onEnter( T* owner, void* )
{
    std::cout << "IdleState onEnter( ) entered." << std::endl;

    owner->mFsm.changeState( new MoveToState<AIStarShipController>(), static_cast<void*>( new sf::Vector2f( 500, 600 ) ) );
}

template<class T>
void IdleState<T>::onExit( T* )
{
    std::cout << "IdleState onExit( ) entered" << std::endl;
}

// ====================================================================================

template<class T>
void MoveToState<T>::update( sf::Time, CommandQueue&, T* owner )
{
    // Calculate needed trajectory, align ship, asign movement?

    owner->fire( );
}

template<class T>
void MoveToState<T>::onEnter( T* owner, void* data )
{
    mTargetPos = *static_cast<sf::Vector2f*>(data);
    std::cout << "Attempting to MoveTo: ( X = " << mTargetPos.x << " Y = " << mTargetPos.y << " )" << std::endl;
    // Calculate needed trajectory
    // build list of commands and times to execute to acheive goal
    // make sure list is sorted


}

template<class T>
void MoveToState<T>::onExit( T* owner )
{
    std::cout << "ID: " << owner->getCategory() << "MoveToState onExit( ) Complete!" << std::endl;
}
