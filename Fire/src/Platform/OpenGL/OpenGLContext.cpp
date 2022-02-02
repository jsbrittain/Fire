#include "frpch.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Fire {

	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_windowHandle(windowHandle)
	{
		FR_CORE_ASSERT(windowHandle,"Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		FR_PROFILE_FUNCTION();

		glfwMakeContextCurrent(m_windowHandle);
		int status = gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
		FR_CORE_ASSERT(status,"Failed to initialise Glad!");

		FR_CORE_INFO("OpenGL Info:");
		FR_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		FR_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		FR_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

	#ifdef FR_ENABLE_ASSERTS
 		int versionMajor;
 		int versionMinor;
 		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
 		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
 		FR_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 1), "Fire requires at least OpenGL version 4.1!");
 	#endif
	}

	void OpenGLContext::SwapBuffers()
	{
		FR_PROFILE_FUNCTION();
		
		glfwSwapBuffers(m_windowHandle);
	}

}