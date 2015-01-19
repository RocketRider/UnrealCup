runup = false
counter = 0
function run()
	print("script 3")
	while AllowedToRun() do
		x,y,z = GetOwnLocation()
		counter = counter +1

		if(counter == 1) then
			Kick(1,0,0,5000)
		end
		if (y<-2000) and (not runup) then
			Rotate(90)
			runup=true
		end
		if (y>2000) and (runup) then
			Rotate(270)
			runup=false
		end

		MoveForward(2)
	end
end

