return {
  version = "1.2",
  luaversion = "5.1",
  tiledversion = "1.3.1",
  orientation = "orthogonal",
  renderorder = "right-down",
  width = 16,
  height = 12,
  tilewidth = 64,
  tileheight = 64,
  nextlayerid = 3,
  nextobjectid = 19,
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
      name = "SpawnPoint",
      firstgid = 50,
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
        1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 17, 17, 25, 17, 17, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 17, 17, 25, 17, 17, 17, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 17, 17, 17, 25, 17, 17, 17, 1, 1, 1, 1, 1,
        25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25,
        1, 1, 1, 17, 17, 17, 17, 25, 17, 17, 17, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 25, 1, 1, 1, 1, 1, 1, 1, 1
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
          x = 448,
          y = 320,
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
          id = 4,
          name = "",
          type = "LightInfantry",
          shape = "rectangle",
          x = 128,
          y = 576,
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
          id = 5,
          name = "",
          type = "LightInfantry",
          shape = "rectangle",
          x = 192,
          y = 640,
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
          id = 12,
          name = "",
          type = "HeavyInfantry",
          shape = "rectangle",
          x = 448,
          y = 192,
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
          id = 13,
          name = "",
          type = "LightInfantry",
          shape = "rectangle",
          x = 832,
          y = 128,
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
          id = 14,
          name = "",
          type = "LightInfantry",
          shape = "rectangle",
          x = 448,
          y = 576,
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
          id = 15,
          name = "",
          type = "LightInfantry",
          shape = "rectangle",
          x = 832,
          y = 256,
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
          id = 16,
          name = "",
          type = "HeavyInfantry",
          shape = "rectangle",
          x = 448,
          y = 704,
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
          id = 17,
          name = "",
          type = "SpawnPoint",
          shape = "rectangle",
          x = 832,
          y = 192,
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
          id = 18,
          name = "",
          type = "SpawnPoint",
          shape = "rectangle",
          x = 128,
          y = 640,
          width = 64,
          height = 64,
          rotation = 0,
          gid = 50,
          visible = true,
          properties = {
            ["Team"] = "Red"
          }
        }
      }
    }
  }
}
