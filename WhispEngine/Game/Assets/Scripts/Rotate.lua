
-- Rotate.lua

Rotate = {}

Rotate.Variables = {
		-- Public Variables that will appear in inspector [All must be initialized]
		--[[
		to access this variables use var.variable, for example:
	 -- name = 'John'
		to access: var.name
		]]
}

-- local var = Rotate.Variables

function Rotate:Start()
	
end

local angle = 0.0
local up = Vector3(0,1,0)
function Rotate:Update()
	--Log(Quaternion.RotateAxisAngle(up, math.rad(angle)):ToString())
	--Log(up:toString())
	--transform:SetRotation(Quaternion.RotateAxisAngle(Vector3(0.0,1.0,0.0), math.rad(angle)))
	transform:SetRotation(Quaternion.RotateY(math.rad(angle)))
	angle = angle + 10 * Time.deltaTime
	--Log(angle)
end
