import qbs 1.0
Product {
    type:"application"
    Depends {
        name:"cpp"
    }
    cpp.cxxLanguageVersion:"c++11"
    Depends {
        name:"Qt"
        submodules: ["core", "gui", "widgets", "quick", "qml"]
    }
    Group {
        files: [
            "src/easy.h",
            "src/render.cpp",
            "src/render.h",
            "src/main.cpp",
            "src/shader.cpp",
            "src/shader.h",
        ]
        name:"Sources"
    }
    Group {
        files: [
            "res/img.qrc",
            "res/qml.qrc",
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
