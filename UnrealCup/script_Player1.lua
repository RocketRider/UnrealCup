counter = 0
data = 55
function tick(s)
   counter = counter + 1
   
   -- TEST
   for i=10,1,-1 do 
	data=data+3
   end
   ----------------
   
   if (counter % 100) == 0 then
      Rotate(90)
   else
      MoveForward(0.2)
   end
end

