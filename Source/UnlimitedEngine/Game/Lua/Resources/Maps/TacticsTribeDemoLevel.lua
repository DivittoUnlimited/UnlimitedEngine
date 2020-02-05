return {
  version = "1.2",
  luaversion = "5.1",
  tiledversion = "1.3.2",
  orientation = "orthogonal",
  renderorder = "right-down",
  width = 32,
  height = 24,
  tilewidth = 32,
  tileheight = 32,
  nextlayerid = 3,
  nextobjectid = 10,
  properties = {},
  tilesets = {
    {
      name = "DemoLevel",
      firstgid = 1,
      filename = "DemoLevel.tsx",
      tilewidth = 32,
      tileheight = 32,
      spacing = 0,
      margin = 0,
      columns = 16,
      image = "../../../../Media/Textures/DemoLevel.png",
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
      tiles = {
        {
          id = 0,
          properties = {
            ["type"] = "Ground"
          }
        },
        {
          id = 48,
          properties = {
            ["type"] = "Mud"
          }
        },
        {
          id = 96,
          properties = {
            ["type"] = "Road"
          }
        }
      }
    },
    {
      name = "LightInfantry",
      firstgid = 193,
      filename = "LightInfantry.tsx",
      tilewidth = 32,
      tileheight = 32,
      spacing = 0,
      margin = 0,
      columns = 1,
      image = "../../../../Media/Textures/LightInfantry.png",
      imagewidth = 32,
      imageheight = 32,
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
      tilecount = 1,
      tiles = {}
    },
    {
      name = "SpawnPoint",
      firstgid = 194,
      filename = "SpawnPoint.tsx",
      tilewidth = 32,
      tileheight = 32,
      spacing = 0,
      margin = 0,
      columns = 1,
      image = "../../../../Media/Textures/SpawnPoint.png",
      imagewidth = 32,
      imageheight = 32,
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
      width = 32,
      height = 24,
      visible = true,
      opacity = 1,
      offsetx = 0,
      offsety = 0,
      properties = {},
      encoding = "lua",
      data = {
        1, 1, 1, 1, 1, 1, 97, 97, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 97, 97, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 97, 97, 1, 1, 1, 1, 1, 1, 1, 1, 49, 49, 49, 1, 1, 1, 1, 1, 97, 97, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 97, 97, 1, 1, 1, 1, 1, 1, 1, 1, 49, 49, 49, 1, 1, 1, 1, 1, 97, 97, 1, 1, 49, 49, 1, 1,
        1, 1, 1, 1, 1, 1, 97, 97, 1, 1, 1, 1, 1, 1, 1, 1, 49, 49, 49, 1, 1, 1, 1, 1, 97, 97, 1, 49, 49, 49, 49, 1,
        1, 1, 1, 1, 1, 1, 97, 97, 1, 1, 1, 1, 49, 49, 1, 1, 49, 49, 49, 1, 1, 1, 1, 1, 97, 97, 1, 49, 49, 49, 49, 1,
        1, 1, 1, 1, 1, 1, 97, 97, 1, 1, 1, 49, 49, 49, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 97, 97, 1, 1, 49, 49, 49, 1,
        1, 1, 1, 1, 1, 1, 97, 97, 1, 1, 1, 49, 49, 49, 1, 1, 1, 1, 1, 1, 49, 49, 1, 1, 97, 97, 1, 1, 49, 49, 49, 1,
        1, 1, 1, 1, 1, 1, 97, 97, 1, 1, 49, 49, 49, 49, 1, 1, 1, 1, 1, 49, 49, 49, 49, 49, 97, 97, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 97, 97, 1, 49, 49, 49, 49, 49, 1, 1, 1, 1, 1, 49, 49, 49, 49, 49, 97, 97, 1, 1, 1, 1, 1, 1,
        1, 1, 49, 49, 1, 1, 97, 97, 1, 49, 49, 49, 1, 1, 1, 1, 1, 1, 1, 1, 49, 49, 49, 49, 97, 97, 1, 1, 49, 49, 1, 1,
        1, 49, 49, 49, 49, 1, 97, 97, 1, 1, 49, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 49, 49, 97, 97, 1, 49, 49, 49, 1, 1,
        1, 49, 49, 49, 49, 1, 97, 97, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 97, 97, 1, 49, 49, 49, 1, 1,
        1, 1, 49, 49, 1, 1, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 97, 1, 49, 49, 49, 1, 1,
        1, 1, 1, 1, 1, 1, 97, 97, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 49, 1, 1, 1, 1, 1, 97, 97, 1, 49, 49, 49, 1, 1,
        1, 1, 1, 1, 1, 1, 97, 97, 1, 1, 49, 49, 1, 1, 1, 1, 1, 49, 49, 49, 1, 1, 1, 1, 97, 97, 1, 49, 49, 49, 1, 1,
        1, 1, 1, 1, 1, 1, 97, 97, 1, 1, 49, 49, 49, 1, 1, 1, 1, 49, 49, 49, 1, 1, 1, 1, 97, 97, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 97, 97, 1, 49, 49, 49, 49, 49, 1, 1, 1, 1, 49, 1, 1, 49, 49, 1, 97, 97, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 97, 97, 49, 49, 49, 49, 49, 49, 49, 1, 1, 1, 1, 1, 49, 49, 49, 49, 97, 97, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 97, 97, 1, 49, 49, 49, 49, 49, 1, 1, 1, 1, 1, 1, 49, 49, 49, 49, 97, 97, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 97, 97, 1, 1, 49, 49, 49, 1, 1, 1, 1, 1, 1, 1, 1, 49, 49, 1, 97, 97, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 97, 97, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 97, 97, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 97, 97, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 97, 97, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 97, 97, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 97, 97, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 97, 97, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 97, 97, 1, 1, 1, 1, 1, 1
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
          type = "LightInfantry",
          shape = "rectangle",
          x = 192,
          y = 192,
          width = 32,
          height = 32,
          rotation = 0,
          gid = 193,
          visible = true,
          properties = {
            ["Team"] = "Blue"
          }
        },
        {
          id = 2,
          name = "",
          type = "LightInfantry",
          shape = "rectangle",
          x = 768,
          y = 416,
          width = 32,
          height = 32,
          rotation = 0,
          gid = 193,
          visible = true,
          properties = {
            ["Team"] = "Red"
          }
        },
        {
          id = 3,
          name = "",
          type = "LightInfantry",
          shape = "rectangle",
          x = 480,
          y = 416,
          width = 32,
          height = 32,
          rotation = 0,
          gid = 193,
          visible = true,
          properties = {
            ["Team"] = "Blue"
          }
        },
        {
          id = 4,
          name = "",
          type = "LightInfantry",
          shape = "rectangle",
          x = 352,
          y = 416,
          width = 32,
          height = 32,
          rotation = 0,
          gid = 193,
          visible = true,
          properties = {
            ["Team"] = "Blue"
          }
        },
        {
          id = 5,
          name = "",
          type = "LightInfantry",
          shape = "rectangle",
          x = 288,
          y = 192,
          width = 32,
          height = 32,
          rotation = 0,
          gid = 193,
          visible = true,
          properties = {
            ["Team"] = "Red"
          }
        },
        {
          id = 6,
          name = "",
          type = "LightInfantry",
          shape = "rectangle",
          x = 640,
          y = 416,
          width = 32,
          height = 32,
          rotation = 0,
          gid = 193,
          visible = true,
          properties = {
            ["Team"] = "Red"
          }
        },
        {
          id = 7,
          name = "",
          type = "SpawnPoint",
          shape = "rectangle",
          x = 128,
          y = 96,
          width = 32,
          height = 32,
          rotation = 0,
          gid = 194,
          visible = true,
          properties = {
            ["Team"] = "Blue"
          }
        },
        {
          id = 8,
          name = "",
          type = "SpawnPoint",
          shape = "rectangle",
          x = 864,
          y = 672,
          width = 32,
          height = 32,
          rotation = 0,
          gid = 194,
          visible = true,
          properties = {
            ["Team"] = "Red"
          }
        },
        {
          id = 9,
          name = "",
          type = "HeavyInfantry",
          shape = "rectangle",
          x = 224,
          y = 256,
          width = 32,
          height = 32,
          rotation = 0,
          gid = 193,
          visible = true,
          properties = {
            ["Team"] = "Red"
          }
        }
      }
    }
  }
}
