runup = false
run = true
--counter = 0
function run()
	print("script 1")
	Rotate(270)
    x,y,z = GetOwnLocation()
    MoveTo(x+1000,y+500)
end

