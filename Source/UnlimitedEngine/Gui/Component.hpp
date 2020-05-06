#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <memory>



namespace sf
{
	class Event;
}

namespace GUI
{

class Component : public sf::Drawable, public sf::Transformable, private sf::NonCopyable
{
public:
    typedef std::shared_ptr<Component> Ptr;

    Component();
    virtual	~Component();

    void setIndex( unsigned int index ) { this->mIndex = index; }
    unsigned int getIndex( void ) { return this->mIndex; }

    virtual bool isSelectable() const = 0;
    bool         isSelected() const;
    virtual void select();
    virtual void deselect();

    virtual bool isActive() const;
    virtual void activate();
    virtual void deactivate();
    virtual void handleEvent(const sf::Event& event) = 0;
    virtual bool contains( float x, float y ) = 0;
private:
    bool mIsSelected;
    bool mIsActive;
    unsigned int mIndex;
};

}

#endif // COMPONENT_HPP
