
-- MoveTank.lua

MoveTank = {}

MoveTank.Variables = {
		-- Public Variables [All must be initialized]
}

local var = MoveTank.Variables
local speed = 15
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
		transform:SetPosition3f(pos.x,pos.y,pos.z-(speed*Time.deltaTime))
		W = true
	else
	W = false
	end

	if input.getKey('a') then
		pos = transform.position
		transform:SetPosition3f(pos.x-(speed*Time.deltaTime),pos.y,pos.z)
	else
	A = false
	end

	if input.getKey('s') then
		pos = transform.position
		transform:SetPosition3f(pos.x,pos.y,pos.z+(speed*Time.deltaTime))
	else
	S = false
	end

	if input.getKey('d') then
		pos = transform.position
		transform:SetPosition3f(pos.x+(speed*Time.deltaTime),pos.y,pos.z)
	else
	D = false
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

--Check if more than 2 key are pressed
	if (W==true and A==true) or (W==true and D==true) or (S==true and A==true) or (A==true and D==true) then
		if input.getKey('w') and input.getKey('d')then
			transform:SetRotation(Quaternion.FromEuler(0,math.rad(315),0))
		end

		if input.getKey('w') and input.getKey('a')then
			transform:SetRotation(Quaternion.FromEuler(0,math.rad(45),0))
		end

		if input.getKey('s') and input.getKey('d')then
			transform:SetRotation(Quaternion.FromEuler(0,math.rad(224),0))
		end

		if input.getKey('s') and input.getKey('a')then
			transform:SetRotation(Quaternion.FromEuler(0,math.rad(135),0))
		end
	end

--Return WASD normal position if was changed
	if not (W==true and A==true) or (W==true and D==true) or (S==true and A==true) or (A==true and D==true) then
		if input.getKey('w') then
			transform:SetRotation(Quaternion.FromEuler(0,math.rad(0),0))
		end

		if input.getKey('a') then
			transform:SetRotation(Quaternion.FromEuler(0,math.rad(90),0))
		end

		if input.getKey('s') then
			transform:SetRotation(Quaternion.FromEuler(0,math.rad(180),0))
		end

		if input.getKey('d') then
			transform:SetRotation(Quaternion.FromEuler(0,math.rad(275),0))
		end
	end

end













