local lib = require "faustian.libfaustian"

local Class = require "Base.Class"
local Encoder = require "Encoder"
--local Fader = require "Unit.ViewControl.Fader"
local GainBias = require "Unit.ViewControl.GainBias"
local Gate = require "Unit.ViewControl.Gate"
local Pitch = require "Unit.ViewControl.Pitch"
local Unit = require "Unit"

local Andromeda = Class {}
Andromeda:include(Unit)

function Andromeda:init(args)
  args.title = "Andromeda Reverb"
  args.mnemonic = "DR"
  Unit.init(self, args)
end

function Andromeda:onLoadGraph(channelCount)
  local rev = self:addObject("rev", lib.Andromeda())

  local mod = self:addObject("mod", app.ParameterAdapter())
  tie(rev, "mod", mod, "Out")
  self:addMonoBranch("mod", mod, "In", mod, "Out")

  local high = self:addObject("HighCut", app.ParameterAdapter())
  local low = self:addObject("LowCut", app.ParameterAdapter())
  local decay = self:addObject("Decay", app.ParameterAdapter())
  local drywet = self:addObject("DryWet", app.ParameterAdapter())

  tie(rev, "HighCut", high, "Out")
  self:addMonoBranch("highcut", high, "In", high, "Out")

  tie(rev, "LowCut", low, "Out")
  self:addMonoBranch("lowcut", low, "In", low, "Out")

  tie(rev, "Decay", decay, "Out")
  self:addMonoBranch("decay", decay, "In", decay, "Out")

  tie(rev, "DryWet", drywet, "Out")
  self:addMonoBranch("drywet", drywet, "In", drywet, "Out")

  connect(rev, "OutL", self, "Out1")
  connect(self, "In1", rev, "InL")

  if channelCount == 1 then
    connect(self, "In1", rev, "InR")
  else
    connect(self, "In2", rev, "InR")
    connect(rev, "OutR", self, "Out2")
  end
end

local views = {
  expanded = {
    "lowcut",
    "highcut",
    "decay",
    "mod",
    "drywet",
  },
  collapsed = {},
}

-- local predelayMap = app.LinearDialMap(0, 100)
-- predelayMap:setSteps(5, 1, 0.5, 0.1)
local fbMap = app.LinearDialMap(0, 5)
fbMap:setSteps(0.1, 0.01, 0.001, 0.0001)

function Andromeda:onLoadViews(objects, branches)
  local controls = {}

  controls.mod = GainBias {
    button = "Mod",
    description = "Mod Rate",
    branch = branches.mod,
    gainbias = objects.mod,
    range = objects.mod,
    biasUnits = app.unitNone,
    biasMap = Encoder.getMap("int[0,100]"),
    initialBias = 50,
  }

  controls.highcut = GainBias {
    button = "High Cut",
    description = "High Cut",
    branch = branches.highcut,
    gainbias = objects.HighCut,
    range = objects.HighCut,
    biasMap = Encoder.getMap("oscFreq"),
    biasUnits = app.unitHertz,
    initialBias = 20000,
    gainMap = Encoder.getMap("freqGain"),
    scaling = app.octaveScaling
  }

  controls.lowcut = GainBias {
    button = "Low Cut",
    description = "Low Cut",
    branch = branches.lowcut,
    gainbias = objects.LowCut,
    range = objects.LowCut,
    biasMap = Encoder.getMap("oscFreq"),
    biasUnits = app.unitHertz,
    initialBias = 20,
    gainMap = Encoder.getMap("freqGain"),
    scaling = app.octaveScaling
  }

  controls.decay = GainBias {
    button = "Decay",
    description = "Decay",
    branch = branches.decay,
    gainbias = objects.Decay,
    range = objects.Decay,
    biasUnits = app.unitNone,
    biasMap = fbMap,
    initialBias = 2,
  }

  controls.drywet = GainBias {
    button = "Wet",
    description = "Wet",
    branch = branches.drywet,
    gainbias = objects.DryWet,
    range = objects.DryWet,
    biasUnits = app.unitNone,
    biasMap = Encoder.getMap("[0,1]"),
    initialBias = 0.25,
  }

  return controls, views
end

return Andromeda
