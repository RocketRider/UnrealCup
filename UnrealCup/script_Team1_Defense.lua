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

            -- Ball over midLine
            if( (negativeSide and ballX >= midLine_x) or (not negativeSide and ballX <= midLine_x) ) then
                -- NotMyProblem.exe
                MoveTo(startPositionX, startPositionY, 50)

            -- Else...
            else
                closestToBall = true
                minDistTeam = ballDist
                minDistEnemy = -1

                -- loop through visible players
                for i = 1, #player, 1 do
                    
                    -- check own team ball dist
                    if (player[i][1] == 1) then
                         otherPlayerBallDist = math.abs(player[i][3]-ballX)+math.abs(player[i][4]-ballY)
                         if (otherPlayerBallDist < minDistTeam) then
                            closestToBall = false
                            minDistTeam = otherPlayerBallDist
                         end
                        
                    -- check enemy team ball dist
                    else
                        otherPlayerBallDist = math.abs(player[i][3]-ballX)+math.abs(player[i][4]-ballY)
                         if (mindDistEnemy == -1 or otherPlayerBallDist < minDistEnemy) then
                            minDistEnemy = otherPlayerBallDist
                         end
                    end
                end
                
                -- is closest to ball?
                if(closestToBall) then
					if(ballDist > 500) then
						moveToBall = true
					else
                        kick = false
                        moveToBall = false
						minDistE = 1000
						for i = 1, #player, 1 do
							-- check enemy players dist
							if (player[i][1] == 2) then
								 enemyDist = math.abs(player[i][3]-ballX)+math.abs(player[i][4]-ballY)
								 if (enemyDist < minDistE) then
									pass = true
									break
								 end
							end
						end
						if(math.abs(x - midLine_x) < 150) then
							pass = true
						end
                        if(pass == false) then
                            kick = true
                        end
					end
                end
                
                if(moveToBall) then
                    --print("movetoball")
                    MoveTo(ballX, ballY, 100)
				end
                if(pass) then
                    --print("pass")
                    minDist = -1
                    passx = -1
                    passy = -1
                    for i = 1, #player, 1 do
                        -- check enemy players dist
                        if (player[i][1] == 1) then
                             teamDist = math.abs(player[i][3]-x)+math.abs(player[i][4]-y)
                             if (minDist == -1 or teamDist < minDist) then
                                minDist = teamDist
                                passx = player[i][3]
                                passy = player[i][4]
                                break
                             end
                        end
                    end
                    strength = 100 * (minDist * 2 / maxDist)
                    if (strength > 100) then
                        strength = 100
                    end
                    if( not (passx == -1) and not (passy == -1) ) then
                        Kick(passx, passy, 0, strength)
                    else
                        Kick(goal2_x, goal2_y, 0, 100)
                    end
                end
                if(kick) then
                    --print("kick")
                    Kick(goal2_x,goal2_y, 0, 100)
                end
            end
        else
            rotation = rotation + 10
			Rotate(rotation)
        end
	end
end