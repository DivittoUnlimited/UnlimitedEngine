
#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "Component.hpp"
#include "Core/ResourceManager.hpp"
#include "Core/ResourceIdentifiers.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <vector>
#include <string>
#include <memory>
#include <functional>


namespace GUI
{

class Button : public Component
{
public:
    typedef std::shared_ptr<Button>		Ptr;
    typedef std::function<void()>		Callback;

    Button( const FontManager& fonts, const TextureManager& textures );

    void	 setCallback( Callback callback );
    void	 setText( const std::string& text );
    void	 setToggle( bool flag );
    virtual bool     contains( float x , float y);

    virtual bool isSelectable( void ) const;
    virtual void select( void );
    virtual void deselect( void );

    virtual void activate( void );
    virtual void deactivate( void );

    virtual void handleEvent( const sf::Event& event );

private:
    virtual void draw( sf::RenderTarget& target, sf::RenderStates states ) const;

    //## Attributes
    Callback           mCallback;
    const sf::Texture& mTexture;
    sf::IntRect        mNormal;
    sf::IntRect        mSelected;
    sf::IntRect        mPressed;
    sf::Sprite         mSprite;
    sf::Text           mText;
    bool               mIsToggle;
};

}

#endif // BUTTON_HPP
