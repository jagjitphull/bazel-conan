import argparse
from dq.stats import read_rows, total_revenue, revenue_by_region

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--csv", required=True, help="Path to CSV (passed by Bazel)")
    args = ap.parse_args()

    rows = read_rows(args.csv)
    rev = total_revenue(rows)
    by_region = revenue_by_region(rows)
    top_region = max(by_region, key=by_region.get)

    print(f"rows={len(rows)}")
    print(f"total_revenue={rev:.2f}")
    print("revenue_by_region=" + ",".join(f"{k}:{v:.2f}" for k, v in sorted(by_region.items())))
    print(f"top_region={top_region}")

if __name__ == "__main__":
    main()
