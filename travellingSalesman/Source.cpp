#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>
using std::cout;
using std::endl;
using std::vector;

const int MAX_INDEX = 10000;
int populationSize = 100;
int n;

struct Point {
	int x;
	int y;
	Point()
	{
		x = -1;
		y = -1;
	}
	Point(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	Point(const Point& other)
	{
		x = other.x;
		y = other.y;
	}
	Point& operator=(const Point& other)
	{
		if(this != &other)
		{
			x = other.x;
			y = other.y;
		}
		return *this;
	}
	bool operator==(const Point& other) const
	{
		return (x == other.x) && (y == other.y);
	}
};

class Path {
public:
	vector<Point> path;
	int fitnessValue;
	Path()
	{
		fitnessValue = -1;
	}
	Path(vector<Point> path)
	{
		this->path = path;
		fitnessValue = 0;
		fitness();
	}
	Path(const Path& other)
	{
		path = other.path;
		fitnessValue = other.fitnessValue;
	}
	Path& operator=(const Path& other)
	{
		if(this != &other)
		{
			path = other.path;
			fitnessValue = other.fitnessValue;
		}
		return *this;
	}
	bool operator< (const Path& other) const
	{
		return fitnessValue < other.fitnessValue;
	}
private:
	void fitness()
	{
		for(int i = 1; i < n; ++i)
		{
			fitnessValue += sqrt(abs(path[i - 1].x - path[i].x) * abs(path[i - 1].x - path[i].x) +
				abs(path[i - 1].y - path[i].y) * abs(path[i - 1].y - path[i].y));
		}
	}
};

vector<Point> points;
vector<Path> currentPopulation;

void generatePoints()
{
	for(int i = 0; i < n; ++i)
	{
		points.push_back(Point(std::rand() % MAX_INDEX, std::rand() % MAX_INDEX));
	}
}

void generateFirstPopulation()
{
	for(int i = 0; i < populationSize; ++i)
	{
		std::random_shuffle(points.begin(), points.end());
		currentPopulation.push_back(Path(points));
	}
}

Path crossover(const Path& firstParent, const Path& secondParent)
{
	int a = n / 4;
	int b = 4 * n / 5;
	vector<Point> child(n);

	for(int i = a; i <= b; ++i)
	{
		child[i] = firstParent.path[i];
	}
	int childIndex = b + 1;
	for(int i = b + 1; i < n; ++i)
	{
		bool pointInChiled = false;
		for(int j = 0; j < n && !pointInChiled; ++j)
		{
			if(secondParent.path[i] == child[j])
			{
				pointInChiled = true;
			}
		}
		if(!pointInChiled)
		{
			child[childIndex] = secondParent.path[i];
			childIndex = (childIndex == n - 1) ? 0 : childIndex + 1;
		}
	}
	for(int i = 0; i < n; ++i)
	{
		bool pointInChiled = false;
		for(int j = 0; j < n && !pointInChiled; ++j)
		{
			if(secondParent.path[i] == child[j])
			{
				pointInChiled = true;
			}
		}
		if(!pointInChiled)
		{
			child[childIndex] = secondParent.path[i];
			childIndex = (childIndex == n - 1) ? 0 : childIndex + 1;
		}
	}
	return Path(child);
}

Path mutate(Path path)
{
	int a = rand() % n;
	int b = rand() % n;
	Point temp = path.path[a];
	path.path[a] = path.path[b];
	path.path[b] = temp;
	return path;
}

void addChildren()
{
	int firstChildIndex = currentPopulation.size();
	for(int i = 1; i < firstChildIndex; ++i)
	{
		currentPopulation.push_back(crossover(currentPopulation[i - 1], currentPopulation[i]));
		currentPopulation.push_back(crossover(currentPopulation[i], currentPopulation[i - 1]));
	}
	currentPopulation.push_back(crossover(currentPopulation[0], currentPopulation[firstChildIndex - 1]));
	currentPopulation.push_back(crossover(currentPopulation[firstChildIndex - 1], currentPopulation[0]));
	int lastChild = currentPopulation.size();

	//mutate some children
	for(int i = 0; i < (lastChild - firstChildIndex) / 2; ++i)
	{
		int random = rand() % (lastChild - firstChildIndex) + firstChildIndex;
		currentPopulation.push_back(mutate(currentPopulation[random]));
	}
}

void selectParents()
{
	vector<Path> parentsPopulation = currentPopulation;
	currentPopulation.resize(populationSize / 5);
	for(int i = 0; i < populationSize / 20; ++i)
	{
		currentPopulation.push_back(parentsPopulation[rand() % (populationSize - (populationSize / 5)) + populationSize / 5]);
	}
}

void optimizePath()
{
	generateFirstPopulation();
	for(int i = 0; i < 1000; ++i)
	{
		sort(currentPopulation.begin(), currentPopulation.end());
		if((i == 10) || (i == 100) || (i == 300) || (i == 600))
		{
			cout << currentPopulation[0].fitnessValue << endl;
		}
		selectParents();
		addChildren();
	}
	sort(currentPopulation.begin(), currentPopulation.end());
	cout << currentPopulation[0].fitnessValue << endl;
}

int main()
{
	cout << "N= ";
	std::cin >> n;

	srand(unsigned(time(0)));

	generatePoints();
	optimizePath();

	return 0;
}