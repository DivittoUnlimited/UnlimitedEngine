return {
  version = "1.2",
  luaversion = "5.1",
  tiledversion = "1.2.1",
  orientation = "orthogonal",
  renderorder = "right-down",
  width = 10,
  height = 10,
  tilewidth = 32,
  tileheight = 32,
  nextlayerid = 6,
  nextobjectid = 3,
  properties = {},
  tilesets = {
    {
      name = "tilesNstuff",
      firstgid = 1,
      filename = "../../../../../../../../Documents/tilesNstuff.tsx",
      tilewidth = 32,
      tileheight = 32,
      spacing = 0,
      margin = 0,
      columns = 8,
      image = "Media/Textures/tilesNstuff.png",
      imagewidth = 256,
      imageheight = 192,
      tileoffset = {
        x = 0,
        y = 0
      },
      grid = {
        orientation = "orthogonal",
        width = 32,
        height = 32
      },
      properties = {},
      terrains = {},
      tilecount = 48,
      tiles = {}
    }
  },
  layers = {
    {
      type = "imagelayer",
      id = 5,
      name = "BackgroundImage",
      visible = true,
      opacity = 1,
      offsetx = 0,
      offsety = 0,
      image = "Media/Textures/BackgroundImage.png",
      properties = {}
    },
    {
      type = "tilelayer",
      id = 1,
      name = "BottomTileLayer",
      x = 0,
      y = 0,
      width = 10,
      height = 10,
      visible = true,
      opacity = 1,
      offsetx = 0,
      offsety = 0,
      properties = {},
      encoding = "lua",
      data = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        33, 33, 33, 0, 0, 33, 0, 0, 33, 33,
        40, 40, 40, 0, 0, 40, 0, 0, 40, 40,
        40, 40, 40, 0, 0, 40, 0, 0, 40, 40,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
      }
    },
    {
      type = "objectgroup",
      id = 3,
      name = "BottomObjLayer",
      visible = true,
      opacity = 1,
      offsetx = 0,
      offsety = 0,
      draworder = "topdown",
      properties = {},
      objects = {
        {
          id = 1,
          name = "BottomObj1",
          type = "NPC",
          shape = "rectangle",
          x = 16.5,
          y = 177,
          width = 32,
          height = 32,
          rotation = 0,
          gid = 19,
          visible = true,
          properties = {
            ["color"] = "blue",
            ["health"] = 100
          }
        }
      }
    },
    {
      type = "tilelayer",
      id = 2,
      name = "TopTileLayer",
      x = 0,
      y = 0,
      width = 10,
      height = 10,
      visible = true,
      opacity = 1,
      offsetx = 0,
      offsety = 0,
      properties = {},
      encoding = "lua",
      data = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 11, 11, 0, 11, 11, 0, 0,
        0, 0, 0, 11, 11, 0, 11, 11, 0, 0,
        0, 0, 0, 11, 11, 0, 11, 11, 0, 0,
        33, 33, 33, 33, 33, 33, 33, 33, 33, 33,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
      }
    },
    {
      type = "objectgroup",
      id = 4,
      name = "TopObjLayer",
      visible = false,
      opacity = 1,
      offsetx = 0,
      offsety = 0,
      draworder = "topdown",
      properties = {},
      objects = {
        {
          id = 2,
          name = "TopObj1",
          type = "NPC",
          shape = "rectangle",
          x = 273,
          y = 177,
          width = 32,
          height = 32,
          rotation = 0,
          gid = 20,
          visible = true,
          properties = {}
        }
      }
    }
  }
}
