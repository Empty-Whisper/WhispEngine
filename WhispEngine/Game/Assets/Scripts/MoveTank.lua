
-- MoveTank.lua

MoveTank = {}

MoveTank.Variables = {
		-- Public Variables [All must be initialized]
		speed = 7.1 --[Slider(0, 10)]
}

local W = false
local A = false
local S = false
local D = false

function MoveTank:Start()
	
end

function MoveTank:Update()
--Movement
	if input.getKey('w') then
		pos = transform.position
		--transform:SetPosition3f(pos.x,pos.y,pos.z-(var.speed*Time.deltaTime))
		transform:SetPosition3f(transform.forward * (var.speed*Time.deltaTime))
		W = true
	else
	W = false
	end

	if input.getKey('a') then
		pos = transform.position
		transform:SetPosition3f(pos.x-(var.speed*Time.deltaTime),pos.y,pos.z)
	else
	A = false
	end

	if input.getKey('s') then
		pos = transform.position
		transform:SetPosition3f(pos.x,pos.y,pos.z+(var.speed*Time.deltaTime))
	else
	S = false
	end

	if input.getKey('d') then
		pos = transform.position
		transform:SetPosition3f(pos.x+(var.speed*Time.deltaTime),pos.y,pos.z)
	else
	D = false
	end
end






















