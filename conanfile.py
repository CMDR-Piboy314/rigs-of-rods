import os
from conan import ConanFile
from conan.tools.files import copy


class RoR(ConanFile):
    name = "Rigs of Rods"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
        self.requires("angelscript/2.35.1")
        self.requires("discord-rpc/3.4.0@anotherfoxguy/stable")
        self.requires("fmt/8.0.1")
        self.requires("libcurl/7.79.1")
        self.requires("mygui/3.4.0@anotherfoxguy/stable")
        self.requires("ogre3d-caelum/0.6.3.1@anotherfoxguy/stable")
        self.requires("ogre3d-pagedgeometry/1.2.0@anotherfoxguy/stable")
        self.requires("ogre3d/1.11.6.1@anotherfoxguy/stable", force=True)
        self.requires("ois/1.4.1@rigsofrods/custom")
        self.requires("openal-soft/1.22.2")
        self.requires("openssl/1.1.1s", force=True)
        self.requires("rapidjson/cci.20200410")
        self.requires("socketw/3.11.0@anotherfoxguy/stable")

        self.requires("libpng/1.6.39", override=True)
        self.requires("libwebp/1.3.0", override=True)
        self.requires("zlib/1.2.13", override=True)

    def generate(self):
        for dep in self.dependencies.values():
            for f in dep.cpp_info.bindirs:
                self.cp_data(f)
            for f in dep.cpp_info.libdirs:
                self.cp_data(f)

    def cp_data(self, src):
        bindir = os.path.join(self.build_folder, "bin")
        copy(self, "*.dll", src, bindir, False)
        copy(self, "*.so*", src, bindir, False)
