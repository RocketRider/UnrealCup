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
			if(startPositionX < 0) then
				negativeSide = true
			else
				negativeSide = false
			end
			first = false
		end
        x,y,z = GetOwnLocation()
        ballX, ballY, ballZ = GetBallPosition()
		stamina = GetStamina()
        player = GetVisiblePlayers()
        ballDist = math.abs(ballX-x) + math.abs(ballY-y)
        moveToBall = false
		pass = false
        
        if not (ballZ == 0 and ballY == 0 and ballX == 0) then
            defense = false
            attack = false
            myJob = false
            ballDist = math.abs(ballX - x) + math.abs(ballY - y)
            inEnemyField = false
            if((negativeSide and ballX > midLine_x) or (not negativeSide and ballX < midLine_x)) then
               inEnemyField =true
            end
            closestToBall = true
            minDistTeam = ballDist
            minDistEnemy = -1
            for i = 1, #player, 1 do
                if (player[i][1] == 1) then
                    otherPlayerBallDist = math.abs(player[i][3]-ballX)+math.abs(player[i][4]-ballY)
                    if (otherPlayerBallDist < minDistTeam) then
                        closestToBall = false
                        minDistTeam = otherPlayerBallDist
                    end
                else
                    otherPlayerBallDist = math.abs(player[i][3]-ballX)+math.abs(player[i][4]-ballY)
                    if (mindDistEnemy == -1 or otherPlayerBallDist < minDistEnemy) then
                        minDistEnemy = otherPlayerBallDist
                    end
                end
            end
            if(not closestToBall and minDistEnemy < minDistTeam) then
                defense = true
                --move backward
            end
            if(not closestToBall and minDistEnemy > minDistTeam) then
                --move forward
            end
            if(closestToBall) then
                myJob = true
            end
            
            if(myJob) then
                playerHasBall = hasBall()
                if(not playerHasBall) then
                    MoveTo(ballX, ballY, 100)
                else
                    if(not inEnemyField) then
                        for i = 1, #player, 1 do
                            minDist = -1
                            if (player[i][1] == 2) then
                                otherPlayerBallDist = math.abs(player[i][3]-ballX)+math.abs(player[i][4]-ballY)
                                if (otherPlayerBallDist < minDist or minDist == -1) then
                                    minDist = otherPlayerBallDist
                                end
                            end
                        end
                        if(minDist < 500) then
                            passX = goal2_x
                            passY = goal2_y
                            maxDist = 0
                            for i = 1, #player, 1 do
                                if (player[i][1] == 1) then
                                    currentDist = -1
                                    for u = 1, #player, 1 do
                                        if(player[u][1] == 2) then
                                            dist = math.abs(player[u][3] - player[i][3]) + (player[u][3] - player[i][3])
                                            if(currentDist == -1 or dist < currentDist) then
                                                currentDist = dist
                                            end
                                        end
                                    end
                                end
                                if(currentDist > maxDist) then
                                    passX = player[i][3]
                                    passY = player[i][4]
                                end
                            end
                            Kick(passX, passY, 0, 70)
                        else
                            Kick(goal2_x-x, goal2_y-y, 0, 10)
                        end
                    else
                        distGoal = math.abs(goal2_x - x) + math.abs(goal2_y - y)
                        closestToGoal = true
                        passX = goal2_x
                        passY = goal2_y
                        for i = 1, #player, 1 do
                            if(player[i][1] == 1) then
                                dist = math.abs(goal2_x - player[i][3]) + math.abs(goal2_y - player[i][4])
                                if(dist < distGoal) then
                                    distGoal = dist
                                    closestToGoal = false
                                    passX = player[i][3]
                                    passY = player[i][4]
                                end
                            end
                        end
                        if(not closestToGoal) then
                            Kick(passX, passY, 70)
                        else    
                            Kick(goal2_x, goal2_y, 100)
                        end
                    end
                end
            end
        else
            rotation = rotation + 10
			Rotate(rotation)
        end
	end
end