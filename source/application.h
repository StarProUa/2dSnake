#ifndef APPLICATION_H
#define APPLICATION_H

struct GLFWwindow;

#define MapW 10
#define MapH 10

enum Key
{
	W = 87,
	S = 83,
	D = 68,
	A = 65,

	P = 80,

	Space = 32,
	Escape = 256,
	lControl = 341,
};

enum Action
{
	Release = 0,
	Press,
	Repeat,
};

inline bool clockUnit;

class Application
{
	GLFWwindow *window;

	int screenWidth = 630, screenHeight = 630;

	double oldTime = 0;
	double sleepTime, delta;

protected:
	Application(const int &width, const int &height, const char *title);

	void Exit();

	virtual ~Application();

public:
	void Run();

	virtual void Render() = 0;
	virtual void Keyboard(const int key, const int action) = 0;

private:
	static void keyboardCallback(GLFWwindow *window, int key, int code, int action, int mode);
};

void Quad(float x1, float y1, float x2, float y2);

void Line(float x1, float y1, float x2, float y2);

bool Collision(const int &x1, const int &y1, const int &x2, const int &y2);

void Cells();

#endif // APPLICATION_H
