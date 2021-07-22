#include <SFML/Graphics.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
using namespace sf;
using namespace std;
const int NN = 100000;
int N;
Vector2f func[NN];
Vector2f f[NN];
int main()
{
	VertexArray funcline(LineStrip, 0);
	int funci = 0;
	bool begin = 0, end = 0;
	double wx, wy;
	ifstream num("C:/Users/hayk/source/c++/repos/SfmlFurierSeries/num.txt");
	num >> N;
	num >> wx >> wy;
	RenderWindow window(VideoMode(wx, wy), "funcmaker");
	window.setFramerateLimit(60);
	RectangleShape bg(Vector2f(wx, wy));
	bg.setOrigin(wx / 2.0, wy / 2.0);
	bg.setPosition(0, 0);
	bg.setFillColor(Color::White);
	View view(Vector2f(0, 0), Vector2f(wx, wy));
	CircleShape pointoo(2), pointo1(2), point1o(2), pointom1(2), pointm1o(2);
	pointoo.setOrigin(2, 2);
	pointo1.setOrigin(2, 2);
	point1o.setOrigin(2, 2);
	pointom1.setOrigin(2, 2);
	pointm1o.setOrigin(2, 2);
	pointoo.setPosition(0, 0);
	pointo1.setPosition(0, 100);
	point1o.setPosition(100, 0);
	pointom1.setPosition(0, -100);
	pointm1o.setPosition(-100, 0);
	pointoo.setFillColor(Color(0, 0, 0, 255));
	pointo1.setFillColor(Color(0, 0, 0, 255));
	point1o.setFillColor(Color(0, 0, 0, 255));
	pointom1.setFillColor(Color(0, 0, 0, 255));
	pointm1o.setFillColor(Color(0, 0, 0, 255));
	window.setView(view);
	int k, a, b, fi = 0;
	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::MouseButtonPressed)
				begin = 1;
			if (event.type == Event::MouseButtonReleased)
				end = 1;
		}
		if (Mouse::isButtonPressed(Mouse::Left) && begin && !end)
		{
			funcline.resize(funci + 1);
			funcline[funci].color = Color(0, 255, 0);
			double x = Mouse::getPosition(window).x;
			double y = Mouse::getPosition(window).y;
			x -= wx / 2.0;
			y -= wy / 2.0;
			funcline[funci].position = Vector2f(x, y);
			y = -y;
			x /= 100.0;
			y /= 100.0;
			func[funci] = Vector2f(x, y);
			cout << x << ' ' << y << endl;
			funci++;
		}
		if (end)
		{
			bg.setFillColor(Color(255, 255, 255, 128));
			if (Keyboard::isKeyPressed(Keyboard::N))
			{
				end = 0;
				begin = 0;
				bg.setFillColor(Color(255, 255, 255, 255));
				for (int i = 0; i < funci; i++) func[i] = Vector2f(0, 0);
				funci = 0;
				funcline.resize(0);
			}
			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				ofstream inputfile("C:/Users/hayk/source/c++/repos/SfmlFurierSeries/inputfile.txt");
				k = N;
				b = funci;
				for (int i = 0; i < funci; i++)
				{
					a = k / b;
					for (int j = 0; j < a; j++)
					{
						f[fi] = func[i] + float(double(j) / double(a))*(func[(i + 1) % funci] - func[i]);
						fi++;
					}
					k -= a;
					b--;
				}
				for (int i = 0; i < N; i++) inputfile << f[i].x << ' ' << f[i].y << endl;
				RenderTexture t;
				t.create(wx, wy);
				t.clear(Color::White);
				t.setView(view);
				t.draw(pointoo);
				t.draw(pointo1);
				t.draw(point1o);
				t.draw(pointom1);
				t.draw(pointm1o);
				t.draw(funcline);
				t.display();
				t.getTexture().copyToImage().saveToFile("funcimage.png");
				inputfile.close();
				window.close();
			}
		}

		window.clear();
		window.draw(bg);
		window.draw(pointoo);
		window.draw(pointo1);
		window.draw(point1o);
		window.draw(pointom1);
		window.draw(pointm1o);
		window.draw(funcline);
		window.display();
	}
	


	return 0;
}
