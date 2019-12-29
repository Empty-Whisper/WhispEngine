function Init()
	-- BulletRotation.lua

	BulletRotation = {}

	BulletRotation.Variables = {
		-- Public Variables that will appear in inspector [All must be initialized]
		speed = 0.1 --[Slider(0, 0.02)]
	}

	local mult = 0
	--Random vars
	local r = 0
	local g = 0
	local b = 0
	local first = 0
	local second = 0
	local first_color = 0
	local second_color = 0
	-- Start is called before the first frame update
	function BulletRotation:Start()
		--Se viene
		first = Random.Range(1,4)

		if first == 1 then
			first_color = Random.Range(1,4)
			if first_color == 1 then
				 r = 0
				second = Random.Range(1,3)
				if second == 1 then
					second_color = Random.Range(1,3)
					if second_color == 1 then
						g = 1
						b = Random.Rangef(0,1)
					end
					if second_color == 2 then
						g = Random.Rangef(0,1)
						b = 1
					end
				end
				if second == 2 then
					second_color = Random.Range(1,3)
					if second_color == 1 then
						b = 1
						g = Random.Rangef(0,1)
					end
					if second_color == 2 then
						b = Random.Rangef(0,1)
						g = 1
					end
					
				end
			end


			if first_color == 2 then
				 r = 1
				second = Random.Range(1,3)
				if second == 1 then
					second_color = Random.Range(1,3)
					if second_color == 1 then
						g = 0
						b = Random.Rangef(0,1)
					end
					if second_color == 2 then
						g = Random.Rangef(0,1)
						b = 0
					end
				end
				if second == 2 then
					second_color = Random.Range(1,3)
					if second_color == 1 then
						b = 0
						g = Random.Rangef(0,1)
					end
					if second_color == 2 then
						b = Random.Rangef(0,1)
						g = 0
					end
					
				end
			end


			if first_color == 3 then
				 r = Random.Rangef(0,1)
				second = Random.Range(1,3)
				if second == 1 then
					second_color = Random.Range(1,3)
					if second_color == 1 then
						g = 0
						b = 1
					end
					if second_color == 2 then
						g = 1
						b = 0
					end
				end
				if second == 2 then
					second_color = Random.Range(1,3)
					if second_color == 1 then
						b = 0
						g = 1
					end
					if second_color == 2 then
						b = 1
						g = 0
					end
					
				end
			end
		end


		if first == 2 then
			first_color = Random.Range(1,4)
			if first_color == 1 then
				 g = 0
				second = Random.Range(1,3)
				if second == 1 then
					second_color = Random.Range(1,3)
					if second_color == 1 then
						r = 1
						b = Random.Rangef(0,1)
					end
					if second_color == 2 then
						r = Random.Rangef(0,1)
						b = 1
					end
				end
				if second == 2 then
					second_color = Random.Range(1,3)
					if second_color == 1 then
						b = 1
						r = Random.Rangef(0,1)
					end
					if second_color == 2 then
						b = Random.Rangef(0,1)
						r = 1
					end
					
				end
			end


			if first_color == 2 then
				 g = 1
				second = Random.Range(1,3)
				if second == 1 then
					second_color = Random.Range(1,3)
					if second_color == 1 then
						r = 0
						b = Random.Rangef(0,1)
					end
					if second_color == 2 then
						r = Random.Rangef(0,1)
						b = 0
					end
				end
				if second == 2 then
					second_color = Random.Range(1,3)
					if second_color == 1 then
						b = 0
						r = Random.Rangef(0,1)
					end
					if second_color == 2 then
						b = Random.Rangef(0,1)
						r = 0
					end
					
				end
			end


			if first_color == 3 then
				 g = Random.Rangef(0,1)
				second = Random.Range(1,3)
				if second == 1 then
					second_color = Random.Range(1,3)
					if second_color == 1 then
						r = 0
						b = 1
					end
					if second_color == 2 then
						r = 1
						b = 0
					end
				end
				if second == 2 then
					second_color = Random.Range(1,3)
					if second_color == 1 then
						b = 0
						r = 1
					end
					if second_color == 2 then
						b = 1
						r = 0
					end
					
				end
			end
		end




		if first == 3 then
			first_color = Random.Range(1,4)
			if first_color == 1 then
				 b = 0
				second = Random.Range(1,3)
				if second == 1 then
					second_color = Random.Range(1,3)
					if second_color == 1 then
						r = 1
						g = Random.Rangef(0,1)
					end
					if second_color == 2 then
						r = Random.Rangef(0,1)
						g = 1
					end
				end
				if second == 2 then
					second_color = Random.Range(1,3)
					if second_color == 1 then
						g = 1
						r = Random.Rangef(0,1)
					end
					if second_color == 2 then
						g = Random.Rangef(0,1)
						r = 1
					end
					
				end
			end


			if first_color == 2 then
				 b = 1
				second = Random.Range(1,3)
				if second == 1 then
					second_color = Random.Range(1,3)
					if second_color == 1 then
						r = 0
						g = Random.Rangef(0,1)
					end
					if second_color == 2 then
						r = Random.Rangef(0,1)
						g = 0
					end
				end
				if second == 2 then
					second_color = Random.Range(1,3)
					if second_color == 1 then
						g = 0
						r = Random.Rangef(0,1)
					end
					if second_color == 2 then
						g = Random.Rangef(0,1)
						r = 0
					end
					
				end
			end


			if first_color == 3 then
				 b = Random.Rangef(0,1)
				second = Random.Range(1,3)
				if second == 1 then
					second_color = Random.Range(1,3)
					if second_color == 1 then
						r = 0
						g = 1
					end
					if second_color == 2 then
						r = 1
						g = 0
					end
				end
				if second == 2 then
					second_color = Random.Range(1,3)
					if second_color == 1 then
						g = 0
						r = 1
					end
					if second_color == 2 then
						g = 1
						r = 0
					end
					
				end
			end
		end

		--Se vino

		GameObject.SetColor(transform.gameObject, r, g, b, 0.5)
		mult = 0
	end

	-- Update is called once per frame
	function BulletRotation:Update()
		mult = mult - var.speed
		transform:SetRotation(Quaternion.RotateY(mult))
	end

	return BulletRotation
end







