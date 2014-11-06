
   turnedRight = false
   turnedLeft = true
function run()
   while 1==1 do
	   x,y,z = GetOwnLocation()
	   
	   if not turnedRight then
          --MoveForward(0.2)
          Rotate(90)
            a = 0
         -- for i=0, 10000000, 1 do
               -- a = a+i
               -- end
		  Rotate(180)
		  turnedRight=true
		  turnedLeft=false
	   end
	   --if (y>=2000) then
		  --Rotate(180)
		  --turnedRight=false
		  --turnedLeft=true
	   --end

	   MoveForward(0.2)
   end
end

