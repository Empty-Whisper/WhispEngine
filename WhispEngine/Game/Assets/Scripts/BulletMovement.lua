
-- BulletMovement.lua

BulletMovement = {}

BulletMovement.Variables = {
		-- Public Variables that will appear in inspector [All must be initialized]
		speed = 70,
}

local time_disappear = 2
local prepare_timer = 0
function BulletMovement:Start()
	var.prepare_timer = 0


end

function BulletMovement:Update()
	pos = transform.position
	transform:SetPositionv(pos + (transform.forward * (var.speed*Time.deltaTime)))
	prepare_timer = prepare_timer + Time.deltaTime

	if prepare_timer >= time_disappear then
		GameObject.Destroy(object)
	end

end

































