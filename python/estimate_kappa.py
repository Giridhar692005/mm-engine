import json
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

cols = ["ts", "price", "qty", "buy_id", "sell_id", "aggressor", "mid_before", "agent_buy", "agent_sell"]
tr = pd.read_csv("results/noagent_trades.csv", names=cols)
tr = tr[tr["mid_before"] > 0]                                   # drop rows with no mid yet

# one market order can produce several trades (a sweep): keep its deepest reach
tr["taker_id"] = np.where(tr["aggressor"] == "B", tr["buy_id"], tr["sell_id"])
tr["depth"] = (tr["price"] - tr["mid_before"]).abs()
reach = tr.groupby("taker_id")["depth"].max()

total_time = tr["ts"].max() - tr["ts"].min()                    # same units as the engine's time

# estimator 1: log-linear regression on the empirical rate curve
d_lo, d_hi = reach.min(), reach.quantile(0.95)
grid = np.linspace(d_lo, d_hi, 25)
lam = np.array([(reach >= d).sum() / total_time for d in grid])
slope, intercept = np.polyfit(grid, np.log(lam), 1)
kappa_ols, A_ols = -slope, np.exp(intercept)

# estimator 2: exponential MLE with left truncation at d_lo
shifted = reach - d_lo
kappa_mle = 1.0 / shifted.mean()
se = kappa_mle / np.sqrt(len(shifted))
print("kappa OLS", kappa_ols, " kappa MLE", kappa_mle, "+/-", se, " A (OLS)", A_ols)

# THE important plot: log lambda vs delta must look like a straight line
plt.semilogy(grid, lam, "o", label="empirical")
plt.semilogy(grid, A_ols * np.exp(-kappa_ols * grid), label="fit")
plt.xlabel("delta (price units)"); plt.ylabel("fill rate"); plt.legend()
plt.savefig("results/kappa_fit.png", dpi=120)

json.dump({"kappa": float(kappa_mle), "A": float(A_ols)}, open("results/params.json", "w"))