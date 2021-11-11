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

  local mod1 = self:addObject("mod1", app.ParameterAdapter())
  tie(rev, "Mod1", mod1, "Out")
  self:addMonoBranch("mod1", mod1, "In", mod1, "Out")

  local mod2 = self:addObject("mod2", app.ParameterAdapter())
  tie(rev, "Mod2", mod2, "Out")
  self:addMonoBranch("mod2", mod2, "In", mod2, "Out")

  local mod3 = self:addObject("mod3", app.ParameterAdapter())
  tie(rev, "Mod3", mod3, "Out")
  self:addMonoBranch("mod3", mod3, "In", mod3, "Out")

  local mod4 = self:addObject("mod4", app.ParameterAdapter())
  tie(rev, "Mod4", mod4, "Out")
  self:addMonoBranch("mod4", mod4, "In", mod4, "Out")

  local high = self:addObject("HighCut", app.ParameterAdapter())
  local low = self:addObject("LowCut", app.ParameterAdapter())
  local decay = self:addObject("Decay", app.ParameterAdapter())
  -- local damping = self:addObject("Damping", app.ParameterAdapter())
  local drywet = self:addObject("DryWet", app.ParameterAdapter())

  tie(rev, "HighCut", high, "Out")
  self:addMonoBranch("highcut", high, "In", high, "Out")

  tie(rev, "LowCut", low, "Out")
  self:addMonoBranch("lowcut", low, "In", low, "Out")

  tie(rev, "Decay", decay, "Out")
  self:addMonoBranch("decay", decay, "In", decay, "Out")

  -- tie(rev, "Damping", damping, "Out")
  -- self:addMonoBranch("damping", damping, "In", damping, "Out")

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
    -- "predelay",
    -- "bandwidth",
    "highcut",
    "lowcut",
    "mod1",
    "mod2",
    "mod3",
    "mod4",
    "decay",
    -- "damping",
    "drywet",
  },
  collapsed = {},
}

-- local predelayMap = app.LinearDialMap(0, 100)
-- predelayMap:setSteps(5, 1, 0.5, 0.1)
local fbMap = app.LinearDialMap(0, 5)
fbMap:setSteps(0.1, 0.01, 0.001, 0.0001)

local modmap = app.LinearDialMap(1, 1000)
modmap:setSteps(100, 10, 5, 1)

function Andromeda:onLoadViews(objects, branches)
  local controls = {}

  controls.mod1 = GainBias {
    button = "mod1",
    description = "mod1",
    branch = branches.mod1,
    gainbias = objects.mod1,
    range = objects.mod1,
    biasUnits = app.unitNone,
    biasMap = modmap,
    initialBias = 130,
  }
  controls.mod2 = GainBias {
    button = "mod2",
    description = "mod2",
    branch = branches.mod2,
    gainbias = objects.mod2,
    range = objects.mod2,
    biasUnits = app.unitNone,
    biasMap = modmap,
    initialBias = 63,
  }
  controls.mod3 = GainBias {
    button = "mod3",
    description = "mod3",
    branch = branches.mod3,
    gainbias = objects.mod3,
    range = objects.mod3,
    biasUnits = app.unitNone,
    biasMap = modmap,
    initialBias = 43,
  }
  controls.mod4 = GainBias {
    button = "mod4",
    description = "mod4",
    branch = branches.mod4,
    gainbias = objects.mod4,
    range = objects.mod4,
    biasUnits = app.unitNone,
    biasMap = modmap,
    initialBias = 20,
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
