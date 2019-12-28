function Init()
	-- BulletMovement.lua

	local BulletMovement = {}

	BulletMovement.Variables = {
		-- Public Variables that will appear in inspector [All must be initialized]
		speed = 10 --[Slider(10,50)]
	}

	local prepare_timer = 0
	local time_disappear = 2

	function BulletMovement:Start()
		prepare_timer = Time.time
	end

	function BulletMovement:Update()
		local pos = transform.position
		transform:SetPositionv(pos + (transform.forward * (var.speed * Time.deltaTime)))

		if Time.time - prepare_timer >= time_disappear then
			GameObject.Destroy(gameObject)
		end
	end

	return BulletMovement
end



