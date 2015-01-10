runup = false
run = true
--counter = 0
function run()
	print("script 1")
   while AllowedToRun() do
	   x,y,z = GetOwnLocation()
	   --counter = counter +1
	   
	   --if (counter%100000 == 0) then
		 --print("running")
	   --end
	   if (y<-2000) and (not runup) then
          Rotate(90)
		  runup=true
	   end
	   if (y>2000) and (runup) then
		  Rotate(270)
		  runup=false
	   end

	   stamina = GetStamina()
	   if stamina < 20 then
		  run = false
	   end
	   if stamina >= 100 then
		  run = true
	   end
	   
	   if run then
	      MoveForward(1)
	   end
	   
	   --print(stamina)
   end
end

