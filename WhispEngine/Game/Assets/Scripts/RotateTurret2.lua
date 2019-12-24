
-- RotateTurret2.lua

RotateTurret2 = {}

RotateTurret2.Variables = {
		-- Public Variables that will appear in inspector [All must be initialized]
		mult = 0.02
}

-- Start is called before the first frame update
function RotateTurret2:Start()
	
end

local mx = 0
local lmx = 0

-- Update is called once per frame
function RotateTurret2:Update()
	mx = input.getMouseX()
	transform:SetRotation(Quaternion.RotateY((lmx-mx)*var.mult))
end

