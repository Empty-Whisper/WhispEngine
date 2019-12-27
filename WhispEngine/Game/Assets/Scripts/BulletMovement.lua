
-- BulletMovement.lua

BulletMovement = {}

BulletMovement.Variables = {
		-- Public Variables that will appear in inspector [All must be initialized]
		prepare_timer = Time.time,
		speed = 10,
}


local shot = true
local time_disappear = 0.5

function BulletMovement:Start()
	var.prepare_timer = Time.time
	
end

function BulletMovement:Update()
	Log(var.prepare_timer)
	local pos = transform.position
	transform:SetPositionv(pos + (transform.forward * (var.speed*Time.deltaTime)))

	--Log(var.prepare_timer)
	--Log(Time.time)
	if Time.time - var.prepare_timer >= time_disappear then
		GameObject.Destroy(object)
	end

end


















































