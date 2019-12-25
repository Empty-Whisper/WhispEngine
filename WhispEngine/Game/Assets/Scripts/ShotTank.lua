
-- ShotTank.lua

ShotTank = {}

ShotTank.Variables = {
		-- Public Variables that will appear in inspector [All must be initialized]
		bullet = nil, --[Prefab]
}

-- local var = ShotTank.Variables
local speed = 20
local shot = true
local prepare_timer = 0
local time_disappear = 2
--public GameObject shell
--public GameObject canon

function ShotTank:Start()
	
end

function ShotTank:Update()

	if input.getKeyDown('r') then
		--SetActive(true)
		pos = transform.position
		transform:SetPosition3f(pos.x,pos.y,pos.z)--Set pos = canon position, same for rotation?
		--SetParent(spawnerBullets)
		shot = true
	end

	if shot==true then
		prepare_timer = prepare_timer + Time.deltaTime
		--Translate forward
	end

	if prepare_timer >= time_disappear then
		--SetActive(false)
		shot = false
		prepare_timer = 0
	end
end





