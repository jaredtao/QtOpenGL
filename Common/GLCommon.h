#pragma once

class GLCommon
{
public:
	//准备GL环境。人为约定GL版本号，Desktop环境使用3.3 core, Android使用 ES 3.0
	static void prepareGL();
	//准备App环境。
	static void prepareApp();
};
