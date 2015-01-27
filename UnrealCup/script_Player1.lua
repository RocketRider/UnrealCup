runup = false
run = true
--counter = 0
function run()
    while AllowedToRun() do
	   --print("script 1")
        x,y,z = GetOwnLocation()
        x2, y2, z2 = GetBallPosition()
        MoveTo(x+200, y-200)
    end
end

