runup = false
counter = 100
function run()
	print("script 3")
	while AllowedToRun() do
        counter = counter + 1
        if (counter >= 100) then
		  x,y,z = GetOwnLocation()
		  ballX, ballY, ballZ = GetBallPosition()
          counter = 0
        end
        if not (ballZ == 0) then
            MoveTo(ballX-1, ballY-1)
        end
        if(x>=ballX-2 and y>=ballX-2) then
           KickBall(1,1,1,50)
        end
        if (ballZ == 0) then
            MoveTo(x+1,y+1)
        end  
	end
end