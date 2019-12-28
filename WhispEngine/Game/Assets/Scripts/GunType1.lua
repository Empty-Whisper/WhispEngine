function Init()
-- GunType1.lua

local GunType1 = {

	Variables = {
			-- Public Variables that will appear in inspector [All must be initialized]
			--[[
			to access this variables use var.variable, for example:
		-- name = 'John'
			to access: Log(var.name) --> John
			]]
	},
}

local num = 0
-- Start is called before the first frame update
function GunType1:Start()
	num = Random.Rangef(1, 4)
	transform:SetScale(num, num, num)
	Log(num)
end


-- Update is called once per frame
function GunType1:Update()

end

return GunType1

end









