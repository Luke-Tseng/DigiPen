def knapsack(items, capacity):
    n = len(items)

    # Create a 2D array to store subproblem results
    dp = [[0] * (capacity + 1) for _ in range(n + 1)]

    # Fill the dp array using a bottom-up approach
    for i in range(1, n + 1):
        for w in range(capacity + 1):
            weight_i = items[i - 1][2]  # weight of the current item
            value_i = items[i - 1][1]   # value of the current item

            if weight_i > w:
                dp[i][w] = dp[i - 1][w]
            else:
                dp[i][w] = max(dp[i - 1][w], dp[i - 1][w - weight_i] + value_i)

    # Find the resulting items
    selected_items = []
    i, w = n, capacity
    while i > 0 and w > 0:
        weight_i = items[i - 1][2]
        if dp[i][w] != dp[i - 1][w]:
            selected_items.append(items[i - 1])
            w -= weight_i
        i -= 1

    return dp, selected_items

# Example usage:
items = [(1, 5, 3), (2, 4, 5), (3, 5, 4), (4, 2, 2), (5, 2, 1)]
knapsack_capacity = 8

result_table, resulting_items = knapsack(items, knapsack_capacity)

# Display the resulting table
for row in result_table:
    print(row)

# Display the resulting items
print("Resulting items:", resulting_items)
