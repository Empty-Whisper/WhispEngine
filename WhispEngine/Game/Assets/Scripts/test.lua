
function Start()
	Log(a+3)	
end

function Update()
local a = 10
	if input.getKeyDown("a") then
		Log('El valor es ' .. a .. ' capichi?')
	end
	for i=0, 5, 1 do
		if input.getMouseDown(i) then
			Log(i)
		end
	end
	Log(input.getMouseX() .. ' ' .. input.getMouseY())
end
