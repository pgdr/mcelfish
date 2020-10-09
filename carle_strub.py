def _cs_T(data):
    return sum(data)


def _cs_K(data):
    return len(data)


def _cs_X(data):
    x = 0
    k = _cs_K(data)
    for i in range(len(data)):
        x += (k - (1 + i)) * data[i]
    return x


def _z_pre_estimate(data, hatN):
    t = _cs_T(data)
    x = _cs_X(data)
    k = _cs_K(data)

    tellerA = hatN - t + 0.5
    tellerB = (k * hatN - x) ** k
    nevner = (k * hatN - x - t) ** k

    return (tellerA * tellerB) / nevner - 0.5


def _cs_Eq(t, hatN, k, x):
    prod = 1
    for i in range(k):
        j = i + 1
        teller = k * hatN - x - t + 1 + k - j
        nevner = k * hatN - x + 2 + k - j
        prod *= (teller * 1.0) / nevner

    return t - 1 + ((hatN + 1) * prod)


def removal_carle_strub(data):
    t = _cs_T(data)
    x = _cs_X(data)
    k = _cs_K(data)
    hatN = t

    for i in range(1000 * 1000):
        lhs = hatN + i
        rhs = _cs_Eq(t, lhs, k, x)
        if lhs >= rhs:
            return lhs
    raise ValueError("Unable to find CS solution")


def removal_zippin(data):
    t = _cs_T(data)
    k = _cs_K(data)
    x = _cs_X(data)
    z_min = ((t - 1) * (k - 1) / 2) - 1

    if x <= z_min:
        raise ValueError(f"Zippin X below z_min for {data}")

    hatN = t
    for i in range(1000 * 1000):
        lhs = hatN + i
        rhs = _z_pre_estimate(data, lhs)
        if rhs > lhs:
            return lhs

    raise ValueError("Unable to find CS solution")


def main():
    from sys import argv

    if len(argv) < 2:
        exit("Usage: carle_strub 30 20 10 0")

    data = [int(e.lstrip(",").rstrip(",")) for e in argv[1:]]
    print(data)
    print("cs", removal_carle_strub(data))
    print("z ", removal_zippin(data))


if __name__ == "__main__":
    main()
