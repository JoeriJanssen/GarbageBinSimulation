import sys

# Your final solution should use sys.stdin for input, but for testing you may
# want to use a file.
# input = open("/some/file", "r")
input = sys.stdin

p, l = [int(x) for x in input.readline().split(" ")]

connections = []
for i in range(l):
    a, b = [int(x) for x in input.readline().split(" ")]
    connections.append((a, b))

# Run your algorithm.
answer = 4

# Print your solution.
print(answer)
