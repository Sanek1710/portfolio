def Carmicle_num1(k)
	puts "(6*k + 1)*(12*k + 1)*(18*k + 1)"
	puts "#{(6*k + 1)}*#{(12*k + 1)}*#{(18*k + 1)}"
	return (6*k + 1)*(12*k + 1)*(18*k + 1)
end

def Carmicle_num2(k)
	puts "(12*k + 5)*(16*k + 13)*(48*k + 17)"
	puts "#{(12*k + 5)}*#{(16*k + 13)}*#{(48*k + 17)}"
	return (12*k + 5)*(16*k + 13)*(48*k + 17)
end

def Carmicle_num3(k)
	puts "(180*k + 7)*(300*k + 11)*(360*k + 13)*(1200*k + 41)"
	puts "#{(180*k + 7)}*#{(300*k + 11)}*#{(360*k + 13)}*#{(1200*k + 41)}"
	return (180*k + 7)*(300*k + 11)*(360*k + 13)*(1200*k + 41)
end

def pow(a, n, mod)
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

def NOD(a , b)
	while a != 0 && b != 0
		(a > b) ? (a = a % b) : (b = b % a)
	end 
	return a + b
end

def Ferm_test(n, iter)
	if n & 1 == 0 || n < 5
		if n != 2 && n != 3
			return false
		else
			return true
		end
	end

	a = []
	for i in (0...iter)
		a[i] = rand(2..n-2)
		if (d = NOD(a[i], n)) != 1
			puts "a = #{a[i]}"
			puts "NOD(a, n) == #{d}"
			return false
		end

		r = pow(a[i], n - 1, n)

		if r != 1
			puts "a = #{a[i]}"
			puts "a^(n - 1) != 1"
			puts "#{r} != 1"
			return false
		end
	end
	for i in (0...iter)
		puts "a = #{a[i]} : ok"
	end
	return true
end

def Jacob(a, b)
	if b & 1 == 0
		return 0
	end

	if NOD(a, b) != 1
		return 0
	end

	r = 1 

	if a < 0 
		a = -a
		if b % 4 == 3 
			r = -r
		end
	end

	begin
		t = 0
		while a & 1 == 0
			t += 1
			a = a >> 1
		end
		if t & 1 == 1
			if b % 8 == 3 || b % 8 == 5
				r = -r
			end
		end
		
		if a % 4 == 3 && b % 4 == 3
			r = -r
		end

		a, b = b % a, a
	end while a != 0

	return r
end

def Solovay_Strassen_test(n, iter)
	if n & 1 == 0 || n < 5
		if n != 2 && n != 3
			return false
		else
			return true
		end
	end
	a = []
	for i in (0...iter)
		a[i] = rand(2..n-1)
		if (d = NOD(a[i], n)) != 1
			puts "a = #{a[i]}"
			puts "NOD(a, n) == #{d}"
			return false
		end

		apow = pow(a[i], (n - 1)/2, n)
		jac0 = Jacob(a[i], n)
		jac = (n + jac0) % n

		if apow != jac
			puts "a = #{a[i]}"
			puts "a^((n - 1)/2) mod n != J(a, n)"
			puts "#{apow} != #{jac0}"
			return false
		end
	end
	for i in (0...iter)
		puts "a = #{a[i]} : ok"
	end
	return true
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
	a = []
	for i in (0...iter)
		a[i] = rand(2..n - 2)
		if (d = NOD(a[i], n)) != 1
			puts "a = #{a[i]}"
			puts "NOD(a, n) == #{d}"
			return false
		end

		x = pow(a[i], t, n)
		
		if x == 1 || x == n - 1
			next
		end

		for j in (1..s - 1)
			x = x*x % n
			if x == 1
				puts "a = #{a[i]}"
				puts "x == 1 mod n"
				return false
			end
			if x == n - 1
				break
			end
		end
		if x == n - 1
			next
		end
		puts "a = #{a[i]}"
		puts "x != n - 1 mod n"
		puts "x = #{x}"
		return false
	end
	for i in (0...iter)
		puts "a = #{a[i]} : ok"
	end
	return true
end

n = []
n[0] = 11339922864793975343 #yes
n[1] = 279903060598223366729968707159806853197 #no 11339922864793975343*24682977471320627779
n[2] = 1483072475923817659253679706605513896303 #yes
n[3] = 25488720976425084237842348354199610694722573056805983727294308148580515046869423 #no


for i in (0...4)
	puts "n = #{n[i]}\n\n"
	puts "Ferm_test: "
	puts "#{			Ferm_test(n[i], 5)}\n\n"		
	puts "Solovay_Strassen_test: "
	puts "#{Solovay_Strassen_test(n[i], 5)}\n\n"	
	puts "Miller_Rabin_test: "
	puts "#{	Miller_Rabin_test(n[i], 5)}\n\n"
	puts "="*100
	puts
end

puts "#"*100
puts

Cm = []
puts "Cm0 = #{Cm[0] = Carmicle_num1(rand(2000000000..5000000000))}\n\n"
puts "Cm1 = #{Cm[1] = Carmicle_num2(rand(2000000000..5000000000))}\n\n"
puts "Cm2 = #{Cm[2] = Carmicle_num3(rand(200000..500000))}\n\n"
puts

for i in (0...3)
	puts "Cm = #{Cm[i]}\n\n"
	puts "Ferm_test: "
	puts "#{			Ferm_test(Cm[i], 5)}\n\n"
	puts "Solovay_Strassen_test: "
	puts "#{Solovay_Strassen_test(Cm[i], 5)}\n\n"
	puts "Miller_Rabin_test: "
	puts "#{	Miller_Rabin_test(Cm[i], 5)}\n\n"
	puts "="*100
	puts
end
