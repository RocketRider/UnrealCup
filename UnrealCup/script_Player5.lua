runup = false
counter = 100
function run()
	print("script 3")
	while AllowedToRun() do
        x,y,z = GetOwnLocation()
        ballX, ballY, ballZ = GetBallPosition()
        
        
        if(x==ballX and y==ballY) then
            Kick(1,1,z+2,10)
        end
        
        if (ballZ == 0 and ballY == 0 and ballX == 0) then
            MoveTo(x+50,y+50)
        end  
        if not (ballZ == 0 and ballY == 0 and ballX == 0) then
            MoveTo(ballX, ballY)
        end
	end
end