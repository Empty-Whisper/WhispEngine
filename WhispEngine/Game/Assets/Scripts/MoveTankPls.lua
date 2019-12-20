
-- MoveTankPls.lua

MoveTankPls = {}

MoveTankPls.Variables = {
		-- Public Variables that will appear in inspector [All must be initialized]
		--[[
		to access this variables use var.variable, for example:
	 -- name = 'John'
		to access: var.name
		]]
}

-- local var = MoveTankPls.Variables

function MoveTankPls:Start()

end

	
end

function MoveTankPls:Update()
--Log(Time.deltaTime)
	if input.getKey('w') then
		pos = transform.position
		transform:SetPosition3f(pos.x,pos.y,pos.z+(speed*Time.deltaTime))
	end

	if input.getKey('a') then
		pos = transform.position
		transform:SetPosition3f(pos.x-(speed*Time.deltaTime),pos.y,pos.z)
	end

	if input.getKey('s') then
		pos = transform.position
		transform:SetPosition3f(pos.x,pos.y,pos.z-(speed*Time.deltaTime))
	end

	if input.getKey('d') then
		pos = transform.position
		transform:SetPosition3f(pos.x+(speed*Time.deltaTime),pos.y,pos.z)
	end
	
	
	
end


	
end


