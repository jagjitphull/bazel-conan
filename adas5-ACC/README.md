Adaptive Cruise Control (ACC) — Toy Model

Concept
The system adjusts the ego vehicle’s speed to maintain a safe headway (distance to the lead vehicle).
It tries to avoid collisions while keeping traffic flow smooth.

Inputs
    ego_speed → current speed of our car
    lead_speed → speed of the car ahead
    distance → current gap to the lead car

Outputs
    "Accelerate" → if we are too far behind and safe to catch up
    "Hold" → if the distance is within the safe headway range
    "Brake" → if the gap is too small (too close to the lead car)


    Perfect 👍 Let’s prepare a **Markdown trainee handout** that contains the corrected files and exact build/run instructions.
This will serve as a step-by-step **lab reference** for Adaptive Cruise Control (ACC) using Bazel + Conan.

---

# 🚗 Adaptive Cruise Control (ACC) — Toy Model Lab (Bazel + Conan)

This lab implements a **rule-based Adaptive Cruise Control (ACC)** system in C++ and shows how to build it with **Bazel** and package it with **Conan**.

---

## 📂 Project Structure

```
adas5-ACC/
├─ .bazelrc
├─ MODULE.bazel
├─ WORKSPACE.bazel
├─ src/
│  ├─ acc/
│  │  ├─ BUILD.bazel
│  │  ├─ acc.h
│  │  └─ acc.cpp
│  └─ apps/
│     ├─ BUILD.bazel
│     └─ demo_acc.cpp
├─ tests/
│  ├─ BUILD.bazel
│  └─ acc_test.cpp
└─ conan_pkg/
   ├─ CMakeLists.txt
   └─ conanfile.py
```

---

## ⚙️ Bazel Configuration

### `.bazelrc`

```bash
build --cxxopt=-std=c++17
test  --cxxopt=-std=c++17
run   --cxxopt=-std=c++17
```

### `MODULE.bazel`

```bzl
module(
    name = "acc5_acc",
    version = "1.0.0",
    compatibility_level = 1,
)

bazel_dep(name = "rules_cc", version = "0.0.11")
```

### `WORKSPACE.bazel`

```bzl
# Minimal; dependencies are declared in MODULE.bazel
```

---

## 🛠️ ACC Library

### `src/acc/BUILD.bazel`

```bzl
cc_library(
    name = "acc",
    hdrs = ["acc.h"],
    srcs = ["acc.cpp"],
    include_prefix = "acc",   # Enables: #include "acc/acc.h"
    visibility = ["//visibility:public"],
)
```

### `src/acc/acc.h`

```cpp
#pragma once
#include <string>

namespace acc {

enum class Action { Accelerate, Hold, Brake };

struct AccParams {
    double time_headway = 1.8;   // seconds
    double min_gap      = 2.0;   // meters at standstill
    double accel_max    = 2.0;   // m/s^2
    double decel_max    = 3.0;   // m/s^2
    double deadband     = 1.0;   // meters
    double rel_speed_brake_bias = 0.5;
};

struct AccInput {
    double ego_speed_mps;   // Ego car speed (m/s)
    double lead_speed_mps;  // Lead car speed (m/s)
    double gap_m;           // Distance to lead car (m)
};

struct AccDecision {
    Action action;
    double suggested_accel_mps2; // Positive accel, negative brake
};

AccDecision decide(const AccInput& in, const AccParams& p = {});
std::string to_string(Action a);

} // namespace acc
```

### `src/acc/acc.cpp`

```cpp
#include "acc/acc.h"
#include <algorithm>
#include <cmath>

namespace acc {

static double clamp(double v, double lo, double hi) {
    return std::max(lo, std::min(v, hi));
}

std::string to_string(Action a) {
    switch (a) {
        case Action::Accelerate: return "Accelerate";
        case Action::Hold:       return "Hold";
        case Action::Brake:      return "Brake";
    }
    return "Hold";
}

AccDecision decide(const AccInput& in, const AccParams& p) {
    const double s_des = p.min_gap + in.ego_speed_mps * p.time_headway;
    const double err   = in.gap_m - s_des;
    const double dV    = in.lead_speed_mps - in.ego_speed_mps;

    double bias = (dV < 0.0) ? p.rel_speed_brake_bias * std::abs(dV) : 0.0;

    AccDecision out{Action::Hold, 0.0};

    if (err < -(p.deadband + bias)) {
        double need = -(err) / (p.time_headway + 1e-6);
        out.action = Action::Brake;
        out.suggested_accel_mps2 = -clamp(need, 0.0, p.decel_max);
    } else if (err > p.deadband && dV >= -0.5) {
        double want = err / (p.time_headway + 1e-6);
        out.action = Action::Accelerate;
        out.suggested_accel_mps2 = clamp(want, 0.0, p.accel_max);
    } else {
        out.action = Action::Hold;
        out.suggested_accel_mps2 = 0.0;
    }

    return out;
}

} // namespace acc
```

---

## 🚦 Demo Application

### `src/apps/BUILD.bazel`

```bzl
cc_binary(
    name = "demo_acc",
    srcs = ["demo_acc.cpp"],
    deps = ["//src/acc:acc"],
)
```

### `src/apps/demo_acc.cpp`

```cpp
#include <iostream>
#include <sstream>
#include "acc/acc.h"

int main() {
    using namespace acc;

    std::cout << "ACC demo. Enter: ego_mps lead_mps gap_m\n";
    std::cout << "Example: 20 18 25\nCtrl+D to end.\n\n";

    AccParams params;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        AccInput in{};
        if (!(iss >> in.ego_speed_mps >> in.lead_speed_mps >> in.gap_m)) {
            std::cerr << "Parse error. Use: ego_mps lead_mps gap_m\n";
            continue;
        }

        AccDecision d = decide(in, params);
        std::cout << "Input ego=" << in.ego_speed_mps
                  << " m/s, lead=" << in.lead_speed_mps
                  << " m/s, gap=" << in.gap_m
                  << " -> " << to_string(d.action)
                  << " (" << d.suggested_accel_mps2 << " m/s^2)\n";
    }
    return 0;
}
```

---

## ✅ Unit Tests

### `tests/BUILD.bazel`

```bzl
cc_test(
    name = "acc_test",
    srcs = ["acc_test.cpp"],
    deps = ["//src/acc:acc"],
)
```

### `tests/acc_test.cpp`

```cpp
#include "acc/acc.h"
#include <cassert>
#include <cmath>

int main() {
    using namespace acc;
    AccParams p;

    { // Too close → Brake
        AccInput in{25.0, 25.0, 5.0};
        auto d = decide(in, p);
        assert(d.action == Action::Brake);
    }

    { // Plenty of room → Accelerate
        AccInput in{15.0, 15.0, 60.0};
        auto d = decide(in, p);
        assert(d.action == Action::Accelerate);
    }

    { // At desired gap → Hold
        double s_des = p.min_gap + 20.0 * p.time_headway;
        AccInput in{20.0, 19.5, s_des};
        auto d = decide(in, p);
        assert(d.action == Action::Hold);
    }

    return 0;
}
```

---

## ▶️ Build & Run

```bash
# Build everything
bazel build //...

# Run demo
bazel run //src/apps:demo_acc
# Example input:
# 20 18 25
# 25 20 15
# 12 12 40

# Run tests
bazel test //tests:acc_test
```

---

## 📦 (Optional) Conan Packaging

### `conan_pkg/CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.15)
project(acc_lib CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(acc STATIC ../src/acc/acc.cpp)
target_include_directories(acc PUBLIC ../src)
```

### `conan_pkg/conanfile.py`

```python
from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout

class AccLibConan(ConanFile):
    name = "acc"
    version = "1.0.0"
    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "../src/acc/*", "CMakeLists.txt"

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy("acc.h", dst="include/acc", src="../src/acc")
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["acc"]
```

### Conan Build

```bash
cd conan_pkg
conan create . --version=1.0.0 --name=acc
conan list acc/1.0.0
```

---

## 📘 Summary for Trainees

* **ACC Logic**: maintains safe headway using simple rule-based control.
* **Inputs**: ego speed, lead speed, gap.
* **Outputs**: Accelerate, Hold, Brake (+ suggested acceleration).
* **Bazel**: manages build, tests, and demo execution.
* **Conan**: packages the ACC library for reuse.

