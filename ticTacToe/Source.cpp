#include <iostream>
#include <vector>
#include <algorithm>
using std::vector;
using std::cout;
using std::endl;

const char X = 'X';
const char O = 'O';
const char EMPTY = '_';
const int MIN_VALUE = -10;
const int MAX_VALUE = 10;

class Board
{
private:
	vector<vector<char>> board;
	bool xBoard;
	int value;
	bool terminal;
	Board* optimalSuccessor;
	int depth;

	void copy(const Board& other)
	{
		board = other.board;
		value = other.value;
		terminal = other.terminal;
		xBoard = other.xBoard;
		optimalSuccessor = other.optimalSuccessor;
		depth = other.depth;
	}
	bool checkTerminal()
	{
		int hasEmpty = false;
		for(int i = 0; i < 3; ++i)
		{
			if(board[i][0] != EMPTY)
			{
				if(board[i][0] == board[i][1] && board[i][1] == board[i][2])
				{
					value = (board[i][0] == X) ? (10 - depth) : -(10 - depth);
					return true;
				}
			}
			else
			{
				hasEmpty = true;
			}
			if(board[0][i] != EMPTY)
			{
				if(board[0][i] == board[1][i] && board[1][i] == board[2][i])
				{
					value = (board[0][i] == X) ? (10 - depth) : -(10 - depth);
					return true;
				}
			}
			else
			{
				hasEmpty = true;
			}
		}
		if(board[1][1] != EMPTY)
		{
			if(board[0][0] == board[1][1] && board[1][1] == board[2][2])
			{
				value = (board[1][1] == X) ? (10 - depth) : -(10 - depth);
				return true;
			}
			if(board[0][2] == board[1][1] && board[1][1] == board[2][0])
			{
				value = (board[1][1] == X) ? (10 - depth) : -(10 - depth);
				return true;
			}
		}
		else
		{
			return false;
		}
		if(hasEmpty)
		{
			return false;
		}
		value = 0;
		return (board[2][2] != EMPTY) && (board[1][2] != EMPTY) && (board[2][1] != EMPTY);
	}
public:
	Board(vector<vector<char>> board, int depth, bool xBoard = true)
	{
		this->board = board;
		this->depth = depth;
		this->xBoard = xBoard;
		optimalSuccessor = NULL;
		terminal = checkTerminal();
	}
	Board(const Board& other)
	{
		copy(other);
	}
	Board& operator=(const Board& other)
	{
		if(this != &other)
		{
			delete optimalSuccessor;
			copy(other);
		}
		return *this;
	}
	~Board()
	{
		delete optimalSuccessor;
	}

	void setUtility(int value)
	{
		this->value = value;
	}
	void setOptimalSuccessor(Board* successor)
	{
		delete optimalSuccessor;
		optimalSuccessor = successor;
	}
	bool setO(int x, int y)
	{
		if(board[x][y] == EMPTY)
		{
			board[x][y] = O;
			terminal = checkTerminal();
			xBoard = true;
			optimalSuccessor = NULL;
			++depth;
			return true;
		}
		else
		{
			return false;
		}
	}

	int getUtility() const
	{
		return value;
	}
	int getDepth() const
	{
		return depth;
	}
	bool isTerminal() const
	{
		return terminal;
	}
	vector<vector<char>> getSuccessorBoard() const
	{
		return optimalSuccessor->board;
	}
	vector<Board> getSuccessors() const
	{
		vector<Board> successors;
		for(int i = 0; i < 3; ++i)
		{
			for(int j = 0; j < 3; ++j)
			{
				if(board[i][j] == EMPTY)
				{
					vector<vector<char>> childBoard = board;
					childBoard[i][j] = xBoard ? X : O;
					successors.push_back(Board(childBoard, depth + 1, !xBoard));
				}
			}
		}
		return successors;
	}
	void print() const
	{
		for(int i = 0; i < 3; ++i)
		{
			for(int j = 0; j < 3; ++j)
			{
				cout << board[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
	}
};

int minValue(Board&, int a, int b);

int maxValue(Board& state, int a, int b)
{
	if(state.isTerminal())
	{
		return state.getUtility();
	}
	state.setUtility(MIN_VALUE);
	vector<Board> successors = state.getSuccessors();
	int newValue;
	for(Board current : successors)
	{
		newValue = minValue(current, a, b);
		if(state.getUtility() < newValue)
		{
			state.setUtility(newValue);
			state.setOptimalSuccessor(new Board(current));
		}
		if(state.getUtility() >= b)
		{
			return state.getUtility();
		}
		a = std::max(a, state.getUtility());
	}
	return state.getUtility();
}

int minValue(Board& state, int a, int b)
{
	if(state.isTerminal())
	{
		return state.getUtility();
	}
	state.setUtility(MAX_VALUE);
	vector<Board> successors = state.getSuccessors();
	for(Board current : successors)
	{
		state.setUtility(std::min(state.getUtility(), maxValue(current, a, b)));
		if(state.getUtility() <= a)
		{
			return state.getUtility();
		}
		b = std::min(b, state.getUtility());
	}
	return state.getUtility();
}

void play()
{
	cout << "Do you want to play first? y/n" << endl;
	char answer;
	std::cin >> answer;

	vector<vector<char>> initBoard = { {EMPTY,EMPTY,EMPTY},{EMPTY,EMPTY,EMPTY},{EMPTY,EMPTY,EMPTY} };
	int x, y;
	int depth = 0;

	if(answer == 'y')
	{
		for(int i = 0; i < 3; ++i)
		{
			for(int j = 0; j < 3; ++j)
			{
				cout << initBoard[i][j] << " ";
			}
			cout << endl;
		}

		cout << endl << "Choose position: ";
		std::cin >> x >> y;
		initBoard[--x][--y] = O;
		depth = 1;
	}

	Board currentBoard(initBoard, depth);

	if(answer == 'y')
	{
		currentBoard.print();
	}

	while(!currentBoard.isTerminal())
	{
		maxValue(currentBoard, MIN_VALUE, MAX_VALUE);
		currentBoard = Board(currentBoard.getSuccessorBoard(), currentBoard.getDepth() + 1);
		currentBoard.print();

		if(!currentBoard.isTerminal())
		{
			do
			{
				cout << endl << "Choose position: ";
				std::cin >> x >> y;
			} while(!currentBoard.setO(--x, --y));
			currentBoard.print();
		}
	}

	if(currentBoard.getUtility() > 0)
	{
		cout << "YOU LOSE!" << endl;
	}
	else if(currentBoard.getUtility() == 0)
	{
		cout << "IT'S A DRAW." << endl;
	}
	else
	{
		cout << "YOU WIN!" << endl;
	}
}

int main()
{
	play();

	return 0;
}