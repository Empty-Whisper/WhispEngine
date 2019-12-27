function Init()
-- BulletMovement.lua

BulletMovement = {}

BulletMovement.Variables = {
		-- Public Variables that will appear in inspector [All must be initialized]
		speed = 10,
}

local prepare_timer = Time.time
local shot = true
local time_disappear = 0.5

function BulletMovement:Start()
	prepare_timer = Time.time
	
end

function BulletMovement:Update()
	local pos = transform.position
	transform:SetPositionv(pos + (transform.forward * (var.speed*Time.deltaTime)))

	if Time.time - prepare_timer >= time_disappear then
		GameObject.Destroy(object)
	end

end

return BulletMovement

end

