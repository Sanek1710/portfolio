#require 'mathn'

def gauss(a)
	n = a[0].size - 1 
	x = Array.new(n)
	k = 0

	while k < n

		max = a[k][k].abs
		index = k

		for i in (k + 1...n)
		
			if a[i][k].abs > max
			
				max = a[i][k].abs
				index = i
			end
		end

		if max == 0
			p index, "mt A err"
			return 0
		end

		a[k], a[index] = a[index], a[k]

		for i in k...n
			temp = a[i][k];
			if temp.abs == 0 
				next
			end

			for j in 0...n
				a[i][j] = a[i][j] / temp
			end

			a[i][n] = a[i][n] / temp

			if i == k
				next
			end

			for j in 0...n
				a[i][j] = a[i][j] - a[k][j]
			end

			a[i][n] = a[i][n] - a[k][n]
		end
	
		k += 1
	end

	k = n - 1
	while k >= 0
	
		x[k] = a[k][n]

		for i in 0...k
			a[i][n] = a[i][n] - a[i][k] * x[k] 
		end

		k -= 1
	end

	return x
end
