import argparse
from dq.stats import read_rows, total_revenue

def _parse():
    p = argparse.ArgumentParser()
    p.add_argument("--csv", required=True)
    return p.parse_args()

def test_small_dataset_totals():
    args = _parse()
    rows = read_rows(args.csv)
    assert len(rows) == 3
    assert float(total_revenue(rows)) == 70.0  # 2*10 + 1*20 + 3*10
