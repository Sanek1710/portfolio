require_relative 'src'

def decomp_p(n)
	a = rand(2...n-1)
	b = a
	c = -1
	puts "a = #{a}"
	puts "c = #{c}"

	i=0
	#printf("%d\t%d\t%d\t%d\n", i, a, b, a)
	begin
		a = (a*a + c) % n
		b = (b*b + c) % n
		b = (b*b + c) % n
		d = NOD((a - b).abs, n)
		i += 1
	end while d == 1
	#printf("%d\t%d\t%d\t%d\n", i, a, b, d)
	if d != n
		return d, n/d
	end
end

def O1(n)
	return Math.sqrt(Math.sqrt(n))
end


class PRIME_BASE 
	def new(sz)
		@b = 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113 
		
		if sz < @b.size
			return
		end
		p @b.size, sz
		for i in (@b.size...sz)
			add
			p "1"
		end
	end 

	def [](i)
		@b[i]
	end

	def size
		@b.size
	end 

	def add
		@b.push(@b[-1] + 2)
		while true
			if Miller_Rabin_test(@b[-1], 10)
				break
			end
			@b[-1] += 2
		end
	end 

	def addn(cnt)
		for i in (0...cnt)
			add
		end
	end
	
	def del
		if @b.size > 2
			@b.pop
		end
	end

	def print
		puts "#{@b}"
	end
end

def decomp_p_m1(n)
	s = n.bit_length
	b = PRIME_BASE.new()
	b.new(s)
	p "1"

	puts "B = #{s}"
	puts "n = #{n}"

	#b.print
	#b.print

		a = rand(2..n)	#2

		d = NOD(a, n)
		if d != 1
			return d, n/d
		end
		i = 0

	while true
	a = rand(2..n)


		for i in (i...b.size)	#3
			l = n.bit_length / b[i].bit_length
			#printf("%d, ", l)
			bl = pow(b[i], l)
			a = powm(a, bl, n)
		end

		d = NOD(a - 1, n)		#4

		if d == 1
			#puts "to+"
			b.addn(10)
			#puts "+"
		elsif d == n
			b.del
			puts "-"
		else
			return d, n/d
		end
		#puts "*"
	end
end

def O2(n)
	b = O1(O1(n))
	return b * Math.log10(b) * (Math.log10(n) ** 2)
end

n = []
n[0] = 49127446850874971237
n[1] = 975539613370052274997414180288562718773
n[2] = 73240738533199146158553962042707483015272520900699573200315363811781100297409051


p "decomp_p"
p decomp_p(4153693)
p "decomp_p_m1"
p decomp_p_m1(4153693)

exit(0)

l = 5
n0 = n[0]

p O2(n[2])



Tm1 =  ((Math.sqrt(l * Math.sqrt(n0))*(Math.log10(n0)*Math.log10(n0))))
#Tm1 = Math.sqrt(n0) * Math.log10(Math.sqrt(n0)* (Math.log10(n0)**2)) 
n0 = n[1]
Tm2 =  ((Math.sqrt(l * Math.sqrt(n0))*(Math.log10(n0)*Math.log10(n0))))
#Tm2 = Math.sqrt(n0) * Math.log10(Math.sqrt(n0) * (Math.log10(n0)**2))
n0 = n[2]
Tm3 =  ((Math.sqrt(l * Math.sqrt(n0))*(Math.log10(n0)*Math.log10(n0))))
#Tm3 = Math.sqrt(n0) * Math.log10(Math.sqrt(n0) * (Math.log10(n0)**2))

puts

t = Time.now
p decomp_p_m1(n[2])
t = Time.now - t
puts t
puts t*(Tm2 / Tm1)
puts t*(Tm3 / Tm1)
