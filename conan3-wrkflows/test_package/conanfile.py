from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout

class HelloTestConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"  # ok to declare here
    test_type = "explicit"  # optional, but nice

    def requirements(self):
        self.requires(self.tested_reference_str)

    def layout(self):
        cmake_layout(self)

    def build(self):
        cm = CMake(self)
        cm.configure()
        cm.build()

    def test(self):
        # Let CTest run the built binary
        cm = CMake(self)
        cm.test()
