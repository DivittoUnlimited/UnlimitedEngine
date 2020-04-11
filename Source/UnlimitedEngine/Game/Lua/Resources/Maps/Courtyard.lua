return {
  version = "1.2",
  luaversion = "5.1",
  tiledversion = "1.3.3",
  orientation = "orthogonal",
  renderorder = "left-down",
  width = 32,
  height = 32,
  tilewidth = 64,
  tileheight = 64,
  nextlayerid = 3,
  nextobjectid = 10,
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
          type = "Ground",
          properties = {
            ["type"] = "Ground"
          }
        },
        {
          id = 16,
          type = "Mud",
          properties = {
            ["type"] = "Mud"
          }
        },
        {
          id = 24,
          type = "Road",
          properties = {
            ["type"] = "Road"
          }
        }
      }
    },
    {
      name = "SpawnPoint",
      firstgid = 49,
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
      firstgid = 50,
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
      width = 32,
      height = 32,
      visible = true,
      opacity = 1,
      offsetx = 0,
      offsety = 0,
      properties = {},
      encoding = "lua",
      data = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 1, 1, 1,
        1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1,
        1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1,
        1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1,
        1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 17, 17, 17, 17, 17, 1, 1, 25, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1,
        1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 25, 1, 17, 17, 17, 17, 17, 17, 17, 1, 25, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1,
        1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 25, 17, 17, 17, 17, 17, 17, 17, 17, 17, 25, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1,
        1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 25, 1, 17, 17, 17, 17, 17, 17, 17, 1, 25, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1,
        1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 17, 17, 17, 17, 17, 1, 1, 25, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1,
        1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1,
        1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1,
        1, 1, 1, 25, 1, 1, 17, 17, 17, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1,
        1, 1, 1, 25, 1, 17, 17, 17, 17, 17, 1, 25, 1, 1, 1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1,
        1, 1, 1, 25, 17, 17, 17, 17, 17, 17, 17, 25, 1, 1, 1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1,
        1, 1, 1, 25, 1, 17, 17, 17, 17, 17, 1, 25, 1, 1, 1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1,
        1, 1, 1, 25, 1, 1, 17, 17, 17, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 17, 17, 1, 1, 25, 1, 1, 1,
        1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 1, 1, 25, 1, 17, 17, 17, 17, 1, 25, 1, 1, 1,
        1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 1, 1, 25, 17, 17, 17, 17, 17, 17, 25, 1, 1, 1,
        1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 1, 1, 25, 1, 17, 17, 17, 17, 1, 25, 1, 1, 1,
        1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 17, 17, 1, 1, 25, 1, 1, 1,
        1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1,
        1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1,
        1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1,
        1, 1, 1, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
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
          id = 1,
          name = "",
          type = "SpawnPoint",
          shape = "rectangle",
          x = 256,
          y = 1920,
          width = 64,
          height = 64,
          rotation = 0,
          gid = 49,
          visible = true,
          properties = {
            ["Team"] = "Blue"
          }
        },
        {
          id = 2,
          name = "",
          type = "SpawnPoint",
          shape = "rectangle",
          x = 1728,
          y = 192,
          width = 64,
          height = 64,
          rotation = 0,
          gid = 49,
          visible = true,
          properties = {
            ["Team"] = "Red"
          }
        },
        {
          id = 3,
          name = "",
          type = "Fighter",
          shape = "rectangle",
          x = 192,
          y = 192,
          width = 64,
          height = 64,
          rotation = 0,
          gid = 50,
          visible = true,
          properties = {
            ["Team"] = "Red"
          }
        },
        {
          id = 4,
          name = "",
          type = "Fighter",
          shape = "rectangle",
          x = 128,
          y = 256,
          width = 64,
          height = 64,
          rotation = 0,
          gid = 50,
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
          x = 128,
          y = 128,
          width = 64,
          height = 64,
          rotation = 0,
          gid = 50,
          visible = true,
          properties = {
            ["Team"] = "Red"
          }
        },
        {
          id = 6,
          name = "",
          type = "Fighter",
          shape = "rectangle",
          x = 384,
          y = 1792,
          width = 64,
          height = 64,
          rotation = 0,
          gid = 50,
          visible = true,
          properties = {
            ["Team"] = "Blue"
          }
        },
        {
          id = 7,
          name = "",
          type = "Fighter",
          shape = "rectangle",
          x = 512,
          y = 1792,
          width = 64,
          height = 64,
          rotation = 0,
          gid = 50,
          visible = true,
          properties = {
            ["Team"] = "Blue"
          }
        },
        {
          id = 8,
          name = "",
          type = "Fighter",
          shape = "rectangle",
          x = 704,
          y = 1792,
          width = 64,
          height = 64,
          rotation = 0,
          gid = 50,
          visible = true,
          properties = {
            ["Team"] = "Blue"
          }
        },
        {
          id = 9,
          name = "",
          type = "TacticalLocation",
          shape = "rectangle",
          x = 704,
          y = 576,
          width = 64,
          height = 64,
          rotation = 0,
          visible = true,
          properties = {
            ["Importance"] = "10"
          }
        }
      }
    }
  }
}
