function sleep(n)
  local t = os.clock()
    while os.clock() - t <= n do
    -- nothing
  end
end



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
            if(kickoff == 0) then
                --print("kickoff 2 received")
                MoveTo(startx, starty, 100)
                while(kickoff == 0) do
                    kickoff = IsKickoff()
                end
            end
            if(kickoff == teamId) then
                --print("kickoff: own team: my team: " .. teamId .. "; kickoff-team: " .. kickoff)
                --cont playing
            else
                --print("kickoff: enemy team: my team: " .. teamId .. "; kickoff-team: " .. kickoff)
                --wait
                while(kickoff > -1) do
                    kickoff = IsKickoff()
                end
            end
        end
		
        if (playerHasBall) then
		--if(math.abs(ballX-x)<150 and math.abs(ballY-y)<150) then
			
			--while (true) do
			--	sleep(1)
			--	rotation = rotation + 10
			--	Rotate(rotation)
			--end
			
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
					
					Kick(goal1_x-x + math.random (1000) - math.random (1000), goal1_y-y,ownAbs/5000,100)
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
				if (ballX > 0) then
					MoveTo(ballX+1000, starty, 50)	
				else
					MoveTo(ballX-1000, starty, 100)
				end
			end
        else -- Search for Ball
			rotation = rotation + 10
			Rotate(rotation)
        end  

	end
end