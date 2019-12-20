
-- Rotate.lua

Rotate = {}

Rotate.Variables = {
		-- Public Variables that will appear in inspector [All must be initialized]
		speed = 1
}

-- local var = Rotate.Variables

function Rotate:Start()
	
end

local angle = 0.0
local up = Vector3(0,1,0)

function Rotate:Update()
	transform:SetRotation(Quaternion.FromEuler(0, math.rad(angle), 0))
	angle = angle + 1 * var.speed * Time.deltaTime
end




