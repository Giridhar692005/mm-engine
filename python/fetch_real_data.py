import requests
import pandas as pd
import matplotlib.pyplot as plt
import plotly.express as px

SYMBOL = "BTCUSDT"
LIMIT = 1000  # max per request on this endpoint

def fetch_recent_trades(symbol: str, limit: int) -> pd.DataFrame:
    url = "https://api.binance.com/api/v3/trades"
    params = {"symbol": symbol, "limit": limit}
    response = requests.get(url, params=params)
    response.raise_for_status()
    raw = response.json()

    df = pd.DataFrame(raw)
    df["price"] = df["price"].astype(float)
    df["qty"] = df["qty"].astype(float)
    df["time"] = pd.to_datetime(df["time"], unit="ms")
    # isBuyerMaker=True means the trade was a sell hitting a resting buy
    df["side"] = df["isBuyerMaker"].map({True: "SELL", False: "BUY"})
    return df[["time", "side", "price", "qty"]]

def main():
    df = fetch_recent_trades(SYMBOL,LIMIT)
    df.to_csv("python/real_trades.csv", index=False)
    print(df.head())
    print(f"\n{len(df)} trades saved to python/real_trades.csv")
    fig = px.histogram(df, x="price", nbins=40, title="Price distribution")
    fig.show()
    fig.write_html("python/real_data_distributions.html")

if __name__ == "__main__":
    main()