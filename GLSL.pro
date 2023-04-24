TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += \
    HelloTexture \
    HelloMix \
    HelloCube \
    HelloFBO \
    QuickOpenGL \
    light \
    lightCasters \
    mulitLight \
    FrameBufferObject \
    material \
    DepthTest \
    SkyBox \
    Instance

OTHER_FILES += \
    README.md \
    .clang-format \
    LICENSE \
    .github/workflows/*.yml
