import requests
import pandas as pd
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
    df = fetch_recent_trades(SYMBOL, LIMIT)
    df.to_csv("data/real_trades.csv", index=False)
    print(df.head())
    print(f"\n{len(df)} trades saved to python/real_trades.csv")

    fig_price = px.histogram(df, x="price", nbins=40, title="Price distribution")
    fig_price.write_html("python/real_price.html")

    fig_qty = px.histogram(df, x="qty", nbins=60, title="Qty distribution")
    fig_qty.write_html("python/real_qty.html")

    fig_qty_log = px.histogram(df, x="qty", nbins=60, log_y=True, title="Qty distribution (log y)")
    fig_qty_log.write_html("python/real_qty_log.html")

    fig_side = px.histogram(df, x="side", title="Buy vs sell count")
    fig_side.write_html("python/real_side.html")

    df["gap_ms"] = df["time"].diff().dt.total_seconds() * 1000
    fig_gap = px.histogram(df.dropna(), x="gap_ms", nbins=60, log_y=True, title="Time between trades (ms)")
    fig_gap.write_html("python/real_gap.html")

    print("HTML files written to python/")

if __name__ == "__main__":
    main()