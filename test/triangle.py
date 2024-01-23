def shape(sides):
	sides.sort()
	if len(set(sides)) == 1:
		return sum(sides)
	elif len(set(sides)) == 2:
		if sides.count(sides[0]) == 2:
			return 2 * (sides[0] + sides[2])
		else:
			return 0
	else:
		return 0

sides = [0,0,0,0]

for i in range(4):
	sides[i] = int(input("Enter side %d:\n" % (i+1)))

perimeter = shape(sides)

if perimeter/sides[0] == 4:
	print("Forms a square with perimeter of %d" % perimeter)
elif perimeter != 0:
	print("Forms a rectangle with perimeter of %d" % perimeter)
else:
	print("Does not form a rectangle or square.")
