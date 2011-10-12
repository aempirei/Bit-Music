#!/usr/bin/env ruby

require 'matrix'

N = 16

def b(i,l,a,b)
	(0...l).map { |n| 2 * ((( n * a / b ) >> i) & 1) - 1 }
	# (0...l).map { |n| 2 * ((( n * a / b ) >> i) & 1) - 1 }
end

# swDSTi = (0..3).map { |k| b(k, N, 1, 1) }
swDSTi = (1..N).map { |k| b(0, N, 2, k) }

def pm(m)
	m.to_a.map { |m_i| m_i.map { |m_ij| sprintf "%+d", m_ij }.join(' ') }.map { |m_i| m_i + "\n" }.join('')
end

m = Matrix.columns(swDSTi)

print pm(m)
puts ''
print pm(m.transpose)
puts ''
print pm(m.transpose * m)
