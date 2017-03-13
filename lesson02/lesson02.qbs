import qbs 1.0
CppApplication {
    Depends {
        name:"Qt"
        submodules: ["core", "gui", "widgets"]
    }
    cpp.cxxLanguageVersion:"c++11"
    Group {
        files: [
            "src/cube.cpp",
            "src/cube.h",
            "src/main.cpp",
            "src/window.cpp",
            "src/window.h",
        ]

        name:"Sources"

    }
    Group {
        files: [
            "res/shader.qrc",
            "res/texture.qrc",
        ]
        name:"Resources"
    }
    Group {
        name:"Files to install"
        fileTagsFilter: product.type
        qbs.install:true
        qbs.installDir:"install-root"
    }
}
