import random
import sys


def main():
    p = 0
    if len(sys.argv) > 1:
        N = int(sys.argv[1])
    else:
        N = random.randint(5, 250)
    print(f"N = {N}")

    if len(sys.argv) > 2:
        p = float(sys.argv[2])
    while not 0.01 < p < 0.99:
        p = round(random.random(), 2)
    print(f"p = {p}")

    current = N
    data = []
    while current > (N * 0.2) and len(data) < 35:
        q = int(current * p)
        data.append(q)
        current -= q
    print(" ".join([str(e) for e in data]))


if __name__ == "__main__":
    main()
