import itertools

M = [8, 13, 9, 6, 15, 4, 10, 19]

for config in itertools.product(*[(0, m) for m in M]):
    if sum(config) == 24:
        print(config, sum(config))
        print(sum(config[: len(M) // 2]))
        print(sum(config[len(M) // 2 :]))

print()


def add(L, R):
    return {a + b for a in L for b in R}


def sub(L, R):
    return {a - b for a in L for b in R}


tree = [[{0, m} for m in M]]

print("Full forward pass")
while len(tree[-1]) > 1:
    tree.append(
        [add(tree[-1][2 * i], tree[-1][2 * i + 1]) for i in range(len(tree[-1]) // 2)]
    )

print("\n".join(map(str, tree)))

print("backward pass")

child_lhs = tree[-2][0]
child_rhs = tree[-2][1]

print("first half menu subtotal is", sub({24}, child_rhs) & child_lhs)

print("second half menu subtotal is", sub({24}, child_lhs) & child_rhs)
