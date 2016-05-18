class Greet():
	def __init__(self,a,b):
		self.my_var = a
		self.my_var_again = b
	
class Hello():
	def __init__(self):
		print 'Hello'

class Hi(Hello,Greet):
	def __init__(self):
		print 'Hi'
	def hi_func(self,x,y):
		print 'hi_func'
	
def myfunc(arg1,arg2,arg3,arg4,arg5):
	print arg

def mainfunction():
	myfunc('Hello','a','ab','\n',"a")
	x = raw_input('Enter a string : ')
	print "Value : " + x
	if x == 'Hello':
		print 'Bye'
	elif x == 'Hi' or x == 'Bye':
		print 'Not Bye'
	else :
		print 'Dont know!!'

	i = 0
	while i < 100:
		j = 0
		while j < 100:
			print i,j + 'x'
			j =+ 1
		i += 1
	#red = Greet(i,x)	
	#return x

if __name__ == '__main__':
	mainfunction()
