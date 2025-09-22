# conanfile.py

from conan import ConanFile
from conan.tools.cmake import CMakeDeps, CMakeToolchain, cmake_layout
import platform

class MyProjectConan(ConanFile):
    name = "myproject"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"
    requires = (
        "sqlitecpp/3.3.2",
        "nlohmann_json/3.12.0",
        "cpr/1.12.0",
        "gtest/1.17.0",
	"spdlog/1.15.3",
        "fmt/11.2.0"
    )

    def configure(self):
        if self.settings.os == "Windows":
            # на win10 не срабатывает, но пока оставим так
#            self.options["curl"].with_ssl = "winssl" 
            self.options["curl"].with_ssl = "openssl" 
	# OpenSSL является дефолтом для curl, подходит для других ОС

    def layout(self):
        cmake_layout(self)
