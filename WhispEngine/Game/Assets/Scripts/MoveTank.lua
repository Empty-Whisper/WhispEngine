
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
--Movement
	if input.getKey('w') then
		pos = transform.position
		transform:SetPosition3f(pos.x,pos.y,pos.z-(speed*Time.deltaTime))
	end

	if input.getKey('a') then
		pos = transform.position
		transform:SetPosition3f(pos.x-(speed*Time.deltaTime),pos.y,pos.z)
	end

	if input.getKey('s') then
		pos = transform.position
		transform:SetPosition3f(pos.x,pos.y,pos.z+(speed*Time.deltaTime))
	end

	if input.getKey('d') then
		pos = transform.position
		transform:SetPosition3f(pos.x+(speed*Time.deltaTime),pos.y,pos.z)
	end
	
--ChangeDirection
	if input.getKeyDown('w') then
		transform:SetRotation(Quaternion.FromEuler(0,math.rad(0),0))
	end

	if input.getKeyDown('a') then
		transform:SetRotation(Quaternion.FromEuler(0,math.rad(90),0))
	end

	if input.getKeyDown('s') then
		transform:SetRotation(Quaternion.FromEuler(0,math.rad(180),0))
	end

	if input.getKeyDown('d') then
		transform:SetRotation(Quaternion.FromEuler(0,math.rad(275),0))
	end



end











