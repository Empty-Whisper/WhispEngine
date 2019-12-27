function Init()
-- ShotTank.lua

ShotTank = {}

ShotTank.Variables = {
		-- Public Variables that will appear in inspector [All must be initialized]
		bullet = nil, --[Prefab]
		speed = 70
}

-- local var = ShotTank.Variables
local shot = true
local prepare_timer = 0
local time_disappear = 2
--public GameObject shell
--public GameObject canon

function ShotTank:Start()
	
end

function ShotTank:Update()
	if input.getMouseDown(1) then
		instance = GameObject.Instantiate(var.bullet)
		pos = transform.gposition
		instance.transform:SetPosition3f(pos.x,pos.y,pos.z)--Set pos = canon position, same for rotation?
		instance.transform:SetRotation(transform.grotation)
	end

end

return ShotTank

end
