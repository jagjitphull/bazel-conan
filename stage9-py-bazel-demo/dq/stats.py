from decimal import Decimal
import csv
from collections import defaultdict

def read_rows(path):
    rows = []
    with open(path, newline="", encoding="utf-8") as f:
        r = csv.DictReader(f)
        for row in r:
            row["qty"] = int(row["qty"])
            row["price"] = Decimal(row["price"])
            rows.append(row)
    return rows

def total_revenue(rows):
    return sum(Decimal(r["qty"]) * r["price"] for r in rows)

def revenue_by_region(rows):
    acc = defaultdict(Decimal)
    for r in rows:
        acc[r["region"]] += Decimal(r["qty"]) * r["price"]
    return dict(acc)
