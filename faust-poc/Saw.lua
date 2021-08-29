local lib = require "faust_poc.libfaustpoc"
local Class = require "Base.Class"
local Unit = require "Unit"
local Pitch = require "Unit.ViewControl.Pitch"
local GainBias = require "Unit.ViewControl.GainBias"
local Gate = require "Unit.ViewControl.Gate"
local Encoder = require "Encoder"

local Saw = Class {}
Saw:include(Unit)

function Saw:init(args)
  args.title = "Faust Saw"
  args.mnemonic = "FS"
  Unit.init(self, args)
end

function Saw:onLoadGraph(channelCount)
  local osc = self:addObject("osc", lib.Saw())

  -- local f0 = self:addObject("f0", app.GainBias())
  -- local f0Range = self:addObject("f0Range", app.MinMax())

  -- connect(f0, "Out", osc, "Freq")
  -- connect(f0, "Out", f0Range, "In")

  connect(osc, "Out", self, "Out1")
  connect(self, "In1", osc, "In")

  if channelCount > 1 then
    connect(osc, "Out", self, "Out2")
  end

  -- self:addMonoBranch("f0", f0, "In", f0, "Out")
end

local views = {
  expanded = {
    "freq",
  },
  collapsed = {},
}

function Saw:onLoadViews(objects, branches)
  local controls = {}

  -- controls.freq = GainBias {
  --   button = "f0",
  --   description = "Fundamental",
  --   branch = branches.f0,
  --   gainbias = objects.f0,
  --   range = objects.f0Range,
  --   biasMap = Encoder.getMap("oscFreq"),
  --   biasUnits = app.unitHertz,
  --   initialBias = 27.5,
  --   gainMap = Encoder.getMap("freqGain"),
  --   scaling = app.octaveScaling
  -- }

  return controls, views
end

return Saw
