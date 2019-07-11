def nod(a, b)
	r = [a, b] 
	x = [1, 0]
	y = [0, 1]
	q = [0, 0]

	i = 1
	while r[i] != 0
		q[i] = r[i - 1] / r[i]
		r[i + 1] = r[i - 1] % r[i]
		x[i + 1] = x[i - 1] - x[i]*q[i]
		y[i + 1] = y[i - 1] - y[i]*q[i]
		i += 1
	end

	i -= 1

	return r[i], x[i], y[i]
end

def nods(a, b)
	while a != 0 && b != 0
		(a > b) ? (a = a % b) : (b = b % a)
	end
	return a + b
end

def powm(a, n, m) #a^n % m
	res = 1
	while n > 0 
		if n & 1 == 1
			res = (res * a) % m
		end
		a = (a * a) % m
		n >>= 1
	end
	return res
end

def cmp1(a, b, m) #a*x = b (mod m)
	a = a % m
	b = b % m
	nd = nods(a, m)

	if b % nd != 0
		puts "error: cmp1"
		return nil
	end

	a, b, m = a / nd, b / nd, m / nd

	d, u, v = nod(a, m)

	if (b % d != 0)
		puts "error: cmp1"
		return nil
	else
		x = []
		x[0] = (b / d) * u % m
		for k in 1...nd
			x[k] = x[0] + k*m
		end
	end

	return x
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

		if (d = nods(a, n)) != 1
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

def check(a, x, b, p)
	for i in 0..x.size
		if powm(a, x[i], p) == b
			return x[i]
		end
	end

	return nil
end