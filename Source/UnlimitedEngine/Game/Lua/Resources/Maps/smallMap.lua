return {
  version = "1.4",
  luaversion = "5.1",
  tiledversion = "1.4.1",
  orientation = "orthogonal",
  renderorder = "right-down",
  width = 16,
  height = 12,
  tilewidth = 64,
  tileheight = 64,
  nextlayerid = 4,
  nextobjectid = 7,
  properties = {},
  tilesets = {
    {
      name = "TexturePack",
      firstgid = 1,
      filename = "TexturePack.tsx",
      tilewidth = 64,
      tileheight = 64,
      spacing = 0,
      margin = 0,
      columns = 10,
      image = "../../../../Media/Textures/TexturePack.png",
      imagewidth = 640,
      imageheight = 640,
      objectalignment = "unspecified",
      tileoffset = {
        x = 0,
        y = 0
      },
      grid = {
        orientation = "orthogonal",
        width = 64,
        height = 64
      },
      properties = {},
      terrains = {},
      tilecount = 100,
      tiles = {
        {
          id = 0,
          type = "Ground",
          properties = {
            ["type"] = "Ground"
          }
        },
        {
          id = 1,
          type = "Mud",
          properties = {
            ["type"] = "Mud"
          }
        },
        {
          id = 2,
          type = "Road",
          properties = {
            ["type"] = "Road"
          }
        },
        {
          id = 3,
          type = "Water",
          properties = {
            ["type"] = "Water"
          }
        },
        {
          id = 12,
          type = "Road",
          properties = {
            ["type"] = "Road"
          }
        },
        {
          id = 22,
          type = "Road",
          properties = {
            ["type"] = "Road"
          }
        },
        {
          id = 32,
          type = "Road",
          properties = {
            ["type"] = "Road"
          }
        }
      }
    },
    {
      name = "UnitSpriteSheet",
      firstgid = 101,
      filename = "UnitSpriteSheet.tsx",
      tilewidth = 64,
      tileheight = 64,
      spacing = 0,
      margin = 0,
      columns = 10,
      image = "../../../../Media/Textures/UnitSpriteSheet.png",
      imagewidth = 640,
      imageheight = 640,
      objectalignment = "unspecified",
      tileoffset = {
        x = 0,
        y = 0
      },
      grid = {
        orientation = "orthogonal",
        width = 64,
        height = 64
      },
      properties = {},
      terrains = {},
      tilecount = 100,
      tiles = {}
    },
    {
      name = "SpawnPoint",
      firstgid = 201,
      filename = "SpawnPoint.tsx",
      tilewidth = 64,
      tileheight = 64,
      spacing = 0,
      margin = 0,
      columns = 1,
      image = "../../../../Media/Textures/SpawnPoint.png",
      imagewidth = 64,
      imageheight = 64,
      objectalignment = "unspecified",
      tileoffset = {
        x = 0,
        y = 0
      },
      grid = {
        orientation = "orthogonal",
        width = 64,
        height = 64
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
      x = 0,
      y = 0,
      width = 16,
      height = 12,
      id = 1,
      name = "TileLayer1",
      visible = true,
      opacity = 1,
      offsetx = 0,
      offsety = 0,
      properties = {},
      encoding = "lua",
      data = {
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
        4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4,
        4, 2, 1, 1, 1, 1, 1, 33, 33, 1, 1, 1, 1, 1, 2, 4,
        4, 2, 1, 1, 1, 1, 1, 33, 33, 1, 1, 1, 1, 1, 2, 4,
        4, 2, 1, 1, 1, 1, 1, 33, 33, 1, 1, 1, 1, 1, 2, 4,
        4, 2, 23, 23, 23, 23, 23, 33, 33, 23, 23, 23, 23, 23, 2, 4,
        4, 2, 23, 23, 23, 23, 23, 33, 33, 23, 23, 23, 23, 23, 2, 4,
        4, 2, 1, 1, 1, 1, 1, 33, 33, 1, 1, 1, 1, 1, 2, 4,
        4, 2, 1, 1, 1, 1, 1, 33, 33, 1, 1, 1, 1, 1, 2, 4,
        4, 2, 1, 1, 1, 1, 1, 33, 33, 1, 1, 1, 1, 1, 2, 4,
        4, 4, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 4, 4,
        4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4
      }
    },
    {
      type = "objectgroup",
      draworder = "topdown",
      id = 2,
      name = "ObjectLayer1",
      visible = true,
      opacity = 1,
      offsetx = 0,
      offsety = 0,
      properties = {},
      objects = {
        {
          id = 1,
          name = "",
          type = "Fighter",
          shape = "rectangle",
          x = 320,
          y = 256,
          width = 64,
          height = 64,
          rotation = 0,
          gid = 101,
          visible = true,
          properties = {
            ["Team"] = "Red"
          }
        },
        {
          id = 2,
          name = "",
          type = "Fighter",
          shape = "rectangle",
          x = 640,
          y = 256,
          width = 64,
          height = 64,
          rotation = 0,
          gid = 101,
          visible = true,
          properties = {
            ["Team"] = "Blue"
          }
        },
        {
          id = 3,
          name = "",
          type = "SpawnPoint",
          shape = "rectangle",
          x = 768,
          y = 256,
          width = 64,
          height = 64,
          rotation = 0,
          gid = 201,
          visible = true,
          properties = {
            ["Team"] = "Blue"
          }
        },
        {
          id = 4,
          name = "",
          type = "SpawnPoint",
          shape = "rectangle",
          x = 192,
          y = 256,
          width = 64,
          height = 64,
          rotation = 0,
          gid = 201,
          visible = true,
          properties = {
            ["Team"] = "Red"
          }
        },
        {
          id = 5,
          name = "",
          type = "Fighter",
          shape = "rectangle",
          x = 704,
          y = 320,
          width = 64,
          height = 64,
          rotation = 0,
          gid = 101,
          visible = true,
          properties = {
            ["Team"] = "Blue"
          }
        },
        {
          id = 6,
          name = "",
          type = "Fighter",
          shape = "rectangle",
          x = 704,
          y = 192,
          width = 64,
          height = 64,
          rotation = 0,
          gid = 101,
          visible = true,
          properties = {
            ["Team"] = "Blue"
          }
        }
      }
    },
    {
      type = "tilelayer",
      x = 0,
      y = 0,
      width = 16,
      height = 12,
      id = 3,
      name = "TileLayer2",
      visible = true,
      opacity = 1,
      offsetx = 0,
      offsety = 0,
      properties = {},
      encoding = "lua",
      data = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 15, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 25, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 35, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
      }
    }
  }
}
