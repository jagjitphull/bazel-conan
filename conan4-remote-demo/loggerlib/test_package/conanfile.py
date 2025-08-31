from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout

class LoggerLibTestPackage(ConanFile):
    settings = "os", "arch", "compiler", "build_type"
    generators = "CMakeToolchain", "CMakeDeps"
    test_type = "explicit"

    def requirements(self):
        self.requires(self.tested_reference_str)

    def layout(self):
        cmake_layout(self)

    def build(self):
        cm = CMake(self)
        cm.configure()
        cm.build()

    def test(self):
        # CTest or run the binary (kept simple)
        pass
