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
      name = "BackgroundImageLayer",
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
    }
  }
}
