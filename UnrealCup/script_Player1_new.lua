runup = false
rotation = 0
function run()
	
	goal1_x,goal1_y,goal1_z = GetGoal1Position()
	goal2_x,goal2_y,goal2_z = GetGoal2Position()
    teamId = GetTeamId()
    startx, starty, startz = GetOwnLocation()
	gotBall = false
	--print("Goal1 ".. goal1_x .. ", " .. goal1_y .. ", " .. goal1_z)
	--print("Goal2 ".. goal2_x .. ", " .. goal2_y .. ", " .. goal2_z)
	--Speak("Hallo Team 2")
	while AllowedToRun() do
        x,y,z = GetOwnLocation()
        ballX, ballY, ballZ = GetBallPosition()
		stamina = GetStamina()
        player = GetVisiblePlayers()
        kickoff = IsKickoff()
		playerHasBall = hasBall()
		
        --timePlayed = TimePlayed()
        
        if(kickoff > -1) then
            print("it's kickoff time!")
            if(kickoff == teamId) then
                --TODO
            else
                MoveTo(startx, starty, startz)
                while(kickoff > -1) do
                   kickoff = IsKickoff()
                end
            end
        end
		
        if (playerHasBall) then
			if (gotBall == false) then
				StopBall()
				gotBall = true
			end
			ownAbs = math.pow(goal1_x-x, 2)+math.pow(goal1_y-y, 2) - 300
			pass = false
			pass_x = 0
			pass_y = 0
			for i = 1, #player, 1 do
				if (player[i][1] == 1) then
					 playerAbs = math.pow(goal1_x-player[i][3], 2)+math.pow(goal1_y -player[i][4], 2)
					 if (playerAbs < ownAbs) then
						ownAbs = playerAbs
						pass_x = player[i][3] - x
						pass_y = player[i][4] - y
						pass = true
					 end
				 end
			end
			
			--Gegner in der nähe?
			--if (pass == false) then
			--	for i = 1, #player, 1 do
			--		if (player[i][1] == 2) then
			--			 enemyAbs = math.sqrt(math.pow(x-player[i][3], 2)+math.pow(x -player[i][4], 2))
			--			 if (enemyAbs < 1000) then
			--				pass = true
			--				for j = 1, #player, 1 do
			--					if (player[j][1] == 1) then
			--						pass_x = player[j][3] - x
			--						pass_y = player[j][4] - y
			--						break
			--					 end
			--				end
			--				break
			--			 end
			--		 end
			--	end
			--end
			
			
			
			if (pass) then
				if (stamina > 70) then
					Kick(pass_x, pass_y,0,100)
				end
			else
				if (ownAbs < 5000000) then -- try to score a goal
					Kick(goal1_x-x, goal1_y-y,ownAbs/5000,100)
				else
					Kick(goal1_x-x, goal1_y-y,0,10)
				end
			end
        
		else
			gotBall = false
        end
		if not (ballZ == 0 and ballY == 0 and ballX == 0) then
			ownAbs = math.pow(x-ballX, 2)+math.pow(y-ballY, 2)
			run = 0
			for i = 1, #player, 1 do
				if (player[i][1] == 1) then
					 playerAbs = math.pow(player[i][3]-ballX, 2)+math.pow(player[i][4]-ballY, 2)
					 if (playerAbs < ownAbs) then
						run = run + 1
					 end
				 end
			end
		
		
			if (run < 3) then
				MoveTo(ballX, ballY, 100 - run*20)
			else --Move with Ball X
				if (ballX > x) then
					if (ballX > 0) then 
						MoveTo(ballX, starty, 20)
					end	
				else
					MoveTo(ballX, y, 100)
				end
			end
        else -- Search for Ball
			rotation = rotation + 10
			Rotate(rotation)
        end  

	end
end