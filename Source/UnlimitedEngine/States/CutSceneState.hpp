#ifndef CUTSCENESTATE_HPP
#define CUTSCENESTATE_HPP

#include "States/MessageBoxState.hpp"

class CutSceneState : public State
{
public:
    CutSceneState( States::ID id, StateStack& stack, Context context, States::ID nextState = States::Level3 );
    ~CutSceneState( void );
    virtual void draw( void );
    virtual bool update( sf::Time dt );
    virtual bool handleEvent( const sf::Event& event );

private:
    MessageBoxNode* mMessageBox;
    States::ID mNextState;
    bool mHasRanDialog;
};

#endif // INTROCUTSCENESTATE_HPP
