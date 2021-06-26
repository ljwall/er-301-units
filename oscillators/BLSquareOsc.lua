local blOsc = require "bloscillators.libbloscillators"
local Class = require "Base.Class"
local Unit = require "Unit"

local BLSquareOscUnit = Class {}
BLSquareOscUnit:include(Unit)

function BLSquareOscUnit:init(args)
  args.title = "BL Square"
  args.mnemonic = "SQ"
  Unit.init(self, args)
end

function BLSquareOscUnit:onLoadGraph(channelCount)
  local osc = self:addObject("osc", blOsc.BLSquareOsc())

  connect(osc, "Out", self, "Out1")
  if channelCount > 1 then
    connect(osc, "Out", self, "Out2")
  end
end

local views = {
  expanded = {},
  collapsed = {},
}

function BLSquareOscUnit:onLoadViews(objects, branches)
  return {}, views
end

return BLSquareOscUnit
