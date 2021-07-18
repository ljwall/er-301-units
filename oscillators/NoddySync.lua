local app = app
local Class = require "Base.Class"
local ViewControl = require "Unit.ViewControl"
local ply = app.SECTION_PLY

local col1 = app.BUTTON1_CENTER
local line4 = app.GRID5_LINE4

local NoddySync = Class {}
NoddySync:include(ViewControl)

function NoddySync:init(args)
  local comparator = args.comparator or
                        app.logError("%s.init: comparator is missing.", self)

  local button = args.button or app.logError("%s.init: button is missing.", self)
  local description = args.description or
                          app.logError("%s.init: description is missing.", self)
  local branch = args.branch or app.logError("%s.init: branch is missing.", self)
  self.branch = branch

  ViewControl.init(self, button)
  self:setClassName("NoddySync")

  local graphic = app.ComparatorView(0, 0, ply, 64, comparator)

  graphic:setLabel(button)
  graphic:setUnits(app.unitHertz)
  graphic:setPrecision(3)
  self.comparatorView = graphic
  self:setMainCursorController(graphic)
  self:setControlGraphic(graphic)
  self:addSpotDescriptor{
    center = 0.5 * ply
  }

  self.subGraphic = app.Graphic(0, 0, 128, 64)
  self.scope = app.MiniScope(col1 - 20, line4, 40, 45)
  self.scope:setBorder(1)
  self.scope:setCornerRadius(3, 3, 3, 3)
  self.subGraphic:addChild(self.scope)

  graphic = app.SubButton("empty", 1)
  self.subGraphic:addChild(graphic)
  self.modButton = graphic

  branch:subscribe("contentChanged", self)
end

function NoddySync:onRemove()
  self.branch:unsubscribe("contentChanged", self)
  ViewControl.onRemove(self)
end

function NoddySync:contentChanged(chain)
  if chain == self.branch then
    local outlet = chain:getMonitoringOutput(1)
    self.scope:watchOutlet(outlet)
    self.modButton:setText(chain:mnemonic())
  end
end

function NoddySync:subReleased(i, shifted)
  if shifted or i > 1 then return false end

  local branch = self.branch
  self:unfocus()
  self.branch:show()

end

return NoddySync
