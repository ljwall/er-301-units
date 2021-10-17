local lib = require "faustpoc.libfaustpoc"
local Class = require "Base.Class"
local Unit = require "Unit"
local GainBias = require "Unit.ViewControl.GainBias"
local Encoder = require "Encoder"

local FDelay = Class {}
FDelay:include(Unit)

function FDelay:init(args)
  args.title = "FDelay"
  args.mnemonic = "FD"
  Unit.init(self, args)
end

function FDelay:onLoadGraph(channelCount)
  local del = self:addObject("del", lib.FDelay())
  local length = self:addObject("length", app.GainBias())
  local lengthRange = self:addObject("lengthRange", app.MinMax())

  connect(length, "Out", del, "Length")
  connect(length, "Out", lengthRange, "In")

  connect(del, "Out", self, "Out1")
  connect(self, "In1", del, "In")

  if channelCount == 2 then
    connect(del, "Out", self, "Out2")
  end

  self:addMonoBranch("length", length, "In", length, "Out")
end

local views = {
  expanded = {
    "length"
  },
  collapsed = {},
}

local map = app.LinearDialMap(5, 1000)
map:setSteps(100, 50, 10, 1)

function FDelay:onLoadViews(objects, branches)
  local controls = {}


  controls.length = GainBias {
    button = "length",
    description = "Length",
    branch = branches.length,
    gainbias = objects.length,
    range = objects.lengthRange,
    biasMap = map,
    initialBias = 200,
    gainMap = map,
  }

  return controls, views
end

return FDelay
