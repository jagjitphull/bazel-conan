from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, CMakeDeps, cmake_layout

class HelloConan(ConanFile):
    name = "hello"
    version = "1.0.0"

    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "with_fmt": [True, False]}
    default_options = {"shared": False, "with_fmt": False}

    exports_sources = "CMakeLists.txt", "src/*", "include/*"

    def layout(self):
        # creates the standard folders: src, build/<conf>, generators, etc.
        cmake_layout(self)

    def requirements(self):
        if self.options.with_fmt:
            self.requires("fmt/11.0.2")

    def generate(self):
        # THIS is what creates CMakePresets.json in the 'generators' folder
        tc = CMakeToolchain(self)
        tc.generate()
        deps = CMakeDeps(self)
        deps.generate()

    def build(self):
        cm = CMake(self)
        cm.configure()
        cm.build()

    def package(self):
        cm = CMake(self)
        cm.install()

    def package_info(self):
        self.cpp_info.libs = ["hello"]



