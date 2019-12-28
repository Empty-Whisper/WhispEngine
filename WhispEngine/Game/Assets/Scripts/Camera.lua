function Init()
	-- Camera.lua

	Camera = {}

	Camera.Variables = {
		-- Public Variables that will appear in inspector [All must be initialized]
		tank = nil, --[GameObject]
		offset_thirdp = 0 --[Slider(-10,10)]
	}

	local diff
	-- Start is called before the first frame update
	function Camera:Start()
		diff = transform.position - var.tank.transform.position
	end

	-- Update is called once per frame
	function Camera:Update()
		--Log(diff.x::toString())
		transform:SetPositionv(var.tank.transform.position + diff)
	 --(diff * var.offset_thirdp))
	end

	return Camera
end
