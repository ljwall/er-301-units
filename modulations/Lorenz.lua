local modulations = require "modulations.libmodulations"

local Class = require "Base.Class"
local Encoder = require "Encoder"
local GainBias = require "Unit.ViewControl.GainBias"
local Unit = require "Unit"

local Lorenz = Class {}
Lorenz:include(Unit)

function Lorenz:init(args)
  args.title = "Lorenz"
  args.mnemonic = "Lo"
  Unit.init(self, args)
end

function Lorenz:onLoadGraph(channelCount)
  local lorenz = self:addObject("logisticMap", modulations.Lorenz())
  local rate = self:addObject("rate", app.ParameterAdapter())
  local rho = self:addObject("rho", app.ParameterAdapter())
  local sigma = self:addObject("sigma", app.ParameterAdapter())
  local beta = self:addObject("beta", app.ParameterAdapter())

  tie(lorenz, "Rate", rate, "Out")
  self:addMonoBranch("rate", rate, "In", rate, "Out")

  tie(lorenz, "Rho", rho, "Out")
  self:addMonoBranch("rho", rho, "In", rho, "Out")

  tie(lorenz, "Sigma", sigma, "Out")
  self:addMonoBranch("sigma", sigma, "In", sigma, "Out")

  tie(lorenz, "Beta", beta, "Out")
  self:addMonoBranch("beta", beta, "In", beta, "Out")

  connect(lorenz, "Out", self, "Out1")

  if channelCount > 1 then
    connect(lorenz, "Out", self, "Out2")
  end
end

local views = {
  expanded = {
    "rate",
    "rho",
    "sigma",
    "beta",
  },
  collapsed = {}
}

local map100 = app.LinearDialMap(0, 100)
map100:setSteps(10, 5, 0.5, 0.1)

local map10 = app.LinearDialMap(0, 10)
map10:setSteps(1, 0.5, 0.1, 0.025)

function Lorenz:onLoadViews(objects, branches)
  local controls = {}

  controls.rate = GainBias {
    button = "Rate",
    description = "Rate",
    branch = branches.rate,
    gainbias = objects.rate,
    range = objects.rate,
    biasUnits = app.unitNone,
    biasMap = Encoder.getMap("[-5,5]"),
    initialBias = 0,
  }

  controls.rho = GainBias {
    button = "Rho",
    description = "Rho",
    branch = branches.rho,
    gainbias = objects.rho,
    range = objects.rho,
    biasUnits = app.unitNone,
    biasMap = map100,
    initialBias = 28,
  }

  controls.sigma = GainBias {
    button = "Sigma",
    description = "Sigma",
    branch = branches.sigma,
    gainbias = objects.sigma,
    range = objects.sigma,
    biasUnits = app.unitNone,
    biasMap = map100,
    initialBias = 10,
  }

  controls.beta = GainBias {
    button = "Beta",
    description = "Beta",
    branch = branches.beta,
    gainbias = objects.beta,
    range = objects.beta,
    biasUnits = app.unitNone,
    biasMap = map10,
    initialBias = 8/3,
  }

  return controls, views
end

return Lorenz
