from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout

class GreetLibConan(ConanFile):
    name = "greetlib"
    version = "1.0.0"
    settings = "os", "arch", "compiler", "build_type"
    options = {"uppercase": [True, False], "shared": [True, False]}
    default_options = {"uppercase": False, "shared": False}
    exports_sources = "CMakeLists.txt", "include/*", "src/*"
    package_type = "library"
    generators = "CMakeDeps"  # CMakeToolchain via generate()

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        tc.cache_variables["UPPERCASE"] = "ON" if self.options.uppercase else "OFF"
        tc.cache_variables["BUILD_SHARED_LIBS"] = "ON" if self.options.shared else "OFF"
        tc.generate()

    def build(self):
        cm = CMake(self)
        cm.configure()
        cm.build()

    def package(self):
        cm = CMake(self)
        cm.install()

    def package_info(self):
        self.cpp_info.libs = ["greet"]
        # Make the target name explicit and stable:
        # This creates IMPORTED target: greetlib::greetlib
        self.cpp_info.set_property("cmake_target_name", "greetlib::greetlib")





