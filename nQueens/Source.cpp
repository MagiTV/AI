#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>
using std::cout;
using std::endl;
using std::vector;
using namespace std::chrono;

int n;
const int k = 3;
vector<int> queens;
vector<int> rows;
vector<int> diag1;
vector<int> diag2;

void addConflicts(int row, int col)
{
	++rows[row];
	++diag1[col - row + n - 1];
	++diag2[col + row];
}

void removeConflicts(int row, int col)
{
	--rows[row];
	--diag1[col - row + n - 1];
	--diag2[col + row];
}

int getConflictsEmptyPosition(int row, int col)
{
	return rows[row] + diag1[col - row + n - 1] + diag2[col + row];
}

void initBoard()
{
	queens.clear();
	queens.resize(n, -1);
	rows.clear();
	rows.resize(n, 0);
	diag1.clear();
	diag1.resize(2 * n - 1, 0);
	diag2.clear();
	diag2.resize(2 * n - 1, 0);

	vector<int> emptyRows;
	for (int i = 0; i < n; ++i)
	{
		emptyRows.push_back(i);
	}
	for (int col = 0; col < n; ++col)
	{
		int random = rand() % emptyRows.size();
		queens[col] = emptyRows[random];
		emptyRows.erase(emptyRows.begin() + random);
		addConflicts(queens[col], col);
	}
}

int getColWithMaxConflicts()
{
	vector<int> colsWithMaxConflicts;
	int maxConflicts = -1;
	int currentColConflicts;

	for (int col = 0; col < n; ++col)
	{
		currentColConflicts = getConflictsEmptyPosition(queens[col], col) - 3;
		if (currentColConflicts > maxConflicts)
		{
			maxConflicts = currentColConflicts;
			colsWithMaxConflicts = { col };
		}
		else if (currentColConflicts == maxConflicts)
		{
			colsWithMaxConflicts.push_back(col);
		}
	}

	if (maxConflicts == 0)
	{
		return -1;
	}

	return colsWithMaxConflicts[rand() % colsWithMaxConflicts.size()];
}

int getRowWithMinConflicts(int col)
{
	vector<int> rowsWithMinConflicts;
	int minConflicts = n;
	int currentRowConflicts;

	for (int row = 0; row < n; ++row)
	{
		currentRowConflicts = getConflictsEmptyPosition(row, col);
		if (currentRowConflicts < minConflicts)
		{
			minConflicts = currentRowConflicts;
			rowsWithMinConflicts = { row };
		}
		else if (currentRowConflicts == minConflicts)
		{
			rowsWithMinConflicts.push_back(row);
		}
	}

	return rowsWithMinConflicts[rand() % rowsWithMinConflicts.size()];
}

void solve()
{
	initBoard();

	int i = 0;
	int col;
	int row;

	while (i++ <= k * n)
	{
		col = getColWithMaxConflicts();
		if (col == -1)
		{
			return;
		}
		row = getRowWithMinConflicts(col);
		removeConflicts(queens[col], col);
		queens[col] = row;
		addConflicts(queens[col], col);
	}

	solve();
}

void print()
{
	int n = queens.size();
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			if (queens[j] == i)
			{
				cout << "X ";
			}
			else
			{
				cout << (char)254 << " ";
			}
		}
		cout << endl;
	}
}

int main()
{
	cout << "n= ";
	std::cin >> n;

	srand(time(0));

	auto start = high_resolution_clock::now();

	solve();

	auto end = high_resolution_clock::now();
	auto time = duration_cast<milliseconds>(end - start);
	cout << endl << time.count() << " millisec" << endl;

	if (n < 20)
	{
		print();
	}
}