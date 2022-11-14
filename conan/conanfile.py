from conans import ConanFile


class EntityprotosConan(ConanFile):
    """ Conan recipe for Entityprotos """

    name = "entityprotos"
    license = "MIT License"
    url = "https://github.com/Adnn/entityprotos"
    description = "Repository of prototypes for entity ECS."
    #topics = ("", "", ...)
    settings = ("os", "compiler", "build_type", "arch")
    options = {
        "build_tests": [True, False],
        "shared": [True, False],
        "visibility": ["default", "hidden"],
    }
    default_options = {
        "build_tests": False,
        "shared": False,
        "visibility": "hidden"
    }

    requires = (
        "entity/83e1ade064@adnn/develop",
        "graphics/aa960a2385@adnn/develop",
        "handy/acd90c0549@adnn/develop",
        "math/541fce5f6a@adnn/develop",
    )

    # Note: It seems conventionnal to add CMake build requirement
    # directly to the build profile.
    #build_requires = ()

    build_policy = "missing"
    generators = "CMakeDeps", "CMakeToolchain"


    python_requires="shred_conan_base/0.0.3@adnn/stable"
    python_requires_extend="shred_conan_base.ShredBaseConanFile"
