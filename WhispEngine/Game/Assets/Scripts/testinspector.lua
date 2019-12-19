
-- testinspector.lua

testinspector = {}

testinspector.Variables = {
		-- Public Variables [All must be initialized]
		a = 5,
	--	b = 3,
--jonnnnnnnnn = 2,
--jajajaj = 'stringgggggg',
--nope = 09
}

--local var = testinspector.Variables

function testinspector:Start()
	
end

function testinspector:Update()

--	if input.getKeyDown('s') then
		Log(var.a)
--	end
end















