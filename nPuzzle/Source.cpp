#include <iostream>
#include <cmath>
#include <vector>
using std::vector;
using std::cout;
using std::endl;

const std::string UP = "up";
const std::string DOWN = "down";
const std::string LEFT = "left";
const std::string RIGHT = "right";
int squareSide;

struct Index {
public:
	int x;
	int y;
	Index()
	{
		this->x = -1;
		this->y = -1;
	}
	Index(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
};

vector<vector<int>> startState;
Index* goalStateIndexes;

class Node {
private:
	vector<vector<int>> state;
	int costFromStart;
	int estimatedCostToGoal;
	Index emptyPosition;
	std::string parentDirection;
	bool visited;

	void findEmptyPosition()
	{
		bool found = false;
		for (int i = 0; i < squareSide && !found; ++i)
		{
			for (int j = 0; j < squareSide && !found; ++j)
			{
				if (state[i][j] == 0)
				{
					emptyPosition.x = i;
					emptyPosition.y = j;
					found = true;
				}
			}
		}
	}
	void estimateCostToGoal()
	{
		estimatedCostToGoal = 0;
		for (int i = 0; i < squareSide; ++i)
		{
			for (int j = 0; j < squareSide; ++j)
			{
				if (state[i][j] != 0)
				{
					estimatedCostToGoal += abs(goalStateIndexes[state[i][j]].x - i) + abs(goalStateIndexes[state[i][j]].y - j);
				}
			}
		}
	}
public:
	Node(vector<vector<int>> state, int costFromStart, std::string parentDirection = "") :
		state(state), costFromStart(costFromStart), parentDirection(parentDirection), visited(false)
	{
		findEmptyPosition();
		estimateCostToGoal();
	}
	vector<vector<int>> getState() const
	{
		return state;
	}
	int getCostFromStart() const
	{
		return costFromStart;
	}
	int getEstimatedTotalCost() const
	{
		return costFromStart + estimatedCostToGoal;
	}
	bool isGoalState() const
	{
		return estimatedCostToGoal == 0;
	}
	Index getEmptyPosition() const
	{
		return emptyPosition;
	}
	std::string getParentDirection() const
	{
		return parentDirection;
	}
	bool isVisited() const
	{
		return visited;
	}
	void visit()
	{
		visited = true;
	}
};

class Stack
{
private:
	std::vector<Node*> elements;
public:
	void pop()
	{
		elements.pop_back();
	}
	void push(Node* newElement)
	{
		elements.push_back(newElement);
	}
	Node* top()
	{
		return elements[elements.size() - 1];
	}
	bool isEmpty()
	{
		return elements.empty();
	}
	void printReversed()
	{
		for (Node* current : elements)
		{
			if (current->isVisited())
			{
				cout << current->getParentDirection() << endl;
			}
		}
	}
};

vector<vector<int>> enterStartState()
{
	cout << "Puzzle: " << std::endl;
	vector<vector<int>> startState;
	int currElem;
	for (int i = 0; i < squareSide; ++i)
	{
		vector<int> currentRow;
		for (int j = 0; j < squareSide; ++j)
		{
			std::cin >> currElem;
			currentRow.push_back(currElem);
		}
		startState.push_back(currentRow);
	}
	return startState;
}

Index* generateGoalState(int emptyPosition)
{
	Index* goalState = new Index[squareSide * squareSide];
	int currentValue = 1;
	int currentPosition = 0;
	for (int i = 0; i < squareSide; ++i)
	{
		for (int j = 0; j < squareSide; ++j)
		{
			goalState[(currentPosition++ == emptyPosition) ? 0 : currentValue++] = Index(i, j);
		}
	}
	return goalState;
}

vector<Node*> sortChilds(vector<Node*> childs)
{
	Node* max;
	Node* temp;
	for (int i = 0; i < childs.size() - 1; ++i)
	{
		max = childs[i];
		for (int j = i + 1; j < childs.size(); ++j)
		{
			if (childs[j]->getEstimatedTotalCost() > max->getEstimatedTotalCost())
			{
				temp = max;
				max = childs[j];
				childs[j] = temp;
			}
		}
		childs[i] = max;
	}
	return childs;
}

vector<Node*> generateChilds(Node* parent)
{
	vector<Node*> childs;
	vector<vector<int>> newState;
	Index parentEmptyPosition = parent->getEmptyPosition();
	int x = parentEmptyPosition.x - 1;
	int y = parentEmptyPosition.y;
	int newCostFromStart = parent->getCostFromStart() + 1;
	std::string grandparentDirection = parent->getParentDirection();
	if (x >= 0 && grandparentDirection.compare(DOWN))
	{
		newState = parent->getState();
		newState[parentEmptyPosition.x][parentEmptyPosition.y] = newState[x][y];
		newState[x][y] = 0;
		childs.push_back(new Node(newState, newCostFromStart, UP));
	}
	x += 2;
	if (x < squareSide && grandparentDirection.compare(UP))
	{
		newState = parent->getState();
		newState[parentEmptyPosition.x][parentEmptyPosition.y] = newState[x][y];
		newState[x][y] = 0;
		childs.push_back(new Node(newState, newCostFromStart, DOWN));
	}
	x -= 1;
	y -= 1;
	if (y >= 0 && grandparentDirection.compare(RIGHT))
	{
		newState = parent->getState();
		newState[parentEmptyPosition.x][parentEmptyPosition.y] = newState[x][y];
		newState[x][y] = 0;
		childs.push_back(new Node(newState, newCostFromStart, LEFT));
	}
	y += 2;
	if (y < squareSide && grandparentDirection.compare(LEFT))
	{
		newState = parent->getState();
		newState[parentEmptyPosition.x][parentEmptyPosition.y] = newState[x][y];
		newState[x][y] = 0;
		childs.push_back(new Node(newState, newCostFromStart, RIGHT));
	}
	return sortChilds(childs);
}

bool findPath(int maxMoves)
{
	Node* root = new Node(startState, 0);
	Stack stack;
	stack.push(root);
	bool foundPath = false;

	while (!stack.isEmpty() && !foundPath)
	{
		Node* current = stack.top();
		if (current->getEstimatedTotalCost() <= maxMoves && !current->isVisited())
		{
			current->visit();
			if (current->isGoalState())
			{
				foundPath = true;
			}
			else
			{
				for (Node* child : generateChilds(current))
				{
					stack.push(child);
				}
			}
		}
		else
		{
			stack.pop();
			delete current;
		}
	}

	if (foundPath)
	{
		cout << maxMoves << std::endl;
		stack.printReversed();
	}

	return foundPath;
}

int countInversions()
{
	vector<int> numbers;
	for (int i = 0; i < squareSide; ++i)
	{
		for (int j = 0; j < squareSide; ++j)
		{
			if (startState[i][j] != 0)
			{
				numbers.push_back(startState[i][j]);
			}
		}
	}
	int inversionsCount = 0;
	for (int i = 0; i < numbers.size() - 1; ++i)
	{
		for (int j = i + 1; j < numbers.size(); ++j)
		{
			if (numbers[i] > numbers[j])
			{
				++inversionsCount;
			}
		}
	}
	return inversionsCount;
}

bool isSolvable(int emptyPositionRow)
{
	bool evenInversionsCount = countInversions() % 2 == 0;
	if (squareSide % 2 != 0)
	{
		return evenInversionsCount;
	}
	if (emptyPositionRow % 2 == 0)
	{
		return !evenInversionsCount;
	}
	return evenInversionsCount;
}

void solvePuzzel()
{
	Node* root = new Node(startState, 0);
	if (root->isGoalState())
	{
		cout << "0" << endl;
		return;
	}

	if (goalStateIndexes[0].x == squareSide - 1 && goalStateIndexes[0].y == squareSide - 1 && !isSolvable(root->getEmptyPosition().x))
	{
		cout << "The puzzel is not solvable." << endl;
		return;
	}

	int i = root->getEstimatedTotalCost();
	bool foundPath = false;
	while (!foundPath)
	{
		foundPath = findPath(i++);
	}
}

void getInputData()
{
	int n;
	int emptyPosition;
	cout << "N = ";
	std::cin >> n;
	cout << "I = ";
	std::cin >> emptyPosition;
	squareSide = sqrt(n + 1);
	startState = enterStartState();
	goalStateIndexes = generateGoalState((emptyPosition == -1) ? n : emptyPosition);
	cout << endl;
}

int main()
{
	getInputData();
	solvePuzzel();
}