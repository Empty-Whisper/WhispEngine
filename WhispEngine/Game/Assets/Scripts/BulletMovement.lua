
-- BulletMovement.lua

BulletMovement = {}

BulletMovement.Variables = {
		-- Public Variables that will appear in inspector [All must be initialized]
		prepare_timer = Time.time,
		speed = 70,
}

local time_disappear = 2

function BulletMovement:Start()
	var.prepare_timer = Time.time
end

function BulletMovement:Update()
	pos = transform.position
	transform:SetPositionv(pos + (transform.forward * (var.speed*Time.deltaTime)))


	if Time.time - var.prepare_timer >= time_disappear then
		GameObject.Destroy(object)
	end

end


























