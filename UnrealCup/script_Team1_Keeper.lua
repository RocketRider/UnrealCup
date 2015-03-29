function run()
	goal1_x,goal1_y,goal1_z = GetGoal1Position()
	goal2_x,goal2_y,goal2_z = GetGoal2Position()
    maxDist = math.abs(goal1_x-goal2_x) + math.abs(goal1_y-goal2_y)
    midLine_x = 0
	first = true
    rotation = 0
    startPositionX = 0
    startPositionY = 0
    startPositionZ = 0
    
	while AllowedToRun() do
        if(first) then
			startPositionX, startPositionY, startPositionZ = GetOwnLocation()
			first = false
		end
        x,y,z = GetOwnLocation()
        ballX, ballY, ballZ = GetBallPosition()
		stamina = GetStamina()
        player = GetVisiblePlayers()
        playerHasBall = hasBall()
        ballDist = math.abs(ballX-x) + math.abs(ballY-y)
        if(ballDist < 500) then
            if(hasBall()) then
                distMin = -1
                kickX = -1
                kickY = -1
                strength = 50
                for i = 1, #player, 1 do
                    if (player[i][1] == 1) then
                        dist = math.abs(player[i][3]-x)+math.abs(player[i][4]-y)
                        if(distMin == -1 or dist < distMin) then
                            kickX = player[i][3]
                            kickY = player[i][4]
                            
                            strength = dist / maxDist * 200
                            if(strength > 100) then
                                strength = 100
                            end
                        end    
                    end
                end
                if(kickX == -1) then
                    kickX = goal2_x
                    strength = 100
                end
                if(kickY == -1) then
                    kickY = goal2_y
                    strength = 100
                end
                Kick(kickX, kickY, 0, strength)
            else
                MoveTo(startPositionX, ballY, 100)
            end
        end
    end
end