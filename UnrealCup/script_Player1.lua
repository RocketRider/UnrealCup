counter = 0
--print("Hi")
function run()
   while 1==2 do

	   counter = counter + 1
	   print("run")
	   
	   if (counter % 100) == 0 then
		  --Rotate(90)
	   else
		  MoveForward(0.2)
	   end
   end
end

