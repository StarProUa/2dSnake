#include "GLFW/glfw3.h"
#include <iostream>
#include <ctime>
#include <vector>

#include "application.h"

using namespace std;

#define MapW 10
#define MapH 10

GLFWwindow *window;

int screenWidth = 630, screenHeight = 630;

class Game : public Application
{
public:

};

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

class Snake;

class Apple
{
	int x, y;
public:

	bool CollisionApple(Snake &snake);
	bool EatApple(Snake &snake);

	void NewApple();

	void Draw()
	{
		glPushMatrix();
		glTranslatef(x,y,0);
		glColor3f(0,1,0);
		Quad(0,0,1,1);
		glPopMatrix();
	}
};

class Snake
{
	struct Tbody
	{
		int x, y;
	};
	double old_time;
	int dx, dy;
public:
	int tail;
	vector<Tbody>body;
	double sleep;

	void NewSnake()
	{
		Tbody temp;
		temp.x = rand()% MapW;
		temp.y = rand()% MapH;
		body.push_back(temp);
	}
	void Draw()
	{
		glColor3f(1,0,0);
		for(int i = 0; i < tail; i++)
		{
			glPushMatrix();
			glTranslatef(body[i].x,body[i].y,0);
			Quad(0,0,1,1);
			glPopMatrix();
		}

	}
	double Time()
	{
		double this_time = glfwGetTime();
		double delta_time = this_time - old_time;
		sleep -= delta_time;
		old_time = this_time;
		return sleep;
	}
	void SetMove(int dx, int dy)
	{
		this->dx = dx;
		this->dy = dy;
	}
	void Move()
	{
		if(Time() <= 0)
		{
			for(int i = tail - 1; i > 0; i--)
			{
				body[i].x = body[i-1].x;
				body[i].y = body[i-1].y;
			}

			body[0].x += this->dx;
			body[0].y += this->dy;

			sleep = 0.2;
		}
	}
	bool Loop()
	{
		if(body[0].x > MapW || body[0].x < 0)
		{
			return true;
		}
		if(body[0].y > MapH-1 || body[0].y < 0)
		{
			return true;
		}
		return false;
	}
	void AddTail()
	{
		Tbody temp;
		temp.x = body[tail-1].x;
		temp.y = body[tail-1].y;
		body.push_back(temp);
		tail++;
	}
	bool Lose()
	{
		for(int i = 3; i < tail; i++)
		{
			if(body[0].x == body[i].x && body[0].y == body[i].y)
			{
				return true;
			}
		}
		return false;
	}
};

Snake snake;
Apple apple;

bool Apple::EatApple(Snake &snake)
{
	return snake.body[0].x == this->x && snake.body[0].y == this->y;
}

bool Apple::CollisionApple(Snake &snake)
{
	bool temp = true;
	for(int i = 0; i < int(snake.body.size()); i++)
	{
		if(this->x == snake.body[i].x && this->y == snake.body[i].y)
		{
			temp = false;
		}
	}
	return temp;
}

void Apple::NewApple()
{
	do
	{
		this->x = rand()% MapW;
		this->y = rand()% MapH;
	}
	while(!CollisionApple(snake));
}

void Resize(GLFWwindow *window, int w, int h)
{
	glViewport(0, 0, w, h);
	glfwGetWindowSize(window, &w, &h);
	glLoadIdentity();

	screenWidth = w;
	screenHeight = h;

	if(w <= h)
	{
		float h_res = h / (float)w;
		//glOrtho(1, 1, -h_res, h_res, 0, 1);
		glOrtho(0, MapW, 0, h_res * MapH, 0, 1);
	}
	else
	{
		float w_res = w / (float)h;
		//glOrtho(-w_res, w_res, -1, 1, 0, 1);
		glOrtho(0, w_res * MapW, 0, MapH, 0, 1);
	}
}

void NewGame()
{
	snake.body.clear();
	snake.sleep = 0.2;
	snake.tail = 1;
	Resize(window, 630, 630);
	snake.NewSnake();
	apple.NewApple();
	snake.SetMove(0,0);
}

void Cells()
{
	glPushMatrix();
	glColor3f(0,0,0);
	for(int i = 0; i < MapW-1; i++)
	{
		glTranslatef(1,0,0);
		Line(0,-1,0,MapH);
	}
	glPopMatrix();
}

void Draw()
{
	glPushMatrix();
	Cells();
	glRotatef(90,0,0,1);
	glTranslatef(0,-MapW,0);
	Cells();
	glPopMatrix();
	snake.Draw();
	snake.Move();
	apple.Draw();
	if(apple.EatApple(snake))
	{
		snake.AddTail();
		apple.NewApple();
	}

	if(snake.Lose() || snake.Loop())
	{
		NewGame();
	}

}

void Keyboard(GLFWwindow *window, int key, int scode, int action, int mode)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwTerminate();
		glfwWindowShouldClose(window);
		exit(0);
	}
	if(key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		NewGame();
	}
	if(key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		snake.SetMove(0,1);
	}
	if(key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		snake.SetMove(0,-1);
	}
	if(key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		snake.SetMove(-1,0);
	}
	if(key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		snake.SetMove(1,0);
	}
	if(key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		for(int i = 0; i < snake.tail; i++)
		{
			cout << i << " - " << snake.body[i].x << "  " << snake.body[i].y << endl;
		}

	}
	if(key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS)
	{
		snake.SetMove(0,0);
	}
}

int main()
{
	srand(time(NULL));
	if(!glfwInit())
	{
		cout << "Failed to init library";
		return -1;
	}

	window = glfwCreateWindow(screenWidth, screenHeight, "Snake2D", nullptr, nullptr);

	if(!window)
	{
		cout << "Error create windwo";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glClearColor(0.9,0.9,0.9,1);

	glfwSetKeyCallback(window,Keyboard);
	glfwSetWindowSizeCallback(window, Resize);

	NewGame();

	while(!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT);

		Draw();

		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwTerminate();
	return 0;
}

