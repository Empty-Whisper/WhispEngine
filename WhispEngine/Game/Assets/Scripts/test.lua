
-- Model.lua

Model = {}

Model.Variables = {
		-- Public Variables [All must be initialized]
		intTest = 4,
		floatTest = 2.578,
		stringTest = "jajajajaja",
		boolTest = false,
		arrayTest = {},
		gameObjectTest = "JAJAJAJAJAJAJJAJA",
}

local var = Model.Variables
transform = GameObject

function Model:Start()

end

function Model:Update()
	vec = Vector3(0,0,0)
	Log(vec:toString())
	vec.x = 9
	vec.y = 1
	vec.z = 1
	Log(vec:toString())
end
