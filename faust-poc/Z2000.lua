local lib = require "faustpoc.libfaustpoc"
local Class = require "Base.Class"
local Unit = require "Unit"
local GainBias = require "Unit.ViewControl.GainBias"
local Encoder = require "Encoder"

local Z2000 = Class {}
Z2000:include(Unit)

function Z2000:init(args)
  args.title = "Z2000"
  args.mnemonic = "Z2"
  Unit.init(self, args)
end

function Z2000:onLoadGraph(channelCount)
  local del = self:addObject("del", lib.Z2000())

  connect(del, "Out", self, "Out1")
  connect(self, "In1", del, "In")

  if channelCount == 2 then
    connect(del, "Out", self, "Out2")
  end
end

local views = {
  expanded = {},
  collapsed = {},
}

function Z2000:onLoadViews(objects, branches)
  local controls = {}
  return controls, views
end

return Z2000
