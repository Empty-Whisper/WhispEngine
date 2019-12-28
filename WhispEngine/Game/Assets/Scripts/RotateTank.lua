function Init()
-- RotateTank.lua

local RotateTank = {

	Variables = {
			-- Public Variables that will appear in inspector [All must be initialized]
	},
}

-- Start is called before the first frame update
function RotateTank:Start()
	
end


-- Update is called once per frame
function RotateTank:Update()
		local angle = 0

		if input.getKey("w") then
			if input.getKey("a") then
				angle = 45
			end
			if input.getKey("d") then
				angle = -45
			end

		elseif input.getKey("a") then
			if input.getKey("s") then
				angle = 135
			else
				angle = 90
			end

		elseif input.getKey("s") then
			if input.getKey("d") then
				angle = -135
			else
				angle = -180
			end

		elseif input.getKey("d") then
			angle = -90
		end
	transform:SetRotation(Quaternion.RotateY(math.rad(angle)))
end

return RotateTank

end


