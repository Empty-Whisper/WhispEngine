
-- BulletMovement.lua

BulletMovement = {}

BulletMovement.Variables = {
		-- Public Variables that will appear in inspector [All must be initialized]
		cannon = nil, -- [GameObject]
		speed = 70
}

-- local var = BulletMovement.Variables
local shot = true
local prepare_timer = 0
local time_disappear = 2


function BulletMovement:Start()
	--GameObject.SetParent(var.cannon)
	transform.position = cannon.transform.position
	transform.rotation = cannon.transform.rotation
	
end

function BulletMovement:Update()

	prepare_timer = prepare_timer + Time.deltaTime
	pos = transform.position
	transform:SetPositionv(pos + (transform.forward * (var.speed*Time.deltaTime)))


	if prepare_timer >= time_disappear then
		GameObject.Destroy(object)
	end

end




















