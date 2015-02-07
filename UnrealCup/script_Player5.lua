runup = false
counter = 100
function run()
	print("script 3")
	while AllowedToRun() do
        x,y,z = GetOwnLocation()
        ballX, ballY, ballZ = GetBallPosition()
        
        if(math.abs(x-ballX)<100 and math.abs(y-ballY)<100) then
			--print("Kick")
            Kick(1,1,0,10000)
        end
        if (ballZ == 0 and ballY == 0 and ballX == 0) then
            MoveTo(x+50,y+50)
        end  
        if not (ballZ == 0 and ballY == 0 and ballX == 0) then
            MoveTo(ballX, ballY)
        end
	end
end