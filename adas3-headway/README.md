Level-1 ADAS module: Headway + FCW/ABW (audible/visual warning).

Level 1 — Core sensing & safety math

1) Headway & FCW+ABW (audible/visual warn)
    What: Extend TTC with time-gap (headway = distance/ego_speed) and dual thresholds.
    Conan: eigen/3.4.0, fmt/11.x, spdlog/1.x
    Bazel: cc_library(:adas_headway), cc_binary(:demo_headway)
    Extras: log CSV for plotting.

2) Brake Distance Model + AEB trigger
    What: Compute stopping distance with jerk-limited decel; trigger AEB if stopping distance ≥ gap.
    Conan: Eigen, fmt

############################
What this adds
############################

Headway (time gap): headway = distance / ego_speed
TTC (closing only): ttc = distance / (ego_speed − lead_speed)

Warnings
    FCW (Forward-Collision Warning) from TTC thresholds
    ABW (Audible/Beep Warning) from either short headway or short TTC

Design goals: tiny, testable, no runtime deps beyond the standard library.

src/
├─ adas/
│  ├─ acc.h                 (existing from earlier lab)
│  ├─ acc.cpp               (existing)
│  ├─ headway_fcw.h         (NEW)
│  └─ headway_fcw.cpp       (NEW)
└─ apps/
   └─ demo_headway.cpp      (NEW)

#################BUILD###################
bazel build //:demo_headway
./bazel-bin/demo_headway

You’ll see a table with Headway, TTC, and three warnings: FCW, HW-W, and combined ABW (with a console “BEEP” when caution/imminent).

bazel-bin/demo_headway
 step | dist(m) | v_ego | v_lead |  HW(s) |  TTC(s) | FCW  | HW-W |  ABW
------|---------|-------|--------|--------|---------|------|------|------
    0 |      36 |    18 |     18 |   2.00 |   --   |   NONE |   NONE |   NONE
    1 |      30 |    18 |     18 |   1.67 |   --   |   NONE |   NONE |   NONE
    2 |      27 |    20 |     18 |   1.35 |  13.50 |   NONE | CAUTION | CAUTION
  >>> BEEP (CAUTION)
    3 |      24 |    22 |     18 |   1.09 |   6.00 |   NONE | CAUTION | CAUTION
  >>> BEEP (CAUTION)
    4 |      20 |    22 |     18 |   0.91 |   5.00 |   NONE | CAUTION | CAUTION
  >>> BEEP (CAUTION)
    5 |      16 |    22 |     18 |   0.73 |   4.00 |   NONE | IMMINENT | IMMINENT
  >>> BEEP (IMMINENT)
    6 |      12 |    22 |     18 |   0.55 |   3.00 | CAUTION | IMMINENT | IMMINENT
  >>> BEEP (IMMINENT)
    7 |       9 |    22 |     18 |   0.41 |   2.25 | CAUTION | IMMINENT | IMMINENT
  >>> BEEP (IMMINENT)
    8 |       6 |    22 |     18 |   0.27 |   1.50 | IMMINENT | IMMINENT | IMMINENT
  >>> BEEP (IMMINENT)

Legend: FCW uses TTC thresholds; HW-W uses headway thresholds; ABW is the stricter of the two.

########################################

#############################################
How it works (quick explanation)
###############################################
Headway (time gap): distance / ego_speed.
    Independent of whether you’re closing.
    If headway gets too small, ABW can already alert—even before TTC becomes small.

TTC (closing only): distance / (ego − lead).
    Only valid if ego is faster than lead and distance > 0.
    Drives FCW severity.

Aggregation (ABW): picks the more severe of (FCW, HeadwayWarning).
    Rationale: a short time-gap is unsafe even during steady following; TTC captures imminent collision when closing.