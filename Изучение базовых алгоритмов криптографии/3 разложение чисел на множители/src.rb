def powm(a, n, mod)
	res = 1
	while n > 0 
		if n & 1 == 1
			res = (res * a) % mod
		end
		a = (a * a) % mod
		n >>= 1
	end
	return res
end

def pow(a, n)
	res = 1
	while n > 0 
		if n & 1 == 1
			res = (res * a)
		end
		a = (a * a)
		n >>= 1
	end
	return res
end

def NOD(a, b)
	while a != 0 && b != 0
		(a > b) ? (a = a % b) : (b = b % a)
	end
	return a + b
end

def Miller_Rabin_test(n, iter)
	if n & 1 == 0 || n < 5
		if n != 2 && n != 3
			return false
		else
			return true
		end
	end

	s, t = 0, n - 1

	while t & 1 == 0
		t = t >> 1
		s += 1
	end

	for i in (0...iter)
		a = rand(2..n - 2)

		if (d = NOD(a, n)) != 1
			return false
		end

		x = powm(a, t, n)
		
		if x == 1 || x == n - 1
			next
		end

		for j in (1..s - 1)
			x = x*x % n

			if x == 1
				return false
			end

			if x == n - 1
				break
			end
		end

		if x == n - 1
			next
		end

		return false
	end
	return true
end

