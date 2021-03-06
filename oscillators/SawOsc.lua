local blOsc = require "band_limited_osc.libbloscillators"
local Class = require "Base.Class"
local Unit = require "Unit"
local Pitch = require "Unit.ViewControl.Pitch"
local GainBias = require "Unit.ViewControl.GainBias"
local Gate = require "Unit.ViewControl.Gate"
local Encoder = require "Encoder"
local NoddySync = require "band_limited_osc.NoddySync"

local BLSawOscUnit = Class {}
BLSawOscUnit:include(Unit)

function BLSawOscUnit:init(args)
  args.title = "Band Limited Saw"
  args.mnemonic = "SW"
  Unit.init(self, args)
end

function BLSawOscUnit:onLoadGraph(channelCount)
  local osc = self:addObject("osc", blOsc.BLSawOsc())
  local tune = self:addObject("tune", app.ConstantOffset())
  local tuneRange = self:addObject("tuneRange", app.MinMax())
  local f0 = self:addObject("f0", app.GainBias())
  local f0Range = self:addObject("f0Range", app.MinMax())

  local syncMonitor = self:addObject("syncMonitor", app.Comparator())
  syncMonitor:setTriggerMode()
  local highpass = self:addObject("highpass", blOsc.DCBlock())

  connect(highpass, "Out", syncMonitor, "In")
  connect(highpass, "Out", osc, "Sync")

  connect(tune, "Out", tuneRange, "In")
  connect(tune, "Out", osc, "V/Oct")

  connect(f0, "Out", osc, "Fundamental")
  connect(f0, "Out", f0Range, "In")

  connect(osc, "Out", self, "Out1")
  if channelCount > 1 then
    connect(osc, "Out", self, "Out2")
  end

  self:addMonoBranch("tune", tune, "In", tune, "Out")
  self:addMonoBranch("f0", f0, "In", f0, "Out")

  self:addMonoBranch("sync", highpass, "In", highpass, "Out")
end

local views = {
  expanded = {
    "tune",
    "freq",
    'sync',
  },
  collapsed = {},
}

function BLSawOscUnit:onLoadViews(objects, branches)
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

  controls.sync = NoddySync {
    button = "Sync",
    description = "Sync",
    branch = branches.sync,
    comparator = objects.syncMonitor
  }

  return controls, views
end

return BLSawOscUnit
