local chaos = require "chaoticmods.libchaoticmods"

local Class = require "Base.Class"
local GainBias = require "Unit.ViewControl.GainBias"
local Gate = require "Unit.ViewControl.Gate"
local Unit = require "Unit"

local LogisticMapUnit = Class {}
LogisticMapUnit:include(Unit)

function LogisticMapUnit:init(args)
  args.title = "Logistic Map"
  args.mnemonic = "LM"
  Unit.init(self, args)
end

function LogisticMapUnit:onLoadGraph(channelCount)
  local clockComparator = self:addObject("clockComparator", app.Comparator())
  clockComparator:setTriggerMode()

  local logisticMap = self:addObject("logisticMap", chaos.LogisticMap())

  connect(clockComparator, "Out", logisticMap, "Trigger")
  connect(logisticMap, "Out", self, "Out1")

  self:addMonoBranch("clock", clockComparator, "In", clockComparator, "Out")

  local r = self:addObject("r", app.ParameterAdapter())
  tie(logisticMap, "R", r, "Out")
  self:addMonoBranch("r", r, "In", r, "Out")

  if channelCount > 1 then
    connect(logisticMap, "Out", self, "Out2")
  end
end

local views = {
  expanded = {
    "clock",
    "r",
  },
  collapsed = {}
}

local map = app.LinearDialMap(0, 4)
map:setSteps(0.1, 0.05, 0.01, 0.001)

function LogisticMapUnit:onLoadViews(objects, branches)
  local controls = {}

  controls.clock = Gate {
    button = "clock",
    description = "Clock Input",
    branch = branches.clock,
    comparator = objects.clockComparator,
  }

  controls.r = GainBias {
    button = "r",
    description = "r parameter",
    branch = branches.r,
    gainbias = objects.r,
    range = objects.r,
    biasUnits = app.unitNone,
    biasMap = map,
    initialBias = 3.95,
  }

  return controls, views
end

return LogisticMapUnit
