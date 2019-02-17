#ifndef CONTAINER_HPP
#define CONTAINER_HPP

#include "Component.hpp"

#include <vector>
#include <memory>

namespace GUI
{

class Container : public Component
{
public:
    typedef std::shared_ptr<Container> Ptr;

    Container( void );

    void pack(Component::Ptr component);

    virtual bool isSelectable( void ) const;
    virtual void handleEvent( const sf::Event& event );
    bool hasSelection( void ) const;
    void select( std::size_t index );
    void selectNext( void );
    void selectPrevious( void );
    void clear( void );

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    //## Attributes
    std::vector<Component::Ptr>	mChildren;
    int							mSelectedChild;
};

}

#endif // BOOK_CONTAINER_HPP
