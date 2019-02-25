#include "CollisionMan.hpp"

// ================================================================================
// ================================================================================
/// QuadTree
CollisionMan::Node::Node( Node* parent, float x, float y, float width, float height, float depth )
    : parent( parent ), rect( x, y, width, height ), depth( depth ) {
    NW = nullptr;
    NE = nullptr;
    SW = nullptr;
    SE = nullptr;
    this->hasKids = false;
    debugRect = sf::RectangleShape( sf::Vector2f( width, height ) );
    debugRect.setPosition( sf::Vector2f( x, y ) );
    debugRect.setOutlineThickness( 1 );
    debugRect.setFillColor( sf::Color( 0,0,0,0 ) );
    debugRect.setOutlineColor( sf::Color( 0, 0, 255, 255 ) );
}
CollisionMan::Node::~Node( void ) {  }
//## Interface

/// \brief addObj
/// Adds a obj to this node, or it's children if any and if they fit inside there boundries
///  \param obj
/// \return
bool CollisionMan::Node::Node::addObj( Obj obj ) {
    if( this->rect.contains( obj.rect.left, obj.rect.top ) &&
            this->rect.contains( obj.rect.left + obj.rect.width, obj.rect.top + obj.rect.height ) ) {
        if( this->depth <= this->MAX_DEPTH ) {
            if( this->hasKids ) {
                if     ( NW->addObj( obj ) ) { return true; }
                else if( NE->addObj( obj ) ) { return true; }
                else if( SW->addObj( obj ) ) { return true; }
                else if( SE->addObj( obj ) ) { return true; }
                else{ this->localObjs.push_back( obj ); return true; } // obj fits in node but not in kids so leave here
            } else {  //  no kids add to node and attempt to split node if needed
                this->localObjs.push_back( obj );
                if( this->localObjs.size() > this->MAX_LOCAL_OBJS ) { this->split( ); }
                return true;
            }
        }else{
            this->localObjs.push_back( obj );
            return true;
        }
    }
    return false;
}
/// \brief split
/// Splits the node into 4 new smaller nodes
/// Method gets called when the node is carrying to many objs
void CollisionMan::Node::split( void ) {
    NW = new Node( this, rect.left, rect.top, rect.width/2, rect.height/2, this->depth + 1 );
    NE = new Node( this, rect.left + rect.width/2, rect.top, rect.width/2, rect.height/2, this->depth + 1 );
    SW = new Node( this, rect.left, rect.top + rect.height/2, rect.width/2, rect.height/2, this->depth + 1 );
    SE = new Node( this, rect.left + rect.width/2, rect.top + rect.height/2, rect.width/2, rect.height/2, this->depth + 1 );

    std::vector<Obj> tempList = std::vector<Obj>();
    int i = localObjs.size() - 1;
    while( i > -1 ) {
        if     ( NW->addObj( localObjs[i] ) ) { localObjs.pop_back( ); }
        else if( NE->addObj( localObjs[i] ) ) { localObjs.pop_back( ); }
        else if( SW->addObj( localObjs[i] ) ) { localObjs.pop_back( ); }
        else if( SE->addObj( localObjs[i] ) ) { localObjs.pop_back( ); }
        else { tempList.push_back( localObjs[i] );localObjs.pop_back( ); }
        i = i - 1;
    }
    this->hasKids = true;
    this->localObjs = tempList;
    // Debugging info
    /*
    std::cout << "$$$$ Parent: ( X: " << this->rect.left << ",Y: " << this->rect.top << ",W: " << this->rect.width << ",H: " << this->rect.height << ",D: " << this->depth << " )" << "Objs: " << this->localObjs.size( ) << std::endl;
    std::cout << " NW ( X: " << NW->rect.left << ",Y: " << NW->rect.top << ",W: " << NW->rect.width << ",H: " << NW->rect.height << ",D: " << NW->depth << " ) Objs: " << NW->localObjs.size( ) << "  ";
    std::cout << " NE ( X: " << NE->rect.left << ",Y: " << NE->rect.top << ",W: " << NE->rect.width << ",H: " << NE->rect.height << ",D: " << NE->depth << " ) Objs: " << NE->localObjs.size( ) << std::endl;
    std::cout << " SW ( X: " << SW->rect.left << ",Y: " << SW->rect.top << ",W: " << SW->rect.width << ",H: " << SW->rect.height << ",D: " << SW->depth << " ) Objs: " << SW->localObjs.size( ) << "  ";
    std::cout << " SE ( X: " << SE->rect.left << ",Y: " << SE->rect.top << ",W: " << SE->rect.width << ",H: " << SE->rect.height << ",D: " << SE->depth << " ) Objs: " << SE->localObjs.size( ) << std::endl;
    */
}
void CollisionMan::Node::getPairs( std::vector<std::pair<Obj, Obj>>& pairs ) {
    if( hasKids ) {
        NW->getPairs( pairs );
        NE->getPairs( pairs );
        SW->getPairs( pairs );
        SE->getPairs( pairs );
    }
    if( this->localObjs.size( ) > 0 ) { // if there are any objs in this node to worry about
        // get all objs from this nodes parent and it's parent's parents
        if( parent != nullptr ) {
            Node* temp = parent;
            std::vector<Obj> objs;
            while( temp != nullptr ) {
                for( auto i : temp->localObjs ) { objs.push_back( i ); }
                temp = temp->parent;
            }
            if( objs.size() > 0 )
                for( auto i : objs ) for( auto j : localObjs )
                    if( i.id != j.id ) pairs.push_back( std::pair<Obj, Obj>( i, j ) );
        }
        // add all pos collisions with the objs in this node
        for( auto i : localObjs )
            for( auto j : localObjs ) if( i.id != j.id ) pairs.push_back( std::pair<Obj, Obj>( i, j ) );
    }
}
///
/// \brief clear
/// This method is meant to be called by the root node to clear the entire
/// tree it can of course also be called by any node to clear all objs and
/// childern before it.
void CollisionMan::Node::clear( void ) {
    if( hasKids ) { // call clear on all children
        NW->clear( ); NE->clear( ); SW->clear( ); SE->clear( );
        NW = nullptr; NE = nullptr; SW = nullptr; SE = nullptr;
    }
    // clear objs
    while( !localObjs.empty() ) localObjs.pop_back( );
    // clear self
    parent = nullptr;
    hasKids = false;
}
// ===========================================================================
// ===========================================================================
/// QuadTree

///

/// Bust out the good book and learn smart pointers for all these nodss this is cuaseing a memory leak!

///


//## Constructor
CollisionMan::QuadTree::QuadTree( float x, float y, float width, float height ) {
    current = root = new Node( nullptr, x, y, width, height, 0 ); // 0 is the depth of the root node becuase it is at the top of the structure
}
CollisionMan::QuadTree::~QuadTree( void ) {  }

//## Interface

/// \brief CisionMan::QuadTree::update
/// Method returns pairs of ids matching all objs that are colliding with each other
///  \return std::vector<std::pair<std::string, std::string>>
std::set<std::pair<SceneNode*, SceneNode*>> CollisionMan::QuadTree::update( std::vector<Obj> &objRects ) {
    unsigned int size = objRects.size( );
    // build the tree structure with this frame's obj list
    while( size > 0 )
    {
        // if there are objs in the list then they need to be added to tree
        if ( !root->addObj( objRects[size - 1] ) ) {
            //std::cout << "ERROR OBJ FAILED TO BE ADDED TO QUADTREE " << objRects.at( size - 1 ).id << " (" << objRects.at( size - 1 ).rect.left << "," << objRects.at( size - 1 ).rect.top << ")" << std::endl;
            //std::cout << "Grid area: (" << root->rect.left << "," << root->rect.top << "," << root->rect.width << "," << root->rect.height << ")" << std::endl;
        }
        objRects.pop_back( );
        size = objRects.size( );
    }
    // traverse tree to get all pos Col pairs
    std::vector<std::pair<CollisionMan::Obj, CollisionMan::Obj>> pairs;
    root->getPairs( pairs );

    // current list could have duplicates or reciprical pairs that need to be removed
    std::vector<std::pair<CollisionMan::Obj, CollisionMan::Obj>> finalPairs;

    while( pairs.size() > 0 )
    {
        bool isDuplicate = false;
        std::pair<CollisionMan::Obj, CollisionMan::Obj> query = pairs.at( pairs.size() - 1 );
        pairs.pop_back( );
        for( auto i : finalPairs ) {  // ensures this collision pair hasn't already been checked in a diferent order.
             if( ( query.first.id == i.first.id && query.second.id == i.second.id )
                        || ( query.second.id == i.first.id && query.first.id == i.second.id ) ) {
                 isDuplicate = true;
                 break;
            }
        }
        if( !isDuplicate && query.first.rect.intersects( query.second.rect ) ) {
            //std::cout << "Colliding Pairs: " << query.first.id << ", " << query.second.id << std::endl;
            finalPairs.push_back( std::pair<CollisionMan::Obj, CollisionMan::Obj>( query.first, query.second ) );
        }
    }

    // translate finalPairs into sceneNodes
    std::set<std::pair<SceneNode*, SceneNode*>> nodes;
    for( auto i : finalPairs )
    {
        nodes.insert( std::pair<SceneNode*, SceneNode*>( i.first.id, i.second.id ) );
    }
    return nodes;
}

