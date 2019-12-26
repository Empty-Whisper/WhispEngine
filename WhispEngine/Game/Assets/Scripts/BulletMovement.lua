
-- BulletMovement.lua

BulletMovement = {}

BulletMovement.Variables = {
		-- Public Variables that will appear in inspector [All must be initialized]
		cannon = nil, -- [GameObject]
		prepare_timer = Time.time,
		speed = 70,
}

-- local var = BulletMovement.Variables
local shot = true
local time_disappear = 2

function BulletMovement:Start()
	--GameObject.SetParent(var.cannon)
	transform.position = cannon.transform.position
	transform.rotation = cannon.transform.rotation
	var.prepare_timer = Time.time
	
end

function BulletMovement:Update()

	--prepare_timer = prepare_timer + Time.deltaTime
	Log(var.prepare_timer)
	pos = transform.position
	transform:SetPositionv(pos + (transform.forward * (var.speed*Time.deltaTime)))


	if Time.time - var.prepare_timer >= time_disappear then
		GameObject.Destroy(object)
	end

end























