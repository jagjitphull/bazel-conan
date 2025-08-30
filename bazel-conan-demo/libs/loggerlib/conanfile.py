from conan import ConanFile
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.files import collect_libs

class LoggerLibRecipe(ConanFile):
    name = "loggerlib"
    version = "1.0.0"
    package_type = "library"

    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": False, "fPIC": True}

    exports_sources = "CMakeLists.txt", "include/*", "src/*"

    def config_options(self):
        if self.settings.os == "Windows":
            del self.options.fPIC

    def layout(self):
        cmake_layout(self)

    def generate(self):
        tc = CMakeToolchain(self)
        # Respect shared/fPIC
        if "fPIC" in self.options:
            tc.variables["CMAKE_POSITION_INDEPENDENT_CODE"] = self.options.fPIC
        tc.variables["BUILD_SHARED_LIBS"] = self.options.shared
        # Install directly into the Conan package directory
        tc.variables["CMAKE_INSTALL_PREFIX"] = self.package_folder
        tc.generate()

    def build(self):
        cm = CMake(self)
        cm.configure()
        cm.build()

    def package(self):
        cm = CMake(self)
        cm.install()  # places libs in <pkg>/lib and headers in <pkg>/include

    def package_info(self):
        # Auto-detect actual produced library names from <pkg>/lib
        self.cpp_info.libs = collect_libs(self)
        # Optional, nice defaults for CMakeDeps find_package()
        self.cpp_info.set_property("cmake_file_name", "loggerlib")
        self.cpp_info.set_property("cmake_target_name", "loggerlib")

