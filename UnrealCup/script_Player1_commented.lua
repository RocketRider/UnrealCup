MAX_ABS = 50000 	-- Maximaler Abstand in dem der Spieler versucht ein Tor zu schießen
MAX_RUN = 3			-- Wie viele Spieler sollen dem Ball hinterher rennen
BALL_DIST = 1000	-- Abstand zum Ball beim Mitlaufen nach vorne/hinten



runup = false
rotation = 0
function run()
	-- Konstanten abrufen
	goal1_x,goal1_y,goal1_z = GetGoal1Position()
	goal2_x,goal2_y,goal2_z = GetGoal2Position()
    teamId = GetTeamId()
    startx, starty, startz = GetOwnLocation()
	gotBall = false

    -- Schleife, die die KI bis zum Ende des Spiels abarbeitet.
	while AllowedToRun() do
        -- Variablen Abrufen
        x,y,z = GetOwnLocation()
        ballX, ballY, ballZ = GetBallPosition()
		stamina = GetStamina()
        player = GetVisiblePlayers()
        kickoff = IsKickoff()
		playerHasBall = hasBall()
        
        -- Überprüfen, ob ein Anstoß ansteht ( -1 -> Kein Anstoß)
        if(kickoff > -1) then
            
            -- 0 -> Anstoß wird vorbereitet, kein Spieler darf den Ball berühren
            -- Rückzug in eigene Teamhälfte und warten auf Freigabe des Anstoßes
            if(kickoff == 0) then
                MoveTo(startx, starty, 100)
                while(kickoff == 0) do
                    kickoff = IsKickoff()
                end
            end
            
            -- Darf das eigene Team den Anstoß ausführen?
            if(kickoff == teamId) then
                -- Wenn ja: keine besondere Aktivität, Spiel regulär fortsetzen
            else
                -- Wenn nein: Warten, bis das gegnerische Team den Anstoß beendet hat
                while(kickoff > -1) do
                    kickoff = IsKickoff()
                end
            end
        end
		
        -- Befindet sich der ball in Reichweite?
        if (playerHasBall) then
            
            -- Wenn der Ball das erste mal in Reichweite kommt, wird er gestoppt
			if (gotBall == false) then
				StopBall()
				gotBall = true
			end
            
            -- Abstand zum Tor berechnen
			ownAbs = math.pow(goal1_x-x, 2)+math.pow(goal1_y-y, 2)
			pass = false
			pass_x = 0
			pass_y = 0
            
            -- Alle befreundeten Spieler durchlaufen und Spieler suchen, der sich nächer am Tor befindet
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
			
            -- Abspielen an Mitspieler..
			if (pass) then
				if (stamina > 70) then
					Kick(pass_x, pass_y,0,100)
				end
                
            -- ..oder weiter in Richtung Tor spielen
			else
                -- Nahe genug am Tor: Versuchen, ein Tor zu schießen
				if (ownAbs < MAX_ABS) then
					
					Kick(goal1_x-x + math.random (1000) - math.random (1000), goal1_y-y,ownAbs/5000,100)
                    
                -- Sonst: Weiter richtung Tor dribbeln
				else
					Kick(goal1_x-x, goal1_y-y,0,10)
				end
			end
        
		else
			gotBall = false
        end
        
		-- Ist der Ball sichtbar?
		if not (ballZ == 0 and ballY == 0 and ballX == 0) then
            
            -- Ist der Ball in Reichweite?
			ownAbs = math.pow(x-ballX, 2)+math.pow(y-ballY, 2)
            
            -- Zähle die Anzahl der Spieler, die Näher am Ball sind
			run = 0
			for i = 1, #player, 1 do
				if (player[i][1] == 1) then
					 playerAbs = math.pow(player[i][3]-ballX, 2)+math.pow(player[i][4]-ballY, 2)
					 if (playerAbs < ownAbs) then
						run = run + 1
					 end
				 end
			end
		
		    -- Ist der Spieler selbst unter den X Spielern die am nächsten am Ball sind: Laufe zum Ball
			if (run < MAX_RUN) then
				MoveTo(ballX, ballY, 100)
                
            -- Bereits genügend Spieler beim Ball: Bewege vor und zurück, abhängig von der Ballposition
			else --Move with Ball X
				if (ballX > 0) then
					MoveTo(ballX+BALL_DIST, starty, 50)	
				else
					MoveTo(ballX-BALL_DIST, starty, 100)
				end
			end
            
        -- Suche nach dem Ball
        else
			rotation = rotation + 10
			Rotate(rotation)
        end  

	end
end