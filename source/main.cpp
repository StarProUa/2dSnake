#include "GLFW/glfw3.h"

#include <iostream>
#include <vector>

#include "application.h"

using namespace std;

class Snake
{
	friend class Game;
protected:
	struct Body
	{
		int x, y;
	};

	int dx, dy;
	vector<Body> body;

public:
	Snake()
	{
		Body temp;

		temp.x = rand()% MapW;
		temp.y = rand()% MapH;

		body.push_back(temp);
	}
	void Draw()
	{
		glColor3f(1,0,0);

		for(int i = 0; i < body.size(); i++)
		{
			glPushMatrix();

			glTranslatef(body[i].x,body[i].y,0);
			Quad(0,0,1,1);

			glPopMatrix();
		}

		Move();
	}
	void setMove(const int &dx, const int &dy)
	{
		this->dx = dx;
		this->dy = dy;
	}
	bool Lose()
	{
		if(body[0].x > MapW || body[0].x < 0) return true;
		if(body[0].y > MapH-1 || body[0].y < 0) return true;

		for(int i = 3; i < body.size(); i++)
		{
			if(Collision(body[0].x, body[0].y, body[i].x, body[i].y)) return true;
		}
		return false;
	}
	void addBody()
	{
		Body temp;

		temp.x = body[body.size()-1].x;
		temp.y = body[body.size()-1].y;

		body.push_back(temp);
	}
private:
	void Move()
	{
		if(clockUnit)
		{
			for(int i = body.size() - 1; i > 0; i--)
			{
				body[i].x = body[i-1].x;
				body[i].y = body[i-1].y;
			}

			body[0].x += this->dx;
			body[0].y += this->dy;
		}
	}
};

class Apple
{
	friend class Game;
protected:
	int x, y;

public:
	Apple()
	{
		this->x = rand()% MapW;
		this->y = rand()% MapH;
	}
	void Draw()
	{
		glPushMatrix();

		glTranslatef(x,y,0);
		glColor3f(0,1,0);
		Quad(0,0,1,1);

		glPopMatrix();
	}
};

class Game : public Application
{
	Snake *snake;
	Apple *apple;
public:
	Game() : Application(630, 630, "Snake")
	{
		snake = new Snake();
		apple = new Apple();
	}

private:
	void Render() override
	{
		glPushMatrix();

		Cells();
		glRotatef(90, 0, 0, 1);
		glTranslatef(0, -MapW,0);
		Cells();

		glPopMatrix();

		snake->Draw();
		apple->Draw();

		if([this]() -> bool
			{
				return snake->body[0].x == apple->x && snake->body[0].y == apple->y;
			}())
		{
			snake->addBody();

			do
			{
				apple->x = rand()% MapW;
				apple->y = rand()% MapH;
			}
			while([this]() -> bool
					{
						for(int i = 0; i < snake->body.size(); i++)
						{
							if(apple->x == snake->body[i].x && apple->y == snake->body[i].y) return true;
						}
						return false;
					}());
		}
		if(snake->Lose())
		{
			delete snake;
			delete apple;

			newGame();
		}
	}
	void Keyboard(const int key, const int action) override
	{
		if(key == Key::Escape && action == Action::Press) Exit();
		if(key == Key::Space && action == Action::Press)
		{
			delete snake;
			delete apple;

			newGame();
		}
		if(key == Key::W && action == Action::Press) snake->setMove(0,1);
		if(key == Key::S && action == Action::Press) snake->setMove(0,-1);
		if(key == Key::A && action == Action::Press) snake->setMove(-1,0);
		if(key == Key::D && action == Action::Press) snake->setMove(1,0);
		if(key == Key::lControl && action == Action::Press) snake->setMove(0,0);
	}
	void newGame()
	{
		snake = new Snake;
		apple = new Apple;

		snake->setMove(0, 0);
	}
};

int main()
{
	srand(time(NULL));

	try
	{
		Game gsnake;
		gsnake.Run();
	}
	catch (runtime_error &error)
	{
		cout << error.what() << endl;
		return -1;
	}

	return 0;
}

