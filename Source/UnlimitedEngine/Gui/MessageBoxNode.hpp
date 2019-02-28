#ifndef MESSAGEBOXNODE_HPP
#define MESSAGEBOXNODE_HPP

#include <memory>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "Core/SceneNode.hpp"
#include "Core/DataTables.hpp"
#include "Core/DialogNode.hpp"

#include "Container.hpp"

class MessageBoxNode : public SceneNode
{
public:
    MessageBoxNode( std::string convoName, FontManager& fonts );

    virtual void drawCurrent( sf::RenderTarget& target, sf::RenderStates states ) const;
    virtual void updateCurrent( sf::Time dt, CommandQueue& commands );
    bool handleEvent( const sf::Event& event );
    void setConversation( std::string conversationName );
    void transitionDialog( int link );
    bool complete( void ) { return mComplete; }

private:
    sf::Text                                 mMessage;
    sf::RectangleShape                       mBackgroundBox;
    std::vector<sf::Text>                    mResponseDisplay;
    GUI::Container                           mGUIContainer;
    ConversationData                         mConversation;
    std::vector<std::pair<std::string, int>> mAnswers;
    FontManager*                             mFonts;
    bool                                     mComplete;
};

#endif // MESSAGEBOXNODE_HPP