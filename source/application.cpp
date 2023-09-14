#include "application.h"
#include "GLFW/glfw3.h"
#include <stdexcept>

#define WAIT 0.2

Application::Application(const int &width, const int &height, const char *title) : screenWidth(width), screenHeight(height)
{
	if(!glfwInit())
	{
		throw std::runtime_error("Failed to init library");
	}

	window = glfwCreateWindow(screenWidth, screenHeight, title, nullptr, nullptr);

	if(!window)
	{
		glfwTerminate();
		throw std::runtime_error("Error to create window");
	}

	glfwSetWindowUserPointer(window, this);

	glfwSetKeyCallback(window, keyboardCallback);
	glfwSetWindowSizeCallback(window, [](GLFWwindow *window, int width, int height)
	{
		glViewport(0, 0, width, height);
	});

	glfwMakeContextCurrent(window);

	glOrtho(0, MapW, 0, MapH, 0, 1);

	glClearColor(1, 1, 1, 0);

	sleepTime = WAIT;
	clockUnit = false;
}

void Application::Run()
{
	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		double nowTime = glfwGetTime();
		delta = nowTime - oldTime;

		glfwPollEvents();

		if((sleepTime -= delta) <= 0)
		{
			clockUnit = true;
			sleepTime = WAIT;
		}

		Render();

		glfwSwapBuffers(window);

		clockUnit = false;

		oldTime = nowTime;
	}
}

void Application::Exit()
{
	glfwSetWindowShouldClose(window, GLFW_TRUE);
}

Application::~Application()
{
	glfwTerminate();
}

void Application::keyboardCallback(GLFWwindow *window, int key, int code, int action, int mode)
{
	Application *app = static_cast<Application *>(glfwGetWindowUserPointer(window));
	app->Keyboard(key, action);
}

// other function

void Quad(float x1, float y1, float x2, float y2)
{
	glBegin(GL_POLYGON);
	glVertex2f(x1, y1);
	glVertex2f(x1, y2);
	glVertex2f(x2, y2);
	glVertex2f(x2, y1);
	glEnd();
}

void Line(float x1, float y1, float x2, float y2)
{
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}

bool Collision(const int &x1, const int &y1, const int &x2, const int &y2)
{
	return x1 == x2 && y1 == y2;
}

void Cells()
{
	glPushMatrix();

	glColor3f(0,0,0);
	for(int i = 0; i < MapW - 1; i++)
	{
		glTranslatef(1,0,0);
		Line(0,-1,0,MapH);
	}

	glPopMatrix();
}
