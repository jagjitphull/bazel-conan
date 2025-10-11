self-contained ADAS demo (Forward-Collision Warning + simple constant-velocity Kalman filter) in C++ that builds with Bazel,
while Conan supplies the third-party dependency (Eigen, header-only).

Install deps, build, and run

#############################################################

# 1) Get dependencies via Conan (Eigen)
chmod +x tools/fetch_deps.sh
./tools/fetch_deps.sh

# 2) Build with Bazel
bazel build //:adas_demo

# 3) Run
./bazel-bin/adas_demo
#################################################################

./tools/fetch_deps.sh
bazel build //:adas_demo

o/p

/bazel-bin/adas_demo
 step | dist_meas | dist_filt | rel_v_filt | TTC(s) | WARN
------|-----------|-----------|------------|--------|------
    0 |     40.00 |     40.00 |       0.00 | 10.000000 | NONE
    1 |     36.00 |     37.91 |      -1.18 | 9.000000 | NONE
    2 |     31.00 |     33.65 |      -3.66 | 7.750000 | NONE
    3 |     27.00 |     28.87 |      -5.72 | 6.750000 | NONE
    4 |     24.00 |     24.76 |      -6.59 | 6.000000 | NONE
    5 |     20.00 |     20.55 |      -7.23 | 5.000000 | NONE
    6 |     16.00 |     16.35 |      -7.63 | 4.000000 | NONE
    7 |     12.00 |     12.20 |      -7.86 | 3.000000 | CAUTION
    8 |      9.00 |      8.73 |      -7.55 | 2.250000 | CAUTION
    9 |      6.00 |      5.61 |      -7.09 | 1.500000 | IMMINENT

Legend: TTC thresholds -> Caution <= 3.0s, Imminent <= 1.5s

What this demonstrates ??

ADAS logic: Time-To-Collision risk classification (simple FCW).
Sensor smoothing: tiny Kalman filter using Eigen (from Conan).
Bazel + Conan: Bazel compiles everything; Conan only vendors headers into third_party/eigen/ using deploy generator—no Bazel rule plugins required, super simple and reliable.

###################################################################  EGO ###################################################
# What “ego” means in ADAS

In ADAS and autonomous driving software, **ego vehicle** = **the vehicle we are controlling** (the car whose sensors and software we’re running).

* **Ego speed (v_ego)** → the forward speed of *our own* car.
* **Lead speed (v_lead)** → the forward speed of the vehicle directly in front of us (target).

So in the formula:

[
TTC = \frac{d}{v_{\text{rel}}}
\quad \text{where} \quad v_{\text{rel}} = v_{\text{ego}} - v_{\text{lead}}
]

* **d** = distance between ego front bumper and lead rear bumper
* **v_ego** = ego vehicle’s speed
* **v_lead** = lead vehicle’s speed

---

# Why it’s defined this way

* If **ego > lead** → we are *closing in*. ( v_{rel} > 0 ). TTC is finite and positive.
* If **ego ≤ lead** → we’re not closing (same speed or pulling away). TTC is undefined or ∞ → no collision predicted.

---

# Quick Example

* Ego car: 20 m/s (~72 km/h)
* Lead car: 15 m/s (~54 km/h)
* Distance: 50 m

Relative speed:
[
v_{rel} = 20 - 15 = 5 \ \text{m/s}
]

Time-to-collision:
[
TTC = \frac{50}{5} = 10 \ \text{seconds}
]

Meaning: if both keep constant speeds, collision in ~10 s.

---

✅ So **ego** just means “your own car” in the ADAS system.

