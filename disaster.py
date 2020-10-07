import pymc3 as pm
import numpy as np
import matplotlib.pyplot as plt

from statistics import NormalDist

SAMPLES = 20 * 1000

EARLY = 110
LATE = 390

_A_MU = 170
_A_SI = 10
_B_MU = 165
_B_SI = 7


_a = NormalDist(mu=_A_MU, sigma=_A_SI)
_b = NormalDist(mu=_B_MU, sigma=_B_SI)
_a_data = [round(x) for x in _a.samples(EARLY)]
_b_data = [round(x) for x in _b.samples(LATE)]

print(min(_a_data), max(_a_data))
print(min(_b_data), max(_b_data))

disaster_data = _a_data + _b_data

year = np.arange(len(disaster_data))

with pm.Model() as disaster_model:
    switchpoint = pm.DiscreteUniform("switchpoint", lower=0, upper=len(year))
    early_mean = pm.Exponential("early_mean", 0.01)
    late_mean = pm.Exponential("late_mean", 0.01)

    rate = pm.math.switch(switchpoint >= year, early_mean, late_mean)

    disasters = pm.Poisson("disasters", rate, observed=disaster_data)


with disaster_model:
    trace = pm.sample(SAMPLES)
    pm.traceplot(trace, ["early_mean", "late_mean", "switchpoint"])

plt.show()
