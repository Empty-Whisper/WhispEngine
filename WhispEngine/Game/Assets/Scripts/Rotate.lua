
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
	Log('soy el start ajaj salu2')
end

local angle = 0.0
local up = Vector3(0,1,0)

function Rotate:Update()
	--[[transform:SetRotation(Quaternion.FromEuler(0, math.rad(angle), 0))
	angle = angle + 10 * Time.deltaTime
	]]
	Log('up')
end



