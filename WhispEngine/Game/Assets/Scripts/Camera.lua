
-- Camera.lua

Camera = {}

Camera.Variables = {
		-- Public Variables that will appear in inspector [All must be initialized]
		tank = nil, --[GameObject]
}

local diff
-- Start is called before the first frame update
function Camera:Start()
	diff = transform.position - var.tank.transform.position
end

local up = Vector3(0,1,0)
-- Update is called once per frame
function Camera:Update()
	--transform:SetPositionv(var.tank.transform.position + diff)
	--transform:LookAt(var.tank.transform.position)
end






