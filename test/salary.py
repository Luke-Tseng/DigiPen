total1 = 0
total2 = 0

for x in range(1, 6):
	salary1 = 0
	salary2 = 0
	while salary1 == salary2:
		salary1 = int(input("Enter the salary individual 1 got in year %d\n" % x))
		salary2 = int(input("Enter the salary individual 2 got in year %d\n" % x))
		if salary1 == salary2:
			print("Salaries are the same, enter again")
	total1 += salary1
	total2 += salary2

if total1 > total2:
	print("Individual 1 has the higher salary")
elif total1 < total2:
	print("Individual 2 has the higher salary")
else:
	print("Same salary")
