return {
  version = "1.2",
  luaversion = "5.1",
  tiledversion = "1.3.3",
  orientation = "orthogonal",
  renderorder = "right-down",
  width = 16,
  height = 12,
  tilewidth = 64,
  tileheight = 64,
  nextlayerid = 4,
  nextobjectid = 40,
  properties = {},
  tilesets = {
    {
      name = "DemoLevel",
      firstgid = 1,
      filename = "DemoLevel.tsx",
      tilewidth = 64,
      tileheight = 64,
      spacing = 0,
      margin = 0,
      columns = 8,
      image = "../../../../Media/Textures/DemoLevel.png",
      imagewidth = 512,
      imageheight = 384,
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
      tilecount = 48,
      tiles = {
        {
          id = 0,
          properties = {
            ["type"] = "Ground"
          }
        },
        {
          id = 16,
          properties = {
            ["type"] = "Mud"
          }
        },
        {
          id = 24,
          properties = {
            ["type"] = "Road"
          }
        }
      }
    },
    {
      name = "LightInfantry",
      firstgid = 49,
      filename = "LightInfantry.tsx",
      tilewidth = 64,
      tileheight = 64,
      spacing = 0,
      margin = 0,
      columns = 1,
      image = "../../../../Media/Textures/LightInfantry.png",
      imagewidth = 64,
      imageheight = 64,
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
    },
    {
      name = "HeavyInfantry",
      firstgid = 50,
      filename = "HeavyInfantry.tsx",
      tilewidth = 64,
      tileheight = 64,
      spacing = 0,
      margin = 0,
      columns = 1,
      image = "../../../../Media/Textures/HeavyInfantry.png",
      imagewidth = 64,
      imageheight = 64,
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
    },
    {
      name = "SpawnPoint",
      firstgid = 51,
      filename = "SpawnPoint.tsx",
      tilewidth = 64,
      tileheight = 64,
      spacing = 0,
      margin = 0,
      columns = 1,
      image = "../../../../Media/Textures/SpawnPoint.png",
      imagewidth = 64,
      imageheight = 64,
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
    },
    {
      name = "UnitSpriteSheet",
      firstgid = 52,
      filename = "UnitSpriteSheet.tsx",
      tilewidth = 64,
      tileheight = 64,
      spacing = 0,
      margin = 0,
      columns = 10,
      image = "../../../../Media/Textures/UnitSpriteSheet.png",
      imagewidth = 640,
      imageheight = 640,
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
    }
  },
  layers = {
    {
      type = "tilelayer",
      id = 1,
      name = "TileLayer1",
      x = 0,
      y = 0,
      width = 16,
      height = 12,
      visible = true,
      opacity = 1,
      offsetx = 0,
      offsety = 0,
      properties = {},
      encoding = "lua",
      data = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        17, 17, 17, 17, 17, 17, 17, 1, 1, 17, 17, 17, 17, 17, 17, 17,
        17, 17, 17, 17, 17, 17, 17, 1, 1, 17, 17, 17, 17, 17, 17, 17,
        17, 17, 17, 17, 17, 17, 17, 1, 1, 17, 17, 17, 17, 17, 17, 17,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
      }
    },
    {
      type = "objectgroup",
      id = 2,
      name = "ObjectLayer1",
      visible = true,
      opacity = 1,
      offsetx = 0,
      offsety = 0,
      draworder = "topdown",
      properties = {},
      objects = {
        {
          id = 11,
          name = "",
          type = "SpawnPoint",
          shape = "rectangle",
          x = 832,
          y = 192,
          width = 64,
          height = 64,
          rotation = 0,
          gid = 51,
          visible = true,
          properties = {
            ["Team"] = "Red"
          }
        },
        {
          id = 12,
          name = "",
          type = "SpawnPoint",
          shape = "rectangle",
          x = 128,
          y = 640,
          width = 64,
          height = 64,
          rotation = 0,
          gid = 51,
          visible = true,
          properties = {
            ["Team"] = "Blue"
          }
        },
        {
          id = 15,
          name = "",
          type = "Fighter",
          shape = "rectangle",
          x = 640,
          y = 576,
          width = 64,
          height = 64,
          rotation = 0,
          gid = 52,
          visible = true,
          properties = {
            ["Team"] = "Blue"
          }
        },
        {
          id = 24,
          name = "",
          type = "Fighter",
          shape = "rectangle",
          x = 640,
          y = 128,
          width = 64,
          height = 64,
          rotation = 0,
          gid = 52,
          visible = true,
          properties = {
            ["Team"] = "Red"
          }
        },
        {
          id = 27,
          name = "",
          type = "Fighter",
          shape = "rectangle",
          x = 320,
          y = 576,
          width = 64,
          height = 64,
          rotation = 0,
          gid = 52,
          visible = true,
          properties = {
            ["Team"] = "Blue"
          }
        },
        {
          id = 28,
          name = "",
          type = "Fighter",
          shape = "rectangle",
          x = 448,
          y = 448,
          width = 64,
          height = 64,
          rotation = 0,
          gid = 52,
          visible = true,
          properties = {
            ["Team"] = "Blue"
          }
        },
        {
          id = 29,
          name = "",
          type = "Fighter",
          shape = "rectangle",
          x = 768,
          y = 256,
          width = 64,
          height = 64,
          rotation = 0,
          gid = 52,
          visible = true,
          properties = {
            ["Team"] = "Red"
          }
        },
        {
          id = 30,
          name = "",
          type = "Fighter",
          shape = "rectangle",
          x = 256,
          y = 320,
          width = 64,
          height = 64,
          rotation = 0,
          gid = 52,
          visible = true,
          properties = {
            ["Team"] = "Red"
          }
        },
        {
          id = 33,
          name = "",
          type = "TacticalLocation",
          shape = "rectangle",
          x = 512,
          y = 384,
          width = 64,
          height = 64,
          rotation = 0,
          visible = true,
          properties = {
            ["Importance"] = "10"
          }
        },
        {
          id = 34,
          name = "",
          type = "TacticalLocation",
          shape = "rectangle",
          x = 448,
          y = 384,
          width = 64,
          height = 64,
          rotation = 0,
          visible = true,
          properties = {
            ["Importance"] = "5"
          }
        },
        {
          id = 35,
          name = "",
          type = "TacticalLocation",
          shape = "rectangle",
          x = 128,
          y = 576,
          width = 64,
          height = 64,
          rotation = 0,
          visible = true,
          properties = {
            ["Importance"] = "5"
          }
        },
        {
          id = 39,
          name = "",
          type = "TacticalLocation",
          shape = "rectangle",
          x = 832,
          y = 128,
          width = 64,
          height = 64,
          rotation = 0,
          visible = true,
          properties = {
            ["Importance"] = "5"
          }
        }
      }
    }
  }
}
