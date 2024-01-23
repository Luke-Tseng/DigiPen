sum = 0
num = 0
while num != -1:
	num = int(input("Enter the next number (enter -1 when finished)\n"))
	if num != -1:
		sum += num
print("The sum of your numbers is %d" % sum)