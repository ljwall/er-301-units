local lib = require "faust_poc.libfaustpoc"
local Class = require "Base.Class"
local Unit = require "Unit"
local Pitch = require "Unit.ViewControl.Pitch"
local GainBias = require "Unit.ViewControl.GainBias"
local Gate = require "Unit.ViewControl.Gate"
local Encoder = require "Encoder"

local Dattorro = Class {}
Dattorro:include(Unit)

function Dattorro:init(args)
  args.title = "Dattorro Reverb"
  args.mnemonic = "DR"
  Unit.init(self, args)
end

function Dattorro:onLoadGraph(channelCount)
  local rev = self:addObject("rev", lib.Dattorro())

  local bandwidth = self:addObject("BandWidth", app.ParameterAdapter())
  local decay = self:addObject("Decay", app.ParameterAdapter())
  local damping = self:addObject("Damping", app.ParameterAdapter())
  local drywet = self:addObject("DryWet", app.ParameterAdapter())

  tie(rev, "BandWidth", bandwidth, "Out")
  self:addMonoBranch("bandwidth", bandwidth, "In", bandwidth, "Out")

  tie(rev, "Decay", decay, "Out")
  self:addMonoBranch("decay", decay, "In", decay, "Out")

  tie(rev, "Damping", damping, "Out")
  self:addMonoBranch("damping", damping, "In", damping, "Out")

  tie(rev, "DryWet", drywet, "Out")
  self:addMonoBranch("drywet", drywet, "In", drywet, "Out")

  connect(rev, "OutL", self, "Out1")
  connect(self, "In1", rev, "InL")

  if channelCount > 1 then
    connect(rev, "OutR", self, "Out2")
    connect(self, "In1", rev, "InR")
  end
end

local views = {
  expanded = {
    "bandwidth",
    "decay",
    "damping",
    "drywet",
  },
  collapsed = {},
}

function Dattorro:onLoadViews(objects, branches)
  local controls = {}

  controls.bandwidth = GainBias {
    button = "bandwidth",
    description = "bandwidth",
    branch = branches.bandwidth,
    gainbias = objects.BandWidth,
    range = objects.BandWidth,
    biasUnits = app.unitNone,
    biasMap = Encoder.getMap("[0,1]"),
    initialBias = 0.6,
  }

  controls.decay = GainBias {
    button = "decay",
    description = "decay",
    branch = branches.decay,
    gainbias = objects.Decay,
    range = objects.Decay,
    biasUnits = app.unitNone,
    biasMap = Encoder.getMap("[0,1]"),
    initialBias = 0.8,
  }

  controls.damping = GainBias {
    button = "damping",
    description = "damping",
    branch = branches.damping,
    gainbias = objects.Damping,
    range = objects.Damping,
    biasUnits = app.unitNone,
    biasMap = Encoder.getMap("[0,1]"),
    initialBias = 0.25,
  }

  controls.drywet = GainBias {
    button = "drywet",
    description = "drywet",
    branch = branches.drywet,
    gainbias = objects.DryWet,
    range = objects.DryWet,
    biasUnits = app.unitNone,
    biasMap = Encoder.getMap("[0,1]"),
    initialBias = 0.25,
  }

  return controls, views
end

return Dattorro
