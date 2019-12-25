
-- BulletMovement.lua

BulletMovement = {}

BulletMovement.Variables = {
		-- Public Variables that will appear in inspector [All must be initialized]
		speed = 70
}

-- local var = BulletMovement.Variables
local shot = true
local prepare_timer = 0
local time_disappear = 2


function BulletMovement:Start()
	
end

function BulletMovement:Update()

	prepare_timer = prepare_timer + Time.deltaTime
	pos = transform.position
	transform:SetPositionv(pos + (transform.forward * (var.speed*Time.deltaTime)))


	if prepare_timer >= time_disappear then
		prepare_timer = 0
		GameObject.active = false
	end

end















