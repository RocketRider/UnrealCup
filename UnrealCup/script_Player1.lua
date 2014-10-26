counter = 0
function tick(s)
   counter = counter + 1
   print(counter)
   if (counter % 100) == 0 then
      Rotate(90)
   else
      MoveForward(10)
   end
end
print("Player 1 loaded!")

