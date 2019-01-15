#ifndef TILEDMANAGER_HPP
#define TILEDMANAGER_HPP

#include <string>
#include <vector>
#include <map>

#include <SFML/System/Vector2.hpp>

#include "Core/Entity.hpp"

#include <iostream>

extern "C" {
    #include "lua/lua.h"
    #include "lua/lualib.h"
    #include "lua/lauxlib.h"
}


namespace Tiled {

// Some Data Types to make things easier
struct TileSet
{
    std::string name;
    unsigned int firstGid;
    unsigned int tileWidth;
    unsigned int tileHeight;
    unsigned int spacing;
    unsigned int margin;
    unsigned int columns;
    std::string filePath;
    unsigned int imageWidth;
    unsigned int imageHeight;
    float offsetX;
    float offsetY;
    std::string gridOrientation;
    unsigned int gridWidth;
    unsigned int  gridHeight;
    std::map<std::string, std::string> properties;
    unsigned int tileCount;
};

struct Object
{
    unsigned int id;
    std::string name;
    std::string type;
    std::string shape;
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;
    unsigned int rotation;
    unsigned int gid;
    bool visible;
};

struct Layer
{
    std::string name;
    std::string type;
    std::string filePath;
    unsigned int id;
    bool visible;
    unsigned int opacity;
    unsigned int offsetX;
    unsigned int offsetY;

    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;

    std::string drawOrder;
    std::vector<Object> objects;

    std::vector<unsigned int> data; // refers to data array in tile layers
    std::map<std::string, std::string> properties;
};

struct TiledMap
{
    TiledMap( void )
        : orientation( "orthagonal" )
        , renderOrder( "top-down" )
        , width( 0 )
        , height( 0 )
        , tileWidth( 0 )
        , tileHeight( 0 )
        , nextLayerId( 0 )
        , properties( std::map<std::string, std::string>( ) )
        , tileSets( std::vector<TileSet>( ) )
        , layers( std::vector<Layer>( ) )
    {
    }

    std::string orientation;
    std::string renderOrder;
    unsigned int width;
    unsigned int height;
    unsigned int tileWidth;
    unsigned int tileHeight;
    unsigned int nextLayerId;
    unsigned int nextObjectId;
    std::map<std::string, std::string> properties;
    std::vector<TileSet> tileSets;
    std::vector<Layer> layers;
};

static TiledMap loadFromFile( std::string filePath )
{
    std::cout << "Loading Map: " << filePath << std::endl;
    Tiled::TiledMap currentTileMap;
    // open lua file at path or throw exception
    lua_State* L = luaL_newstate( );
    luaL_openlibs( L );
    lua_getglobal( L, "debug" );
    lua_getfield( L, -1, "traceback" );
    lua_replace( L, -2 );
    luaL_loadfile( L, filePath.c_str( ) );
    if ( lua_pcall( L, 0, LUA_MULTRET, -2 ) ) {
        luaL_traceback( L, L, lua_tostring( L, -1 ), 1 );
        std::cout << "ERROR: " << lua_tostring( L, -1 ) << std::endl;
        throw( lua_tostring( L, -1 ) );
    }

    if( lua_istable( L, -1 ) )
    {
        // get basic Tiled Map info
        lua_getfield( L, -1, "orientation" );
        if( lua_isstring( L, -1 ) ) currentTileMap.orientation = lua_tostring( L, -1 );
        else throw( "Error: TiledMap Orientation invalid" );
        lua_pop( L, 1 ); // Orientaion value

        lua_getfield( L, -1, "renderorder" );
        if( lua_isstring( L, -1 ) ) currentTileMap.renderOrder = lua_tostring( L, -1 );
        else throw( "Error: TiledMap RenderOrder invalid" );
        lua_pop( L, 1 ); // RenderOrder value

        lua_getfield( L, -1, "width" );
        if( lua_isnumber( L, -1 ) ) currentTileMap.width = (unsigned int)lua_tonumber( L, -1 );
        else throw( "Error: TiledMap width invalid" );
        lua_pop( L, 1 ); // width value

        lua_getfield( L, -1, "height" );
        if( lua_isnumber( L, -1 ) ) currentTileMap.height = (unsigned int)lua_tonumber( L, -1 );
        else throw( "Error: TiledMap height invalid" );
        lua_pop( L, 1 ); // height value

        lua_getfield( L, -1, "nextlayerid" );
        if( lua_isnumber( L, -1 ) ) currentTileMap.nextLayerId = (unsigned int)lua_tonumber( L, -1 );
        else throw( "Error: TiledMap nextLayerId invalid" );
        lua_pop( L, 1 ); // nextLayerID

        lua_getfield( L, -1, "nextobjectid" );
        if( lua_isnumber( L, -1 ) ) currentTileMap.nextObjectId = (unsigned int)lua_tonumber( L, -1 );
        else throw( "Error: TiledMap nextObjectId invalid" );
        lua_pop( L, 1 ); // nextObjId value

        // load custom map properties here as string string map.

        // load tile sets
        lua_getfield( L, -1, "tilesets" );
        if( lua_istable( L, -1 ) )
        {
            lua_pushnil( L );  // first anon table representing a tileSet
            while( lua_next( L, -2 ) != 0 )
            {
                Tiled::TileSet currentTileSet;

                lua_getfield( L, -1, "name" );
                if( lua_isstring( L, -1 ) ) currentTileSet.name = lua_tostring( L, -1 );
                else throw( "Error: TileSet name invalid" );
                lua_pop( L, 1 ); // name value

                lua_getfield( L, -1, "firstgid" );
                if( lua_isnumber( L, -1 ) ) currentTileSet.firstGid = (unsigned int)lua_tonumber( L, -1 );
                else throw( "Error: TileSet firstGid invalid" );
                lua_pop( L, 1 ); // firstGid value

                lua_getfield( L, -1, "tilewidth" );
                if( lua_isnumber( L, -1 ) ) currentTileSet.tileWidth = (unsigned int)lua_tonumber( L, -1 );
                else throw( "Error: TileSet tileWidth invalid" );
                lua_pop( L, 1 ); // tileWidth value

                lua_getfield( L, -1, "tileheight" );
                if( lua_isnumber( L, -1 ) ) currentTileSet.tileHeight = (unsigned int)lua_tonumber( L, -1 );
                else throw( "Error: TiledSet tileHeight invalid" );
                lua_pop( L, 1 ); // tileHeight value

                lua_getfield( L, -1, "spacing" );
                if( lua_isnumber( L, -1 ) ) currentTileSet.spacing = (unsigned int)lua_tonumber( L, -1 );
                else throw( "Error: TiledSet spacing invalid" );
                lua_pop( L, 1 ); // spacing value

                lua_getfield( L, -1, "margin" );
                if( lua_isnumber( L, -1 ) ) currentTileSet.margin = (unsigned int)lua_tonumber( L, -1 );
                else throw( "Error: TileSet margin invalid" );
                lua_pop( L, 1 ); // margin value

                lua_getfield( L, -1, "columns" );
                if( lua_isnumber( L, -1 ) ) currentTileSet.columns = (unsigned int)lua_tonumber( L, -1 );
                else throw( "Error: TileSet columns invalid" );
                lua_pop( L, 1 ); // firstGid value

                lua_getfield( L, -1, "image" );
                if( lua_isstring( L, -1 ) ) currentTileSet.filePath = lua_tostring( L, -1 );
                else throw( "Error: TileSet filePath invalid" );
                lua_pop( L, 1 ); // name value

                lua_getfield( L, -1, "imagewidth" );
                if( lua_isnumber( L, -1 ) ) currentTileSet.imageWidth = (unsigned int)lua_tonumber( L, -1 );
                else throw( "Error: TileSet imageWidth invalid" );
                lua_pop( L, 1 ); // imageWidth value

                lua_getfield( L, -1, "imageheight" );
                if( lua_isnumber( L, -1 ) ) currentTileSet.imageHeight = (unsigned int)lua_tonumber( L, -1 );
                else throw( "Error: TileSet imageHeigt invalid" );
                lua_pop( L, 1 ); // imageHeight value

                lua_getfield( L,  -1, "tileoffset" );
                if(  lua_istable( L, -1 ) )
                {
                    lua_getfield( L, -1, "x" );
                    if( lua_isnumber( L, -1 ) ) currentTileSet.offsetX = (unsigned int)lua_tonumber( L, -1 );
                    else throw( "Error: TileSet tileOffsetX invalid" );
                    lua_pop( L, 1 ); // tileOffset.x value

                    lua_getfield( L, -1, "y" );
                    if( lua_isnumber( L, -1 ) ) currentTileSet.offsetY = (unsigned int)lua_tonumber( L, -1 );
                    else throw( "Error: TileSet tileOffsetY invalid" );
                    lua_pop( L, 1 ); // tileOffset.y value
                }
                else
                    std::cout << "Error reading tileoffset table: " << filePath << std::endl;
                lua_pop( L, 1 ); // tileoffset

                lua_getfield( L,  -1, "grid" );
                if(  lua_istable( L, -1 ) )
                {
                    lua_getfield( L, -1, "orientation" );
                    if( lua_isstring( L, -1 ) ) currentTileSet.gridOrientation = lua_tostring( L, -1 );
                    else throw( "Error: TiledMap Orientation invalid" );
                    lua_pop( L, 1 ); // Orientaion value

                    lua_getfield( L, -1, "width" );
                    if( lua_isnumber( L, -1 ) ) currentTileSet.gridWidth = (unsigned int)lua_tonumber( L, -1 );
                    else throw( "Error: TileSet grid.width invalid" );
                    lua_pop( L, 1 ); // grid.width value

                    lua_getfield( L, -1, "height" );
                    if( lua_isnumber( L, -1 ) ) currentTileSet.gridHeight = (unsigned int)lua_tonumber( L, -1 );
                    else throw( "Error: TileSet grid.height invalid" );
                    lua_pop( L, 1 ); // grid.height value
                }
                else
                    std::cout << "Error reading tileoffset table: " << filePath << std::endl;
                lua_pop( L, 1 ); // grid

                // properties table

                // terrains??? youtube it!!!!!!!!!!!

                // tileCount
                lua_getfield( L, -1, "tilecount" );
                if( lua_isnumber( L, -1 ) ) currentTileSet.tileCount = (unsigned int)lua_tonumber( L, -1 );
                else throw( "Error: TileSet tileCount invalid" );
                lua_pop( L, 1 ); // tileCount value

                // tiles table

                currentTileMap.tileSets.push_back( currentTileSet );
                lua_pop( L, 1); // removes anon tileset table; keeps 'key' for next iteration
            }
           // lua_pop( L, 1 ); // remove anon tileSet
        }
        else
            std::cout << "Tiled Map failed to load a TileSet: " << filePath << std::endl;
        lua_pop( L, 1 ); // tileSets

        // develop layers
        if( lua_istable( L, -1 ) )
        {
            lua_getfield( L, -1, "layers" );
            if( lua_istable( L, -1 ) )
            {
                lua_pushnil( L );  // first key
                while( lua_next( L, -2 ) != 0 )
                {
                    std::string type = "";
                    lua_getfield( L, -1, "type" );
                    if( lua_isstring( L, -1 ) ) type  = lua_tostring( L, -1 );
                    else type = "TILED_LAYER_READ_ERROR";
                    lua_pop( L, 1 ); // type value

                    if( type == "tilelayer" )
                    {
                        Tiled::Layer currentLayer;
                        // Add tileLayer specific data here
                        currentLayer.type = type;

                        lua_getfield( L, -1, "id" );
                        if( lua_isnumber( L, -1 ) ) currentLayer.id = (unsigned int)lua_tonumber( L, -1 );
                        else throw( "Error: Layer id invalid" );
                        lua_pop( L, 1 ); // id value

                        lua_getfield( L, -1, "name" );
                        if( lua_isstring( L, -1 ) ) currentLayer.name = lua_tostring( L, -1 );
                        else throw( "Error: Layer name invalid" );
                        lua_pop( L, 1 ); // name value

                        lua_getfield( L, -1, "x" );
                        if( lua_isnumber( L, -1 ) ) currentLayer.x = (unsigned int)lua_tonumber( L, -1 );
                        else throw( "Error: Layer x invalid" );
                        lua_pop( L, 1 ); // x value

                        lua_getfield( L, -1, "y" );
                        if( lua_isnumber( L, -1 ) ) currentLayer.y = (unsigned int)lua_tonumber( L, -1 );
                        else throw( "Error: y invalid" );
                        lua_pop( L, 1 ); // y value

                        lua_getfield( L, -1, "width" );
                        if( lua_isnumber( L, -1 ) ) currentLayer.width = (unsigned int)lua_tonumber( L, -1 );
                        else throw( "Error: Layer width invalid" );
                        lua_pop( L, 1 ); // x value

                        lua_getfield( L, -1, "height" );
                        if( lua_isnumber( L, -1 ) ) currentLayer.height = (unsigned int)lua_tonumber( L, -1 );
                        else throw( "Error: height invalid" );
                        lua_pop( L, 1 ); // height value

                        lua_getfield( L, -1, "visible" );
                        if( lua_isboolean( L, -1 ) ) currentLayer.visible = lua_toboolean( L, -1 );
                        else throw( "Error: Layer visible invalid" );
                        lua_pop( L, 1 ); // visible value

                        lua_getfield( L, -1, "opacity" );
                        if( lua_isnumber( L, -1 ) ) currentLayer.opacity = (unsigned int)lua_tonumber( L, -1 );
                        else throw( "Error: Layer Opacity invalid" );
                        lua_pop( L, 1 ); // Opacity value

                        lua_getfield( L, -1, "offsetx" );
                        if( lua_isnumber( L, -1 ) ) currentLayer.offsetX = (unsigned int)lua_tonumber( L, -1 );
                        else throw( "Error: Layer offsetx invalid" );
                        lua_pop( L, 1 ); // offsetx value

                        lua_getfield( L, -1, "offsety" );
                        if( lua_isnumber( L, -1 ) ) currentLayer.offsetY = (unsigned int)lua_tonumber( L, -1 );
                        else throw( "Error: offsety invalid" );
                        lua_pop( L, 1 ); // offsety value

                        // data 2d array
                        lua_getfield( L, -1, "data" );
                        if( lua_istable( L, -1 ) )
                        {
                            lua_pushnil( L );
                            for( unsigned int i = 0; i < currentLayer.height; ++i )
                            {
                                for( unsigned int j = 0; j < currentLayer.width; ++j )
                                {
                                    lua_next( L, -2 );
                                    currentLayer.data.push_back( (unsigned int)lua_tonumber( L , -1 ) );
                                    lua_pop( L, 1 );
                                }
                            }
                        }
                        else
                            std::cout << "ERROR Tiled Map tilelayer data invalid" << std::endl;
                        lua_pop( L, 1 ); // data array
                        currentTileMap.layers.push_back( currentLayer );
                        lua_pop( L, 1 );
                    }
                    else if( type == "objectgroup" )
                    {
                        Layer currentLayer = Layer();
                        currentLayer.type = type;

                        // Add object layer specific data here
                        lua_getfield( L, -1, "id" );
                        if( lua_isnumber( L, -1 ) ) currentLayer.id = (unsigned int)lua_tonumber( L, -1 );
                        else throw( "Error: ObjectLayer id invalid" );
                        lua_pop( L, 1 ); // id value

                        lua_getfield( L, -1, "name" );
                        if( lua_isstring( L, -1 ) ) currentLayer.name = lua_tostring( L, -1 );
                        else throw( "Error: ObjectLayer name invalid" );
                        lua_pop( L, 1 ); // name value

                        lua_getfield( L, -1, "visible" );
                        if( lua_isboolean( L, -1 ) ) currentLayer.visible = lua_toboolean( L, -1 );
                        else throw( "Error: ObjectLayer visible invalid" );
                        lua_pop( L, 1 ); // visible value

                        lua_getfield( L, -1, "opacity" );
                        if( lua_isnumber( L, -1 ) ) currentLayer.opacity = (unsigned int)lua_tonumber( L, -1 );
                        else throw( "Error: ObjectLayer Opacity invalid" );
                        lua_pop( L, 1 ); // Opacity value

                        lua_getfield( L, -1, "offsetx" );
                        if( lua_isnumber( L, -1 ) ) currentLayer.offsetX = (unsigned int)lua_tonumber( L, -1 );
                        else throw( "Error: ObjectLayer offsetx invalid" );
                        lua_pop( L, 1 ); // offsetx value

                        lua_getfield( L, -1, "offsety" );
                        if( lua_isnumber( L, -1 ) ) currentLayer.offsetY = (unsigned int)lua_tonumber( L, -1 );
                        else throw( "Error: ObjectLayer offsety invalid" );
                        lua_pop( L, 1 ); // offsety value

                        lua_getfield( L, -1, "draworder" );
                        if( lua_isstring( L, -1 ) ) currentLayer.drawOrder = lua_tostring( L, -1 );
                        else throw( "Error: ObjectLayer draworder invalid" );
                        lua_pop( L, 1 ); // draworder value

                        // Properties table never loaded, not needed.

                        // Objects list
                        lua_getfield( L, -1, "objects" );
                        if( lua_istable( L, -1 ) ) // object table
                        {
                            currentLayer.objects = std::vector<Object>( );
                            lua_pushnil( L );
                            while( lua_next( L, -2 ) != 0 ) {
                                if( lua_istable( L, -1 ) )
                                {
                                    Object currentObj;

                                    lua_getfield( L, -1, "type" );
                                    if( lua_isstring( L, -1 ) ) currentObj.type = lua_tostring( L, -1 );
                                    else throw( "Error: Object type invalid" );
                                    lua_pop( L, 1 ); // Object type value

                                    lua_getfield( L, -1, "id" );
                                    if( lua_isnumber( L, -1 ) ) currentObj.id = (unsigned int)lua_tonumber( L, -1 );
                                    else throw( "Error: Object id invalid" );
                                    lua_pop( L, 1 ); // id value

                                    lua_getfield( L, -1, "name" );
                                    if( lua_isstring( L, -1 ) ) currentObj.name = lua_tostring( L, -1 );
                                    else throw( "Error: Object name invalid" );
                                    lua_pop( L, 1 ); // name value

                                    lua_getfield( L, -1, "shape" );
                                    if( lua_isstring( L, -1 ) ) currentObj.shape = lua_tostring( L, -1 );
                                    else throw( "Error: Object shape invalid" );
                                    lua_pop( L, 1 ); // shape value

                                    lua_getfield( L, -1, "x" );
                                    if( lua_isnumber( L, -1 ) ) currentObj.x = (unsigned int)lua_tonumber( L, -1 );
                                    else throw( "Error: Object x invalid" );
                                    lua_pop( L, 1 ); // x value

                                    lua_getfield( L, -1, "y" );
                                    if( lua_isnumber( L, -1 ) ) currentObj.y = (unsigned int)lua_tonumber( L, -1 );
                                    else throw( "Error: Object y invalid" );
                                    lua_pop( L, 1 ); // y value

                                    lua_getfield( L, -1, "width" );
                                    if( lua_isnumber( L, -1 ) ) currentObj.width = (unsigned int)lua_tonumber( L, -1 );
                                    else throw( "Error: Object width invalid" );
                                    lua_pop( L, 1 ); // x value

                                    lua_getfield( L, -1, "height" );
                                    if( lua_isnumber( L, -1 ) ) currentObj.height = (unsigned int)lua_tonumber( L, -1 );
                                    else throw( "Error: Object height invalid" );
                                    lua_pop( L, 1 ); // height value

                                    lua_getfield( L, -1, "gid" );
                                    if( lua_isnumber( L, -1 ) ) currentObj.gid = (unsigned int)lua_tonumber( L, -1 );
                                    else currentObj.gid = 0;
                                    lua_pop( L, 1 ); // gid value POSSIBLE TO NOT HAVE ONE THEREFOR NO THROW!

                                    lua_getfield( L, -1, "rotation" );
                                    if( lua_isnumber( L, -1 ) ) currentObj.rotation = (unsigned int)lua_tonumber( L, -1 );
                                    else throw( "Error: Object rotation invalid" );
                                    lua_pop( L, 1 ); // rotation value

                                    lua_getfield( L, -1, "visible" );
                                    if( lua_isboolean( L, -1 ) ) currentObj.visible = lua_toboolean( L, -1 );
                                    else throw( "Error: Object visible invalid" );
                                    lua_pop( L, 1 ); // visible value
  /*
                                    lua_getfield( L, -1, "properties" );
                                    if( lua_istable( L, -1 ) )
                                    {

                                    }
                                    else throw( "Error: Object Properties invalid" );
                                    lua_pop( L, 1 ); // Properties table
*/
                                    currentLayer.objects.push_back( currentObj );
                                }
                                else
                                    std::cout << "ERROR reading Objects list from Tiled Map " << filePath << std::endl;
                                lua_pop( L, 1); // object
                            }
                        }
                        else
                            std::cout << "Tiled Map object Layer object list ERROR" << std::endl;

                        lua_pop( L, 1 ); // remove objects table

                        currentTileMap.layers.push_back( currentLayer );
                       // lua_pop( L, 1 );

                    }

                    else if( type == "imagelayer" )
                    {
                        Tiled::Layer currentLayer = Tiled::Layer( );
                        // Add Image layer specific data
                        currentLayer.type = type;

                        lua_getfield( L, -1, "type" );
                        if( lua_isstring( L, -1 ) ) currentLayer.type = lua_tostring( L, -1 );
                        else throw( "Error: imageLayer type invalid" );
                        lua_pop( L, 1 ); // Object type value

                        lua_getfield( L, -1, "id" );
                        if( lua_isnumber( L, -1 ) ) currentLayer.id = (unsigned int)lua_tonumber( L, -1 );
                        else throw( "Error: imageLayer id invalid" );
                        lua_pop( L, 1 ); // id value

                        lua_getfield( L, -1, "name" );
                        if( lua_isstring( L, -1 ) ) currentLayer.name = lua_tostring( L, -1 );
                        else throw( "Error: imageLayer name invalid" );
                        lua_pop( L, 1 ); // name value

                        lua_getfield( L, -1, "visible" );
                        if( lua_isboolean( L, -1 ) ) currentLayer.visible = lua_toboolean( L, -1 );
                        else throw( "Error: imageLayer visible invalid" );
                        lua_pop( L, 1 ); // visible value

                        lua_getfield( L, -1, "opacity" );
                        if( lua_isnumber( L, -1 ) ) currentLayer.opacity = (unsigned int)lua_tonumber( L, -1 );
                        else throw( "Error: imageLayer Opacity invalid" );
                        lua_pop( L, 1 ); // Opacity value

                        lua_getfield( L, -1, "offsetx" );
                        if( lua_isnumber( L, -1 ) ) currentLayer.offsetX = (unsigned int)lua_tonumber( L, -1 );
                        else throw( "Error: imageLayer offsetx invalid" );
                        lua_pop( L, 1 ); // offsetx value

                        lua_getfield( L, -1, "offsety" );
                        if( lua_isnumber( L, -1 ) ) currentLayer.offsetY = (unsigned int)lua_tonumber( L, -1 );
                        else throw( "Error: imageLayer offsety invalid" );
                        lua_pop( L, 1 ); // offsety value

                        lua_getfield( L, -1, "image" ); // filepath to the image used
                        if( lua_isstring( L, -1 ) ) currentLayer.filePath = lua_tostring( L, -1 );
                        else throw( "Error: imageLayer image invalid" );
                        lua_pop( L, 1 ); // image value

                        currentTileMap.layers.push_back( currentLayer );
                    }
                    else
                        std::cout << "Error reading layer in " << filePath << std::endl;
                    lua_pop( L, 1 ); // removes anon 'layer'; keeps 'key' for next iteration
                }
            }
            else
                std::cout << "Tiled Map failed to load a Layer: " << filePath << std::endl;
            lua_pop( L, 1 ); // layers
        }
        else
            std::cout << "Error loading layer information from " << filePath << std::endl;
        lua_pop( L, 1 );
    }
    else
        std::cout << "Error reading Tiled level @" << filePath << std::endl;
    return currentTileMap;
}


}

#endif // TILEDMANAGER_HPP
