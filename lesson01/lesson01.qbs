import qbs
CppApplication {
    Depends {
        name:"Qt"
        submodules: ["core", "gui", "widgets"]
    }
    cpp.cxxLanguageVersion:"c++11"
    Group {
        files: [
            "src/main.cpp",
            "src/window.cpp",
            "src/window.h",
        ]
        name:"Sources"

    }
    Group {
        files: [
            "res/img.qrc",
            "res/shader.qrc",
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
