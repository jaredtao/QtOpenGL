import qbs 1.0
Product {
    type:"application"
    Depends {
        name:"cpp"
    }
    cpp.cxxLanguageVersion: "c++11"

    Depends {
        name:"Qt"
        submodules: ["core", "gui", "widgets"]
    }
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
        fileTagsFilter: product.type
        qbs.install:true
        qbs.installDir:"install-root"
    }
}
