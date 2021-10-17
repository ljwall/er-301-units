local lib = require "faustpoc.libfaustpoc"
local Class = require "Base.Class"
local Unit = require "Unit"
local GainBias = require "Unit.ViewControl.GainBias"
local Encoder = require "Encoder"

local FDelayltv = Class {}
FDelayltv:include(Unit)

function FDelayltv:init(args)
  args.title = "FDelayltv"
  args.mnemonic = "FD"
  Unit.init(self, args)
end

function FDelayltv:onLoadGraph(channelCount)
  local del = self:addObject("del", lib.FDelayltv())
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

function FDelayltv:onLoadViews(objects, branches)
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

return FDelayltv
