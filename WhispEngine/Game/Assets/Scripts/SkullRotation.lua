function Init()
	-- SkullRotation.lua

	SkullRotation = {}

	SkullRotation.Variables = {
		-- Public Variables that will appear in inspector [All must be initialized]
		speed = 0.1 --[Slider(0, 0.02)]
	}

	local mult = 0
	-- Start is called before the first frame update
	function SkullRotation:Start()
		mult = 0
	end

	-- Update is called once per frame
	function SkullRotation:Update()
		mult = mult - var.speed
		transform:SetRotation(Quaternion.RotateY(mult))
	end

	return SkullRotation
end






