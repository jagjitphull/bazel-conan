from conan import ConanFile
from conan.tools.cmake import CMake, cmake_layout

class LoggerLibRecipe(ConanFile):
    name = "loggerlib"
    version = "1.0.0"
    license = "Proprietary"
    url = "https://git.example.com/yourorg/loggerlib"
    description = "Internal logging library"
    settings = "os", "compiler", "build_type", "arch"
    exports_sources = "CMakeLists.txt", "include/*", "src/*"
    generators = "CMakeToolchain", "CMakeDeps"

    def layout(self):
        cmake_layout(self)

    def build(self):
        cm = CMake(self)
        cm.configure()
        cm.build()

    def package(self):
        cm = CMake(self)
        cm.install()   # relies on the install() rules you added in CMakeLists

    def package_info(self):
        self.cpp_info.libs = ["loggerlib"]

