
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
	Log(Time.deltaTime)
end
