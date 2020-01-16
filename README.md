# Qt5 OpenGL

Qt5写的一些OpenGL小例子集合,
参考[LearnOpenGL教程](http://bullteacher.com/category/zh_learnopengl_com)

## License
[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://github.com/jaredtao/HelloDirectX/blob/master/LICENSE)

# CI徽章

|[License][license-link]| [Windows][win-link]|[MacOS][macos-link]| [Ubuntu][ubuntu-link]|[Android][android-link]|[IOS][ios-link]|
|---------------|---------------|---------------|-----------------|-----------------|----------------|
|![license-badge] | ![win-badge]  |![macos-badge] | ![ubuntu-badge]      | ![android-badge]   |![ios-badge]   |



[win-link]: https://github.com/JaredTao/QtOpenGL/actions?query=workflow%3AWindows "WindowsAction"
[win-badge]: https://github.com/JaredTao/QtOpenGL/workflows/Windows/badge.svg  "Windows"

[ubuntu-link]: https://github.com/JaredTao/QtOpenGL/actions?query=workflow%3AUbuntu "UbuntuAction"
[ubuntu-badge]: https://github.com/JaredTao/QtOpenGL/workflows/Ubuntu/badge.svg "Ubuntu"

[macos-link]: https://github.com/JaredTao/QtOpenGL/actions?query=workflow%3AMacOS "MacOSAction"
[macos-badge]: https://github.com/JaredTao/QtOpenGL/workflows/MacOS/badge.svg "MacOS"

[android-link]: https://github.com/JaredTao/QtOpenGL/actions?query=workflow%3AAndroid "AndroidAction"
[android-badge]: https://github.com/JaredTao/QtOpenGL/workflows/Android/badge.svg "Android"

[ios-link]: https://github.com/JaredTao/QtOpenGL/actions?query=workflow%3AIOS "IOSAction"
[ios-badge]: https://github.com/JaredTao/QtOpenGL/workflows/IOS/badge.svg "IOS"

[license-link]: https://github.com/jaredtao/QtOpenGL/blob/master/LICENSE "LICENSE"
[license-badge]: https://img.shields.io/badge/license-MIT-blue.svg "MIT"

## 目录

* HelloTexture    

   以QOpenGLWidget为框架，简单的纹理贴图,使用定时器控制纹理旋转。
   ![HelloTexture](img/HelloTexture.png)

* HelloCube

   立方体贴图的例子，自己用画板画了一个图，然后贴成一个Cube。可以用鼠标控制立方体旋转。
   ![HelloCube](img/HelloCube.png)

* HelloMix   

    绘制了10个立方体，每个立方体的每个表面使用2张纹理混合贴图。混合比例按数字键1/2进行调节。
    ![HelloMix](img/HelloMix.png)

* QuickOpenGL

    这个是在不了解QML Quick内部原理的时候，在qml中添加自定义OpenGL渲染控件，用的是QQuickWindow的beforeRendering信号。后来知道了QQuickFramebufferObject之后，基本上这种方式就被抛弃了。这里保留项目仅作为观赏，建议参考下面FrameBufferObject。
    ![QuickOpenGL](img/QuickOpenGL.png)
* FrameBufferObject

    效果与上一个例子QuickOpenGL 类似，多了全屏放大功能,纹理沿着三个坐标轴旋转的功能。

    使用QQuickFramebufferObject 作为OpenGL自绘制控件(相当于QQuickItem了)。

    QQuickFramebufferObject是 Qt封装好的，需要理解其使用架构。

    这里大概说一下用法：
    1. 继承QQuickFramebufferObject实现一个自定义item,并注册到QML中使用
    2. 继承QQuickFramebufferObject::Renderer,实现一个自定义Renderer
    3. 自定义Renderer内部再封装一个OpenGL相关的渲染类。Renderer是在Scene Graph线程工作，始终拥有OpenGL上下文环境，所以渲染类写在Renderer内部，非常方便。
    4. Renderer 有个函数叫void synchronize(QQuickFramebufferObject *item), 可以用来做线程同步
    (Item工作在主线程，Renderer 工作在Scene Graph绘图线程)
    我用的时候，是把参数item可以强制转换成我从Item派生出来的子类，然后用里面的数据和这个Renderer交互就好了。

![FrameBufferObject](img/FrameBufferObject.png)
* light

    Shader光照的例子。  一个立方体的前面, 放一个移动的光源。
    ![light](img/light.png)
* lightCasters

  手电筒光源

    ![lightCasters](img/lightCasters.png)

* mulitLight

  镜面反射、漫反射，手电筒的集合
  ![mulitLight](img/mulitLight.png)

* material

  材质的使用,边缘部分发光，中间部分不发光

  ![material](img/material.png)

* DepthTest

  利用深度测试，后面的箱子边缘部分用红色绘制出来

  ![DepthTest](img/DepthTest.png)

* SkyBox
 
    天空盒， 可以键盘和鼠标操作

  ![SkyBox](img/SkyBox.png)

* HelloFBO

    帧缓冲的使用
    
    反相处理效果如下:

  ![HelloFBO](img/HelloFBO.png)

# 赞助

如果您觉得这个项目还不错，请打赏一下作者。

<img src="https://gitee.com/jaredtao/jaredtao/raw/master/img/weixin.jpg?raw=true" width="25%" height="25%" /><img src="https://gitee.com/jaredtao/jaredtao/raw/master/img/zhifubao.jpg?raw=true" width="25%" height="25%" />

# 联系作者

|-|-|
| ---- | -------------------------------- |
| 作者 | 武威的涛哥                           |
| 博客 | https://jaredtao.github.io/ |
| 博客-gitee镜像|https://jaredtao.gitee.io|
| 知乎专栏| https://zhuanlan.zhihu.com/TaoQt |
| QQ群| 734623697(高质量群，大佬多、不灌水）|
| 邮箱 | jared2020@163.com                |
| 微信 | xsd2410421                       |
| QQ、TIM | 759378563                      |
