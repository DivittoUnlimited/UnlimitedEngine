#ifndef STATESTACK_HPP
#define STATESTACK_HPP

#include "State.hpp"
#include "StateIdentifiers.hpp"
#include "ResourceManager.hpp"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include <vector>
#include <utility>
#include <functional>
#include <map>

// Forward decs
namespace sf
{
	class Event;
	class RenderWindow;
}

class StateStack : private sf::NonCopyable
{
public:
    enum Action
    {
        Push,
        Pop,
        Clear,
    };
    explicit StateStack( State::Context context );

    //## Interface
    template <typename T>
    void registerState(States::ID stateID );

    void update(sf::Time dt);
    void draw();
    void handleEvent(const sf::Event& event);

    void pushState(States::ID stateID);
    void popState();
    void clearStates();

    bool isEmpty() const;

private:
    //## Behaviors
    State::Ptr	createState(States::ID stateID);
    void		applyPendingChanges();

    struct PendingChange
    {
        explicit PendingChange(Action action, States::ID stateID = States::None);

        Action	   action;
        States::ID stateID;
    };

    //## Attributes
    std::vector<State::Ptr>                             mStack;
    std::vector<PendingChange>                          mPendingList;
    std::map<States::ID, std::function<State::Ptr()>>	mFactories;
    State::Context                                      mContext;
};

template <typename T>
void StateStack::registerState(States::ID stateID )
{
    mFactories[stateID] = [=] ()
	{
        return State::Ptr(new T( stateID, *this, mContext ) );
	};
}

#endif // STATESTACK_HPP
