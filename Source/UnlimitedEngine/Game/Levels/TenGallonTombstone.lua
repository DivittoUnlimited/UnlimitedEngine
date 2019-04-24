return {
  version = "1.2",
  luaversion = "5.1",
  tiledversion = "1.2.3",
  orientation = "orthogonal",
  renderorder = "right-down",
  width = 4,
  height = 4,
  tilewidth = 256,
  tileheight = 192,
  nextlayerid = 3,
  nextobjectid = 3,
  properties = {},
  tilesets = {
    {
      name = "Dirt",
      firstgid = 1,
      filename = "../../Tiled/Dirt.tsx",
      tilewidth = 256,
      tileheight = 192,
      spacing = 0,
      margin = 0,
      columns = 1,
      image = "../../Media/Textures/Dirt.png",
      imagewidth = 256,
      imageheight = 192,
      tileoffset = {
        x = 0,
        y = 0
      },
      grid = {
        orientation = "orthogonal",
        width = 256,
        height = 192
      },
      properties = {},
      terrains = {},
      tilecount = 1,
      tiles = {}
    },
    {
      name = "Hat",
      firstgid = 2,
      filename = "../../Tiled/Hat.tsx",
      tilewidth = 200,
      tileheight = 192,
      spacing = 0,
      margin = 0,
      columns = 1,
      image = "../../Media/Textures/Hat.png",
      imagewidth = 200,
      imageheight = 192,
      tileoffset = {
        x = 0,
        y = 0
      },
      grid = {
        orientation = "orthogonal",
        width = 200,
        height = 192
      },
      properties = {},
      terrains = {},
      tilecount = 1,
      tiles = {}
    }
  },
  layers = {
    {
      type = "tilelayer",
      id = 1,
      name = "TileLayer1",
      x = 0,
      y = 0,
      width = 4,
      height = 4,
      visible = true,
      opacity = 1,
      offsetx = 0,
      offsety = 0,
      properties = {},
      encoding = "lua",
      data = {
        1, 1, 1, 1,
        1, 1, 1, 1,
        1, 1, 1, 1,
        1, 1, 1, 1
      }
    },
    {
      type = "objectgroup",
      id = 2,
      name = "ObjectLayer",
      visible = true,
      opacity = 1,
      offsetx = 0,
      offsety = 0,
      draworder = "topdown",
      properties = {},
      objects = {
        {
          id = 1,
          name = "GoodGuy",
          type = "Player",
          shape = "rectangle",
          x = 512,
          y = 662,
          width = 200,
          height = 192,
          rotation = 0,
          gid = 2,
          visible = true,
          properties = {}
        },
        {
          id = 2,
          name = "BadGuy",
          type = "Cowboy",
          shape = "rectangle",
          x = 512,
          y = 112,
          width = 200,
          height = 192,
          rotation = 0,
          gid = 2,
          visible = true,
          properties = {}
        }
      }
    }
  }
}
