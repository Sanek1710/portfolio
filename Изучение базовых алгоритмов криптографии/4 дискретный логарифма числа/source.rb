require_relative 'src'
require_relative 'gauss'

class LG
	attr_accessor :a
	attr_accessor :b 
	attr_accessor :p 
	attr_accessor :q 
	attr_accessor :k0
	attr_accessor :k1
	attr_accessor :lg

	def initialize(a, b, p, q, u, v)
		@a = a
		@b = b
		@p = p
		@q = q
		@k0 = u
		@k1 = v
		@lg = powm(a, u, p) * powm(b, v, p) % p
	end 

	def f
		if 2*@lg < @p
			@lg = @a*@lg % @p
			@k0 = (@k0 + 1) % @q
		else
			@lg = @b*@lg % @p
			@k1 = (@k1 + 1) % @q
		end
		return self
	end 

	def !=(oth)
		#puts "#{lg} != #{oth.lg}"
		return lg != oth.lg
	end

	def lin_coef(oth)
		k = []
		k.push((k0 - oth.k0) % (q))
		k.push((oth.k1 - k1) % (q))

		return k
	end

	def print
		puts "#{[@a,@b,@lg,@p]}"
	end
end

def log1(a, b, p, q) #a^x = b mod p; res: x
	u, v = rand(2..q - 2), rand(2..q - 2)
	u, v = 13, 13
	c = LG.new(a, b, p, q, u, v)
	d = LG.new(a, b, p, q, u, v)
	
	puts "#{c.lg}".rjust(4) + "#{c.k0} + #{c.k1}x".rjust(20) + "#{d.lg}".rjust(4) + "#{d.k0} + #{d.k1}x".rjust(20)

	begin
		c.f
		d.f.f
		puts "#{c.lg}".rjust(4) + "#{c.k0} + #{c.k1}x".rjust(20) + "#{d.lg}".rjust(4) + "#{d.k0} + #{d.k1}x".rjust(20)
	end while c != d

	k = c.lin_coef(d)
	#puts k[0], k[1]
	x = cmp1(k[1], k[0], (q))

	for i in 0...x.size
		if powm(a, x[i], p) == b
			return x[i]
		end
	end

	return nil
end


class BASE2
	attr_accessor :a
	attr_accessor :b 
	attr_accessor :p 
	attr_accessor :q 
	attr_accessor :s 
	attr_accessor :sq71
	attr_accessor :sq72

	def initialize(a, b, p, q)
		@a = a
		@b = b
		@p = p
		@q = q
		@sq71 = [1]
		@sq72 = [b]
		@s = Math.sqrt(q).to_int + 1
	end 

	def set_sq72
		a_s = powm(a, (-s) % q, p)
		puts a_s

		last = b
		puts "s = " + s.to_s
		puts "a^s = " + a_s.to_s

		for i in (1...s) 
			@sq72.push(@sq72.last * a_s % p)
			#19205005773	3:12 часа
			#puts  @sq72[-1].to_s
		end
		#puts "seq 7.2 ready"
	end

	def set_sq71
		a71 = 1
		for i in (1...s)
			idx = sq72.index(a71)
			if idx != nil
				#puts "seq 7.1 ready"
				return (idx * s + i - 1) % p
			end
			a71 = a71 * a % p
		end
		return nil
	end

end

def log2(a, b, p, q) #a^x = b mod p; res: x
	#puts "#{a} ^ x = #{b} (mod #{p})"
	bs = BASE2.new(a, b, p, q)
	bs.set_sq72()
	p bs.sq72

	x = bs.set_sq71()

	return x
end


class BASE3
	attr_accessor :pr
	attr_accessor :b
	attr_accessor :m
	attr_accessor :g

	def initialize(pr)
		@pr = pr
		@b = [-1]
		@m = Math.exp(Math.sqrt(Math.log(pr)*Math.log(Math.log(pr))))

		@b.push(2)
		@b.push(3)

		puts "p = #{pr}"
		puts "M = #{m}"

		while @b.last < m
			@b.push(@b.last + 2)
			while true
				if Miller_Rabin_test(@b.last, 10)
					break
				end
				@b[-1] += 2			
			end
		end
	end

	def size
		return b.size
	end

	def [](i)
		return b[i]
	end

	def contain(n)
		dec = Array.new(b.size, 0.to_r)

		if n < 0
			dec[0] += 1
			n = -n
		end
		#p n
		for i in 1...b.size
			while n % b[i] == 0
				n = n/b[i]
				#printf("%d ", b[i])
				dec[i - 1] += 1
			end
		end

		if n != 1 
			return false, nil
		end
		return true, dec
	end

	def print
		p b
		puts "base size = #{@b.size}"
	end
end

def log3(a, b, p, q) #a^x = b mod p; res: x
	s = BASE3.new(p)
	s.print

	b_sys = []
	begin
		v = rand(1...q)
		flag, b_sys = s.contain(powm(b, v, p))
	end while flag == false
	b_sys[-1] = v

	g_sys = [[]]
	for i in 0...s.size - 1

		begin
			u = rand(1...q)
		end while powm(a, u, p) % s[i + 1] != 0

		flag, g_sys[i] = s.contain(powm(a, u, p))
		if flag == true 
			#p i
			g_sys[i][-1] = u
			#p "########################################"
			#p g_sys[i].map{|i| i.to_i}
		else
			redo
		end
	end

	puts 'log(b) = ' + b_sys.map{|i| i.to_i}.to_s

	x = gauss(g_sys)
	#puts 'log(BASE) = ' + x.to_s

	pa = 0
	for i in 0...s.size - 1
		pa += x[i]*b_sys[i].to_r
	end

	p pa

	x = cmp1(powm(b_sys[-1].to_i, pa.denominator, q), pa.numerator, q)
	x = check(a, x, b, p)
	return x
end


a = 3
b = 19
p = 31
q = 30

a = 3
b = 13
p = 737664493462811141483
q = 368832246731405570741
a = 7
b = 167
p = 587
q = 293

p Math.exp(Math.sqrt(Math.log(p)*Math.log(Math.log(p))))

#p "X = " + log1(a, b, p, q).to_s
puts
#p "X = " + log2(a, b, p, q).to_s
puts
p "X = " + log3(a, b, p, q).to_s
puts
