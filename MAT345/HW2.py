import numpy as np

# Dataset
data = np.array([
    [3, 2, 1, 1],
    [1, 2, 0, 1],
    [4, 0, 0, -1],
    [2, 1, 2, 1],
    [3, -2, 1, -1],
    [2, 2, -1, 1],
    [4, 1, 3, -1],
    [0, -1, 2, -1],
    [1, 0, 4, -1],
    [-2, 1, 2, 1]
])

# Split data into x and y
x = np.c_[np.ones(data.shape[0]), data[:, :-1]]
y = data[:, -1]

# Initialize weights to 0
w = np.zeros(x.shape[1])

# PLA algorithm
def pla(x, y, w):
    iterations = 0
    while True:
        error_found = False
        for i in range(len(y)):
            if np.sign(np.dot(w, x[i])) != y[i]:
                w += y[i] * x[i]
                error_found = True
                break;
        iterations += 1
        if error_found == False:
            break
    return w, iterations

# Run PLA
final_weights, num_iterations = pla(x, y, w)

# Output the final hypothesis function and number of iterations
print("Final weights:", final_weights)
print("Number of iterations:", num_iterations)

# Create a dataset of random points
def generate_data(num_points):
    points = []
    while len(points) < num_points:
        x1 = np.random.randint(-5, 6)
        x2 = np.random.randint(-5, 6)
        if 2*x1 + 3*x2 != 1:  # Discard points on the line
            sign = 1 if 2*x1 + 3*x2 > 1 else -1
            points.append((1, x1, x2, sign))
    return np.array(points)

data = generate_data(20)

print(data)

x = data[:, :-1]
y = data[:, -1]

# Initialize weights to 0
w = np.zeros(x.shape[1])

# Run PLA
final_weights, num_iterations = pla(x, y, w)

# Output the final hypothesis function and number of iterations
print("Final weights:", final_weights)
print("Number of iterations:", num_iterations)