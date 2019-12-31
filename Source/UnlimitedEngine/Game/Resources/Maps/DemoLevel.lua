return {
  version = "1.2",
  luaversion = "5.1",
  tiledversion = "1.3.1",
  orientation = "orthogonal",
  renderorder = "right-down",
  width = 32,
  height = 24,
  tilewidth = 32,
  tileheight = 32,
  nextlayerid = 2,
  nextobjectid = 1,
  properties = {},
  tilesets = {
    {
      name = "DemoLevel",
      firstgid = 1,
      filename = "../../../../../../../Documents/DemoLevel.tsx",
      tilewidth = 32,
      tileheight = 32,
      spacing = 0,
      margin = 0,
      columns = 16,
      image = "../../../Media/Textures/DemoLevel.png",
      imagewidth = 512,
      imageheight = 384,
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
      tilecount = 192,
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
      width = 32,
      height = 24,
      visible = true,
      opacity = 1,
      offsetx = 0,
      offsety = 0,
      properties = {},
      encoding = "lua",
      data = {
        1, 1, 1, 2, 97, 98, 97, 97, 7, 49, 49, 49, 49, 49, 49, 49, 49, 54, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 50,
        17, 17, 17, 18, 113, 114, 97, 98, 7, 49, 50, 50, 65, 49, 50, 50, 50, 54, 50, 50, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 49, 50,
        17, 18, 18, 2, 113, 114, 97, 98, 7, 49, 50, 66, 50, 50, 50, 66, 66, 54, 66, 66, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
        17, 18, 17, 18, 113, 114, 97, 98, 7, 49, 49, 49, 49, 49, 49, 49, 49, 54, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
        1, 2, 1, 2, 113, 114, 97, 98, 7, 65, 49, 50, 50, 50, 50, 50, 50, 54, 50, 50, 50, 50, 50, 50, 65, 65, 65, 65, 65, 65, 65, 49,
        17, 18, 17, 18, 113, 114, 97, 98, 7, 49, 49, 49, 50, 50, 50, 50, 50, 54, 50, 50, 50, 50, 50, 66, 50, 50, 50, 50, 50, 50, 50, 50,
        1, 2, 1, 2, 113, 114, 97, 98, 7, 65, 49, 49, 49, 49, 49, 49, 49, 54, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
        17, 18, 17, 18, 113, 114, 97, 98, 7, 49, 50, 65, 65, 65, 49, 50, 50, 54, 50, 50, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 49, 50,
        1, 2, 1, 2, 113, 114, 97, 98, 7, 49, 50, 50, 50, 50, 50, 49, 49, 54, 49, 49, 49, 49, 49, 49, 49, 50, 50, 50, 50, 50, 50, 50,
        17, 18, 17, 18, 113, 114, 97, 98, 7, 49, 49, 50, 49, 49, 49, 65, 65, 54, 65, 65, 65, 65, 65, 65, 65, 49, 49, 49, 49, 49, 49, 49,
        1, 2, 1, 2, 113, 114, 97, 98, 7, 65, 49, 49, 65, 65, 65, 49, 49, 54, 49, 49, 50, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49, 49,
        17, 18, 17, 18, 113, 114, 97, 98, 49, 50, 50, 49, 49, 49, 49, 50, 65, 54, 65, 49, 49, 49, 49, 50, 50, 50, 50, 50, 50, 50, 50, 50,
        1, 2, 1, 2, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54,
        17, 18, 17, 18, 113, 114, 97, 98, 65, 65, 65, 65, 66, 65, 49, 49, 50, 54, 66, 66, 66, 66, 66, 49, 49, 49, 49, 49, 50, 49, 49, 50,
        1, 2, 1, 2, 113, 114, 97, 98, 7, 65, 49, 49, 49, 49, 49, 49, 49, 54, 49, 49, 49, 49, 49, 65, 65, 65, 65, 49, 50, 65, 49, 50,
        17, 18, 17, 18, 113, 114, 97, 98, 7, 49, 49, 65, 49, 50, 49, 50, 50, 54, 50, 65, 65, 65, 65, 65, 66, 65, 66, 49, 49, 50, 50, 50,
        1, 2, 1, 2, 113, 114, 97, 98, 7, 65, 65, 49, 50, 50, 49, 49, 49, 54, 49, 50, 50, 50, 50, 50, 50, 50, 50, 49, 50, 50, 50, 66,
        17, 18, 17, 18, 7, 7, 7, 7, 7, 49, 50, 49, 49, 49, 65, 65, 65, 54, 65, 66, 66, 66, 66, 66, 66, 66, 7, 7, 7, 7, 7, 7,
        1, 2, 1, 2, 8, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 7,
        17, 18, 17, 18, 8, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 54, 7,
        1, 2, 1, 2, 7, 7, 7, 7, 7, 49, 49, 65, 49, 49, 49, 65, 65, 65, 65, 66, 66, 66, 66, 66, 65, 65, 7, 7, 7, 7, 7, 7,
        17, 18, 17, 18, 113, 114, 97, 98, 7, 65, 65, 65, 65, 65, 65, 65, 66, 65, 65, 65, 65, 65, 65, 65, 66, 50, 65, 66, 66, 66, 50, 50,
        1, 2, 1, 2, 113, 114, 98, 98, 7, 49, 65, 66, 50, 50, 65, 65, 65, 66, 50, 50, 50, 50, 50, 50, 66, 66, 66, 66, 66, 66, 50, 66,
        17, 18, 17, 18, 113, 114, 114, 114, 7, 65, 65, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 66, 65, 65, 65, 65, 65, 65, 66, 49
      }
    }
  }
}
