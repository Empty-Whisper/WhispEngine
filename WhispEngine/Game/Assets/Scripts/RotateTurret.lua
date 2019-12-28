function Init()
	-- RotateTurret.lua

	RotateTurret = {}

	RotateTurret.Variables = {
		-- Public Variables that will appear in inspector [All must be initialized]
		mult = 0.01 --[Slider(0, 0.02)]
	}

	-- Start is called before the first frame update
	function RotateTurret:Start()
	end

	local mx = 0
	local lmx = 0

	-- Update is called once per frame
	function RotateTurret:Update()
		mx = input.getMouseX()
		transform:SetRotation(Quaternion.RotateY((lmx - mx) * var.mult))
	end

	return RotateTurret
end
