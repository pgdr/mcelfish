"""Removal sampling.

Maximum Likelihood (ML) estimates for estimating population size (N)
for a constant probability of capture model.
"""

import pymc3 as pm
import sys
from testdata import catches

SAMPLES = 50 * 1000
TUNE = 5 * 1000
TESTCASE = 2

basic_model = pm.Model()

case = catches[TESTCASE]

with basic_model:
    # prior
    N = pm.DiscreteUniform("N", lower=0, upper=1000)
    p = pm.Uniform("p", lower=0, upper=1)

    observations = case.data
    catch = []
    for idx, c in enumerate(observations):
        q = pm.Binomial(f"q{idx}", N - sum(catch), p, observed=[c])
        catch.append(q)

with basic_model:
    trace = pm.sample(draws=SAMPLES, tune=TUNE)
    print(pm.summary(trace))
    var = ["N", "p"]


median = sorted(trace["N"])[len(trace["N"]) // 2]
mm = sorted([case.hat_nz, case.hat_ncs])
print(f"median = {median}, expected = {mm}")

with basic_model:
    if "--plot" in sys.argv:
        import matplotlib.pyplot as plt

        pm.traceplot(trace, var)
        plt.show()
