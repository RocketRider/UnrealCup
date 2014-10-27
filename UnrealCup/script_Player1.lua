counter = 0
function tick(s)
   counter = counter + 1
   if (counter % 100) == 0 then
      Rotate(90)
   else
      MoveForward(0.2)
   end
end
print("Player 1 loaded!")

