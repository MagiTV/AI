#include <iostream>
#include <vector>
#include <chrono>
using namespace std::chrono;

struct Node
{
public:
	std::string value;
	int emptyPosition;
	bool visited;
	Node(std::string value)
	{
		this->value = value;
		visited = false;
		bool found = false;
		for(int i = 0; i < value.size() && !found; ++i)
		{
			if(value[i] == '_')
			{
				emptyPosition = i;
				found = true;
			}
		}
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
		for(Node* current : elements)
		{
			if(current->visited)
			{
				std::cout << current->value << std::endl;
			}
		}
	}
};

std::string generateState(int frogsNumber, bool start)
{
	std::string state;
	char firstSymbol = start ? '>' : '<';
	char secondSymbol = start ? '<' : '>';
	for(int i = 0; i < frogsNumber; ++i)
	{
		state.push_back(firstSymbol);
	}
	state.push_back('_');
	for(int i = 0; i < frogsNumber; ++i)
	{
		state.push_back(secondSymbol);
	}
	return state;
}

std::vector<Node*> generateChilds(Node* parent)
{
	int parentEmpty = parent->emptyPosition;
	std::vector<Node*> result;

	for(int i : {-2, -1, 1, 2})
	{
		char symbol = (i < 0) ? '>' : '<';

		if(parentEmpty + i >= 0 && parentEmpty + i < parent->value.size() && parent->value[parentEmpty + i] == symbol)
		{
			std::string newValue = parent->value;
			newValue[parentEmpty] = symbol;
			newValue[parentEmpty + i] = '_';
			result.push_back(new Node(newValue));
		}
	}

	return result;
}

void playGame(int frogsNumber)
{
	std::string start = generateState(frogsNumber, true);
	std::string end = generateState(frogsNumber, false);

	Stack stack;
	Node* root = new Node(start);
	stack.push(root);
	bool findPath = false;

	while(!stack.isEmpty() && !findPath)
	{
		Node* current = stack.top();
		if(current->visited)
		{
			stack.pop();
			delete current;
		}
		else
		{
			current->visited = true;

			if(current->value.compare(end) == 0)
			{
				findPath = true;
			}
			else
			{
				for(Node* child : generateChilds(current))
				{
					stack.push(child);
				}
			}
		}
	}

	stack.printReversed();
}

int main()
{
	int frogsNumber;
	std::cout << "Enter the number of frogs:" << std::endl;
	std::cin >> frogsNumber;

	auto start = high_resolution_clock::now();

	playGame(frogsNumber);

	auto end = high_resolution_clock::now();
	auto time = duration_cast<milliseconds>(end - start);
	std::cout << std::endl << time.count() << " millisec" << std::endl;

	return 0;
}

