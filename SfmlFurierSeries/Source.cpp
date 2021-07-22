#include <SFML/Graphics.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
using namespace sf;
using namespace std;
#define PI 3.1415926535
class Complex
{
public:
	double a, b;
	Complex()
	{

	}
	Complex(double a, double b)
	{
		this->a = a;
		this->b = b;
	}
	void operator=(Vector2f x)
	{
		a = x.x;
		b = x.y;
	}
	void operator=(Complex x)
	{
		a = x.a;
		b = x.b;
	}
	Complex operator+(Complex x)
	{
		Complex y;
		y.a = x.a + this->a;
		y.b = x.b + this->b;
		return y;
	}
	Complex operator-(Complex x)
	{
		Complex y;
		y.a = x.a - this->a;
		y.b = x.b - this->b;
		return y;
	}
	Complex operator*(Complex x)
	{
		Complex y;
		y.a = x.a * this->a - x.b * this->b;
		y.b = x.a * this->b + x.b * this->a;
		return y;
	}
	Complex operator/(Complex x)
	{
		Complex y;
		y.a = (x.a * this->a + x.b * this->b) / (x.a * x.a + x.b * x.b);
		y.b = (x.a * this->b - x.b * this->b) / (x.a * x.a + x.b * x.b);
		return y;
	}
	
};
const int NN = 100000;
Complex f[NN];
int N;
Complex arg(double a)
{
	Complex b = Complex(cos(a*2.0*PI), sin(a*2.0*PI));
	return b;
}
Complex findconst(int n)
{
	Complex c = Complex(0, 0);
	for (int i = 0; i < N; i++) c = c + f[i] * arg((double(i) / double(N))*double(n))*Complex((1.0 / double(N)), 0.0);
	return c;
}
Vector2f comptovec(Complex a)
{
	Vector2f b = Vector2f(a.a*100.0, -a.b*100.0);
	return b;
}
Vector2f findCenter(Vector2f center, double zoom, double wx, double wy)
{
	if (center.x <= (zoom - 1.0) * wx / 2.0)
		center.x = (zoom - 1.0) * wx / 2.0;
	if (center.x >= (1.0 - zoom) * wx / 2.0)
		center.x = (1.0 - zoom) * wx / 2.0;
	if (center.y <= (zoom - 1.0) * wy / 2.0)
		center.y = (zoom - 1.0) * wy / 2.0;
	if (center.y >= (1.0 - zoom) * wy / 2.0)
		center.y = (1.0 - zoom) * wy / 2.0;
	return center;
}
VertexArray funcline(LineStrip, 1), vectors(LineStrip, 2);
Complex consts[10001];
int main()
{
	ifstream inputfile("../inputfile.txt");
	ifstream num("../num.txt");
	double wx, wy;
	num >> N;
	num >> wx >> wy;
	double x, y;
	for (int i = 0; i < N; i++)
	{
		inputfile >> x >> y;
		f[i].a = x;
		f[i].b = y;
	}
	RenderWindow window(VideoMode(wx, wy), "SFML works!");
	window.setFramerateLimit(60);
	RectangleShape bg(Vector2f(wx*4.0, wy*4.0));
	bg.setOrigin(wx * 2.0, wy * 2.0);
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
	double zoom = 1;
	double dt = 50.0;
	double loop = 0;
	int nofc = 1;
	int ci = 1;
	consts[0] = findconst(0);
	int funci = 0;
	Complex topv = Complex(0, 0);
	vectors[0].position = Vector2f(0, 0);
	vectors[0].color = Color(0, 0, 255, 255);
	while (window.isOpen())
	{
		
		topv = Complex(0, 0);
		loop += dt;
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
			if (event.type == Event::MouseWheelScrolled)
				if (Keyboard::isKeyPressed(Keyboard::LControl))
				{
					view.zoom(pow(1.2, -event.mouseWheelScroll.delta));
					zoom *= pow(1.2, -event.mouseWheelScroll.delta);
					if (zoom > 1)
					{
						view.zoom(1.0 / zoom);
						zoom = 1;
					}
					
				}
				else
					dt *= pow(1.2, event.mouseWheelScroll.delta);
		}
		
		if (loop >= double(N))
		{
			loop = 0;
			nofc+=2;
			funcline.resize(1);
			funcline[0].position = Vector2f(0, 0);
			vectors.resize(nofc+1);
			consts[ci] = findconst((ci + 1) / 2);
			ci++;
			consts[ci] = findconst(-(ci + 1) / 2);
			ci++;
			funci = 0;
			
		}
		
		for (int i = 1; i <= nofc; i++)
		{
			topv = topv + consts[i - 1] * arg(-double(((i % 2 == 0) - (i % 2 == 1))*(i / 2))*(loop / double(N)));
			vectors[i].position = comptovec(topv);
			vectors[i].color = vectors[i - 1].color;
			
		}
		view.setCenter(findCenter(comptovec(topv), zoom, wx, wy));
		funcline.resize(funci + 1);
		funcline[funci].position = comptovec(topv);
		funcline[funci].color = Color(255, 0, 0, 255);
		funci++;
		window.setView(view);
		window.clear();
		window.draw(bg);
		window.draw(pointoo);
		window.draw(pointo1);
		window.draw(point1o);
		window.draw(pointom1);
		window.draw(pointm1o);
		window.draw(vectors);
		window.draw(funcline);
		window.display();
		cout << loop << ' ' << nofc << ' ' << topv.a << ' ' << topv.b << ' ' << zoom << endl;
		
	}
	Vector2f a;


	return 0;
}
