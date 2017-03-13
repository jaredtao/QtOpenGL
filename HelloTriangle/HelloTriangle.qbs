import qbs

Product {
    type:"application"
    Depends {
        name:"cpp"
    }
    cpp.cxxLanguageVersion: "c++11"
    //    Properties {
    //        condition: qbs.toolchain.contains('gcc') && qbs.hostOS.contains("osx")
    //        cpp.cxxFlags: [ "-std=c++11", "-stdlib=libc++" ]
    //    }
    //    Properties {
    //        condition: qbs.toolchain.contains('gcc') && !qbs.hostOS.contains("osx")
    //        cpp.cxxFlags: [ "-std=c++11" ]
    //    }

    Depends {
        name:"Qt"
        submodules: ["core", "gui", "widgets"]
    }

    Group {
        files: [
            "src/main.cpp",
            "src/window.cpp",
            "src/window.h",
        ]
        name: "Sources"

    }
    Group {
        files: [
            "res/res.qrc",
            "res/shader.qrc",
        ]
        name:"Resources"

    }

    Group {
        name: "Files to Install"
        fileTagsFilter: product.type
        qbs.install:true
        qbs.installDir:"install-root"
    }

}
