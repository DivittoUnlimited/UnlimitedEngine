#ifndef COLLISIONMAN
#define COLLISIONMAN

#include <map>
#include <set>
#include <vector>
#include <string>
#include <iostream>
#include <assert.h>

#include <SFML/System.hpp>
#include <SFML/Graphics/RectangleShape.hpp> // for debug rect
#include <SFML/Graphics/RenderWindow.hpp>

#include "SceneNode.hpp"
#include "Globals.hpp"

namespace CollisionMan {
    struct Obj {
        SceneNode* id;
        sf::Rect<float> rect;
        //## Constructors
        Obj( void ) : id( nullptr ), rect( sf::Rect<float>( ) ) { }
        Obj( SceneNode* id, sf::Rect<float> boundingRect )
            : id( id ), rect( boundingRect ) { }
    };

    /// \brief LayerDiminsions
    /// simple map to contain the area of all the layers of the grid so that
    /// the engine can set the right sizes without help from the user
    static std::map<std::string, sf::Rect<float>> LayerDiminsions;

    /// \brief The node struct
    /// the node represents a quadrant in the quad tree
    struct Node {
        Node* NE;
        Node* NW;
        Node* SE;
        Node* SW;
        Node* parent;
        std::vector<Obj> localObjs;
        sf::Rect<float> rect;
        sf::RectangleShape debugRect;
        float depth; // depth from the root of the stucture
        bool hasKids; // flag to tell if the node has been split yet or not
        const unsigned int MAX_DEPTH = 5; // the max depth a node can be before the tree ends
        const unsigned int MAX_LOCAL_OBJS = 1; // the max number of objs a node cn carry before it must be split.

        //## Constructor
        Node( Node* parent, float x, float y, float width, float height, float depth );
        ~Node( void );

        //## Interface
        /// \brief addObj
        /// Adds a obj to this node, or it's children if any and if they fit inside there boundries
        ///  \param obj
        bool addObj( Obj obj );

        /// \brief split
        /// Splits the node into 4 new smaller nodes
        /// Method gets called when the node is carrying to many objs
        void split( void );

        void getPairs( std::vector<std::pair<Obj, Obj>>& pairs );
        /// \brief clear
        /// This method is meant to be called by the root node to clear the entire
        /// tree it can of course also be called by any node to clear all objs and
        /// childern before it.
        void clear( void );

        /// \brief draw
        /// Temp Method debug only
        ///  \param mWindow
        ///
        void draw( sf::RenderTarget& target, sf::RenderStates states ) const {
            if( this->hasKids ) {
                NW->draw( target, states );
                NE->draw( target, states );
                SW->draw( target, states );
                SE->draw( target, states );
            }
            else target.draw( debugRect, states );
        }
    };
    /// \brief The QuadTree class
    /// QuadTree designed to make collision detection as fast as possible
    /// by sending only the most likely objs to be colliding is does this
    /// by dividing sections of the screen with two or more objs until  it cant be
    /// divided anymore then passing the objs still in the same section to be checked for
    /// collision against each other
    class QuadTree {
    public:
        QuadTree( float x, float y, float width, float height );
        ~QuadTree( void );

        //## Interface
        std::set<std::pair<SceneNode *, SceneNode*>> update( std::vector<Obj> &objRects );
        void setRootRect( float x, float y, float width, float height )
        {
            root->rect.left = x;
            root->rect.top = y;
            root->rect.width = width;
            root->rect.height = height;
        }

        Node* root;
    private:
        //## Attributes
        Node* current;
    };
    static QuadTree QUAD_TREE( 0, 0, 0, 0 );

    /// \brief update
    static std::set<std::pair<SceneNode*, SceneNode*>> update( SceneNode* layer )
    {
        // build object list from SceneGraph and let quad treee work it's magic.
        std::vector<Obj> objs;
        std::vector<SceneNode*> nodes;
        layer->buildList( nodes );
        for( auto i : nodes )
        {
            objs.push_back( CollisionMan::Obj( i, i->getBoundingRect( ) ) );
        }
        QUAD_TREE.root->clear( );
        return QUAD_TREE.update( objs );
    }
}

#endif // COLLISIONMAN

