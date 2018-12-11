# usr/bin/lua
register_collision();
--local Sprite = require( "Sprite" );
local Texture = require( "Texture" );
local Rectangle = require( "Rectangle" );
local MapMan = {}

function MapMan:loadMap( map )
	-- generate texture rects for all possible tiles being loaded 
        local World = Clone( require( "World" ) );
        World:create( );

        local counter = 1;
	local tileSet = {};
	tileSet[0] = { texId = "NONE", rect = { x = 0; y = 0; width = 32; height = 32; } };
	for k, v in pairs( map.tilesets ) do 	
            -- load the texture for the current tileSet
            World.textures[v.name] = Clone( Texture );
            World.textures[v.name]:create( v.name, v.image );
            -- build rectangles that will cover the area on the texture that is the tile ( divide texture into tiles )
            local y = 0;
            while y < v.imageheight - 1 do
                local x = 0;
                while x < v.imagewidth - 1 do
                        tileSet[counter] = { texId = v.name; rect = { x = x; y = y; width = v.tilewidth; height = v.tileheight; }; };
                        counter = counter + 1;
                        x = x + v.tilewidth;
                end
                y = y + v.tileheight;
            end
        end
	-- create and set up tile layers using texture rects
	for k, v in pairs( map.layers ) do
           World.layers[v.name] = addLayer(  ); -- add a layer to the engine
           if v.type == "tilelayer" then
                local x = 0;
                local y = 0;
                -- iterate through all tiles in the layer and create them in the engine with the correct tile as created above
                for i, j in pairs( v.data ) do
                        if y < (v.height * tileSet[j].rect.height) - 1 then
                                if j ~= 0 then
                                    World.tiles[i] = Clone( Rectangle );
                                    World.tiles[i]:create( ( v.name .. x .. y), World.layers[v.name], x, y, tileSet[j].rect.width, tileSet[j].rect.height );

                                    -- add special inf about tiles here i.e terrain info
                                    -- could be done by running properties loop such as below with obj!!!
						
						
                                    shape_setTexture( World.tiles[i].id, tileSet[j].texId, true );
                                    shape_setTexRect( World.tiles[i].id, tileSet[j].rect.x, tileSet[j].rect.y, tileSet[j].rect.width, tileSet[j].rect.height );
                                end
                                if x < (v.width * tileSet[j].rect.width) - tileSet[j].rect.width then -- the minus 32 is to subtract the tilewidth not sure why but it fixes a problem with tiles being skewed
                                        x = x + tileSet[j].rect.width;
                                else
                                        x = 0;
                                        y = y + tileSet[j].rect.height;
                                end
                        end
                end
                -- buld up the object layer
		elseif v.type == "objectgroup" then
			-- cycle through objs in the layer building there base stats
			-- objects are handled much diferently then regular tiles 
                        create_collisionGrid( World.layers[v.name], -100, -100, (map.width * map.tilewidth) + 200, (map.height * map.tileheight) + 200 )
                        for i, j in pairs( v.objects ) do
                                local obj = Clone( require( j.properties.fileName ) );
                                obj.layer = World.layers[v.name];
				obj.x = j.x;
				obj.y = j.y;
				obj.height = j.height;
				obj.width = j.width;
				for k, v in pairs( j.properties ) do
					obj[k] = v;
				end
                                World.objs[obj.id] = obj;
			end	
		end
	end
        log( "normal", "MapMan Complete" );
        return World;
end

return MapMan;
