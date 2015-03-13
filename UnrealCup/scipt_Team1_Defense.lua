function run()
	goal1_x,goal1_y,goal1_z = GetGoal1Position()
	goal2_x,goal2_y,goal2_z = GetGoal2Position()
    maxDist = math.sqrt(math.pow(goal1_x-goal2_x, 2) + math.pow(goal1_y-goal2_y, 2))
    midLine_x = 0
	first = true
    rotation = 0
    
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
		goalDist = math.sqrt(math.pow(goal1_x-x, 2) + math.pow(goal1_y-y, 2))
        ballDist = math.sqrt(math.pow(ballX-x, 2) + math.pow(ballY-y, 2))
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
                         otherPlayerBallDist = math.sqrt(math.pow(player[i][3]-ballX, 2)+math.pow(player[i][4]-ballY, 2))
                         if (otherPlayerBallDist < minDistTeam) then
                            closestToBall = false
                            minDistTeam = otherPlayerBallDist
                         end
                        
                    -- check enemy team ball dist
                    else
                        otherPlayerBallDist = math.sqrt(math.pow(player[i][3]-ballX, 2)+math.pow(player[i][4]-ballY, 2))
                         if (otherPlayerBallDist < minDistEnemy) then
                            minDistEnemy = otherPlayerBallDist
                         end
                    end
                end
                
                -- is closest to ball?
                if(closestToBall) then
					if(ballDist > 200) then
						moveToBall = true
					else
						minDistE = 1000
						for i = 1, #player, 1 do
							-- check enemy players dist
							if (player[i][1] == 2) then
								 enemyDist = math.sqrt(math.pow(player[i][3]-ballX, 2)+math.pow(player[i][4]-ballY, 2))
								 if (enemyDist < minDistE) then
									pass = true
									break
								 end
							end
						end
						if(math.abs(x - midLine_x < 150)) then
							pass = true
						end
					end
                end
                
                if(moveToBall) then
                    MoveTo(ballX, ballY, 100)
				else
					if(pass) then
						minDist = -1
						passx = -1
						passy = -1
						for i = 1, #player, 1 do
							-- check enemy players dist
							if (player[i][1] == 1) then
								 teamDist = math.sqrt(math.pow(player[i][3]-x, 2)+math.pow(player[i][4]-y, 2))
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
							Kick(goal2_x, goal2_y, 0, 10)
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