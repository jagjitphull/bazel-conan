AEB (Automatic Emergency Braking (AEB) Trigger) module.
It compares stopping distance vs gap and says “Brake now” when needed.

adas4-AEB/
├─ MODULE.bazel
├─ src/
│  ├─ adas/
│  │  ├─ aeb.h
│  │  ├─ aeb.cpp
│  │  └─ BUILD
│  └─ apps/
│     ├─ demo_aeb.cpp
│     └─ BUILD

bazel build //:demo_aeb
./bazel-bin/demo_aeb

