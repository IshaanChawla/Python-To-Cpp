class TestClass():
	def __init__(self,x,y):
		self.a = x
		self.a = x + 1
		self.b = y
		self.c = 10
		self.c = 15

	def print_class(self):
		self.d = self.a - 1
		z = self.a + 1 
		f = self.geta()
		print self.a, self.b
	
	def geta(self):
		return self.a

def abc(a,b,c):
	d = 1.0
	e = d-1.5
	f = a
	d = 5.0
	print a
	print b
	print c
	return e

if __name__ == '__main__':
	a = 0
	c = a
	b = 'ishaan'
	new_var = TestClass(a,b)
	new_var.print_table()
	if a < 10:
		print a
		a += 1
	abc(a,b,c)
	print e
