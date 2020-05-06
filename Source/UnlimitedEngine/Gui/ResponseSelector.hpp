#ifndef RESPONSESELECTOR_HPP
#define RESPONSESELECTOR_HPP

#include <vector>
#include <string>
#include <memory>
#include <functional>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "Core/ResourceManager.hpp"
#include "Core/ResourceIdentifiers.hpp"
#include "Gui/Component.hpp"

class MessageBoxNode;

namespace GUI
{

class ResponseSelector : public Component
{
public:
    typedef std::shared_ptr<ResponseSelector>    Ptr;
    typedef std::function<void( )>               Callback;

    ResponseSelector( std::string text, const FontManager& fonts , MessageBoxNode *owner );

    void setCallback( Callback callback );
    void setToggle( bool flag );

    virtual bool isSelectable( void ) const;
    virtual void select( void );
    virtual void deselect( void );

    virtual void activate( void );
    virtual void deactivate( void );

    virtual void handleEvent( const sf::Event& event );
    virtual bool     contains( float x , float y);

private:
    virtual void draw( sf::RenderTarget& target, sf::RenderStates states ) const;

    //## Attributes
    Callback           mCallback;
    sf::Color          mNormal;
    sf::Color          mSelected;
    sf::Color          mPressed;
    sf::Text           mText;
    bool               mIsToggle;
    sf::RectangleShape mSprite;
    MessageBoxNode*    mOwner;
};

}

#endif // RESPONSESELECTOR_HPP
