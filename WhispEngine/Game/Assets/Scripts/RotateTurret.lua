
-- RotateTurret.lua

RotateTurret = {}

RotateTurret.Variables = {
		-- Public Variables that will appear in inspector [All must be initialized]
		--[[
		to access this variables use var.variable, for example:
	 -- name = 'John'
		to access: var.name
		]]
}

-- local var = RotateTurret.Variables

local angle = 0.0
local speed = 5

function RotateTurret:Start()
	
end

function RotateTurret:Update()

	if input.getKey('q') then
		angle = angle + speed
		--transform:SetRotation(Quaternion.RotateZ(math.rad(angle)))
		transform:SetRotation(Quaternion.FromEuler(math.rad(-90), 0, math.rad(angle)))

	end

	if input.getKey('e') then
		angle = angle - speed
		--transform:SetRotation(Quaternion.RotateZ(math.rad(angle)))
		transform:SetRotation(Quaternion.FromEuler(math.rad(-90), 0, math.rad(angle)))
	end
	
	
end






