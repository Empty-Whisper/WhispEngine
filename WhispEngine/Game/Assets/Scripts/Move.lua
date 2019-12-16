
-- Move.lua

Move = {}

Move.Variables = {
		-- Public Variables [All must be initialized]
}

local var = Move.Variables

function Move:Start()
	
end

function Move:Update()
	pos = transform.position
	transform:SetPosition3f(0,pos.y+0.1,0)
	--object.transform:SetPositionv(pos)
end








