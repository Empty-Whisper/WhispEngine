
-- RotateTank.lua

RotateTank = {}

RotateTank.Variables = {
		-- Public Variables that will appear in inspector [All must be initialized]
		--[[
		to access this variables use var.variable, for example:
	 -- name = 'John'
		to access: var.name
		]]
}

-- Start is called before the first frame update
function RotateTank:Start()
	
end


-- Update is called once per frame
function RotateTank:Update()
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
	if not ((W==true and A==true) or (W==true and D==true) or (S==true and A==true) or (A==true and D==true)) then --Put and gameObject.transform.eulerAngles == new Vector3(0, 135, 0)   
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

