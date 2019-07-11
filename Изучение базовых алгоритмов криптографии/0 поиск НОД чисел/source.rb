def nod(a, b)
	while a != 0 && b != 0
		if a > b 
			a = a % b 
		else 
			b = b % a 
		end
	end

	return a + b
end

def nod_bin(a, b)
	if a == 0
		return b
	elsif b == 0
		return a
	end

	c = 1
	while a % 2 == 0 && b % 2 == 0 
		a = a << 1
	end

	while a != b
		while a % 2 == 0 && a != 0
			a = a >> 1
		end

		while b % 2 == 0 && b != 0
			b = b >> 1
		end

		if a > b 
			a -= b
		elsif b > a
			b -= a
		else 
			return b * c
		end
	end

	return b * c
end

a = 10
b = 5

a = 33924414461653598264285564845389272911316139915199151947549878594618783073313249
b = 44602576298334788347500983066239965450969299441697511923198570955036712395012561

puts "#{nod(a, b)}\n"