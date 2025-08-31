from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout

class DemoAppConan(ConanFile):
    name = "demoapp"
    version = "0.1.0"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def layout(self):
        cmake_layout(self)

    def requirements(self):
        self.requires("fmt/11.0.2")         # from ConanCenter
        self.requires("loggerlib/1.0.0")    # from your private remote

    def build(self):
        cm = CMake(self)
        cm.configure()
        cm.build()

