
-- ShotTank.lua

ShotTank = {}

ShotTank.Variables = {
		-- Public Variables that will appear in inspector [All must be initialized]
		bullet = nil, --[Prefab]
		--bulletParent = nil, --[Prefab]
		speed = 70
}

-- local var = ShotTank.Variables
local shot = true
local prepare_timer = 0
local time_disappear = 2
--public GameObject shell
--public GameObject canon

function ShotTank:Start()
	
end

function ShotTank:Update()
	if input.getKeyDown('space') then
		GameObject.Instantiate(var.bullet)
		--pos = transform.position
		--transform:SetPosition3f(pos.x,pos.y,pos.z)--Set pos = canon position, same for rotation?
		--transform.SetParent(bulletParent)
		--shot = true
	end

	if shot==true then
		--prepare_timer = prepare_timer + Time.deltaTime
		--pos = var.bullet.transform.position
		--transform:SetPositionv(pos + (transform.forward * (var.speed*Time.deltaTime)))
	end

	if prepare_timer >= time_disappear then
		--shot = false
		--prepare_timer = 0
	end

end













