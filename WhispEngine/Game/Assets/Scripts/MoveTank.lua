
-- MoveTank.lua

MoveTank = {}

MoveTank.Variables = {
		-- Public Variables [All must be initialized]
}

local var = MoveTank.Variables
local speed = 8

function MoveTank:Start()
	
end

function MoveTank:Update()

	if input.getKey('w') then
		pos = transform.position
		transform:SetPosition3f(pos.x,pos.y+0.1,pos.z)
	end

	if input.getKey('a') then
		pos = transform.position
		transform:SetPosition3f(pos.x-0.1,pos.y,pos.z)
	end

	if input.getKey('s') then
		pos = transform.position
		transform:SetPosition3f(pos.x,pos.y-0.1,pos.z)
	end

	if input.getKey('d') then
		pos = transform.position
		transform:SetPosition3f(pos.x+0.1,pos.y,pos.z)
	end
	
	
	
end


