local a = 5
function Start()
	Log(a+3)	
end

function Update()
	if input.getKeyDown("a") then
		Log('pulsaste la a puto')
	end
	if input.getKeyDown('s') then
		Log('pulsaste la s puto')
	end

	if input.getKey('a') then
		Log('aaaaaaaaaaaaa')
	end
	if input.getKey('s') then
		Log('ssssssssssssssssss')
	end

	if input.getKeyUp("a") then
		Log('retiraste la a pendejo')
	end
	if input.getKeyUp('s') then
		Log('rfgasdgfasfas')
	end
end
