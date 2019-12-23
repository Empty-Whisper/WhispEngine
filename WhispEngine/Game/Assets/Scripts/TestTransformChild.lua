
-- TestTransformChild.lua

TestTransformChild = {}

TestTransformChild.Variables = {
		-- Public Variables that will appear in inspector [All must be initialized]
		--[[
		to access this variables use var.variable, for example:
	 -- name = 'John'
		to access: var.name
		]]
}

-- Start is called before the first frame update
function TestTransformChild:Start()
	
end


-- Update is called once per frame
function TestTransformChild:Update()
	if input.getKeyDown('s') then
		Log('num children: ' .. transform.ChildCount)
		Log('finding TankChassis...')
		local chassis = transform:Find('TankChassis')
		Log(chassis.name)
		Log('gameobject with index 1 must be TankTracksRight, and is: ' .. transform:GetChild(1).name)
		if transform:IsChildOf(transform.parent.transform) then
			Log(transform.gameobject.name .. ' is child of ' .. transform.parent.name)
		end
		transform:SetParent(transform.parent.transform:GetChild(0).transform)
	end
end



