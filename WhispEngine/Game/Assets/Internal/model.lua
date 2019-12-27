function Init()
-- Model.lua

local Model = {

	Variables = {
			-- Public Variables that will appear in inspector [All must be initialized]
			--[[
			to access this variables use var.variable, for example:
		-- name = 'John'
			to access: Log(var.name) --> John
			]]
	},
	
	speed = 5
}

-- Start is called before the first frame update
function Model:Start()
	
end


-- Update is called once per frame
function Model:Update()
	Model.speed = Model.speed+1
	Log(Model.speed)
end

return Model
end