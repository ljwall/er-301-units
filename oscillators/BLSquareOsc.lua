local blOsc = require "bloscillators.libbloscillators"
local Class = require "Base.Class"
local Unit = require "Unit"
local Pitch = require "Unit.ViewControl.Pitch"
local GainBias = require "Unit.ViewControl.GainBias"
local Gate = require "Unit.ViewControl.Gate"
local Encoder = require "Encoder"

local BLSquareOscUnit = Class {}
BLSquareOscUnit:include(Unit)

function BLSquareOscUnit:init(args)
  args.title = "BL Square"
  args.mnemonic = "SQ"
  Unit.init(self, args)
end

function BLSquareOscUnit:onLoadGraph(channelCount)
  local osc = self:addObject("osc", blOsc.BLSquareOsc())
  local tune = self:addObject("tune", app.ConstantOffset())
  local tuneRange = self:addObject("tuneRange", app.MinMax())
  local f0 = self:addObject("f0", app.GainBias())
  local f0Range = self:addObject("f0Range", app.MinMax())
  local pw = self:addObject("pw", app.GainBias())
  local pwRange = self:addObject("pwRange", app.MinMax())
  local sync = self:addObject("sync", app.Comparator())
  sync:setTriggerMode()

  connect(sync, "Out", osc, "HardSync")

  connect(tune, "Out", tuneRange, "In")
  connect(tune, "Out", osc, "V/Oct")

  connect(f0, "Out", osc, "Fundamental")
  connect(f0, "Out", f0Range, "In")

  connect(pw, "Out", osc, "PulseWidth")
  connect(pw, "Out", pwRange, "In")

  connect(osc, "Out", self, "Out1")
  if channelCount > 1 then
    connect(osc, "Out", self, "Out2")
  end

  self:addMonoBranch("tune", tune, "In", tune, "Out")
  self:addMonoBranch("f0", f0, "In", f0, "Out")
  self:addMonoBranch("pw", pw, "In", pw, "Out")
  self:addMonoBranch("sync", sync, "In", sync, "Out")
end

local views = {
  expanded = {
    "tune",
    "freq",
    "sync",
    "pw"
  },
  collapsed = {},
}

function BLSquareOscUnit:onLoadViews(objects, branches)
  local controls = {}

  controls.tune = Pitch {
    button = "V/oct",
    branch = branches.tune,
    description = "V/oct",
    offset = objects.tune,
    range = objects.tuneRange
  }

  controls.freq = GainBias {
    button = "f0",
    description = "Fundamental",
    branch = branches.f0,
    gainbias = objects.f0,
    range = objects.f0Range,
    biasMap = Encoder.getMap("oscFreq"),
    biasUnits = app.unitHertz,
    initialBias = 27.5,
    gainMap = Encoder.getMap("freqGain"),
    scaling = app.octaveScaling
  }

  controls.sync = Gate {
    button = "sync",
    description = "Hard Sync",
    branch = branches.sync,
    comparator = objects.sync
  }

  controls.pw = GainBias {
    button = "pw",
    description = "Pulse Width",
    branch = branches.pw,
    gainbias = objects.pw,
    range = objects.pwRange,
    biasMap = Encoder.getMap("[0,1]"),
    initialBias = 0.5,
    gainMap = Encoder.getMap("[0,1]"),
  }

  return controls, views
end

return BLSquareOscUnit
