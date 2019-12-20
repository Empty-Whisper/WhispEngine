
-- MoveTank.lua

MoveTank = {}

MoveTank.Variables = {
		-- Public Variables [All must be initialized]
}

local var = MoveTank.Variables
local speed = 15

function MoveTank:Start()
	
end

function MoveTank:Update()
--Log(Time.deltaTime)
	if input.getKey('w') then
		pos = transform.position
		transform:SetPosition3f(pos.x,pos.y+(speed*Time.deltaTime),pos.z)
	end

	if input.getKey('a') then
		pos = transform.position
		transform:SetPosition3f(pos.x-(speed*Time.deltaTime),pos.y,pos.z)
	end

	if input.getKey('s') then
		pos = transform.position
		transform:SetPosition3f(pos.x,pos.y-(speed*Time.deltaTime),pos.z)
	end

	if input.getKey('d') then
		pos = transform.position
		transform:SetPosition3f(pos.x+(speed*Time.deltaTime),pos.y,pos.z)
	end
	
	
	
end




