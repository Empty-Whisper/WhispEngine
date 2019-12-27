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
}

-- Start is called before the first frame update
function Model:Start()
	
end


-- Update is called once per frame
function Model:Update()

end

return Model

end