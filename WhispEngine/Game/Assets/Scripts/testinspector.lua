
-- testinspector.lua

testinspector = {}

testinspector.Variables = {
		-- Public Variables [All must be initialized]
		integer = 6,
		float = 3.315,
		boolean = true,
		name = 'Tina',
		gameObject = nil,
		vector = Vector3(1,0,0)
		-- _table = {
}

--local var = testinspector.Variables

function testinspector:Start()
	
end

function testinspector:Update()

	if input.getKeyDown('s') then
		Log(var.integer)
		Log(tostring(var.boolean))
	end
end






















