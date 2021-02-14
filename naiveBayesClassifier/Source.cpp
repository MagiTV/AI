#include <iostream>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
using std::vector;

const std::string DATA_FILE_NAME = "data.txt";
const int ATTRIBUTES_COUNT = 17;
const int ITERATIONS_COUNT = 10;
const int GROUP_SIZE = 43;
int indexTestGroup = 0;

enum Vote {
	yes = 0,
	no = 1,
	abstained = 2
};

vector<vector<Vote>> data;
vector<vector<double>> democratInfo;
vector<vector<double>> republicanInfo;
double democratCount;
double republicanCount;

void saveLine(std::string line)
{
	vector<Vote> current;
	int firstAttributePosition = 0;
	if(line[0] == 'd')
	{
		current.push_back(Vote::yes);
		firstAttributePosition = 9;
	}
	else if(line[0] == 'r')
	{
		current.push_back(Vote::no);
		firstAttributePosition = 11;
	}

	for(int i = firstAttributePosition; i < line.size(); i += 2)
	{
		switch(line[i])
		{
		case 'y':
		{
			current.push_back(Vote::yes);
			break;
		}
		case 'n':
		{
			current.push_back(Vote::no);
			break;
		}
		case '?':
		{
			current.push_back(Vote::abstained);
			break;
		}
		}
	}
	data.push_back(current);
}

void readData()
{
	std::ifstream dataFile(DATA_FILE_NAME);
	std::string currentLine;

	while(getline(dataFile, currentLine))
	{
		saveLine(currentLine);
	}

	dataFile.close();
}

void makeModel()
{
	democratInfo.clear();
	democratInfo.resize(ATTRIBUTES_COUNT - 1, { 1,1,1 });
	republicanInfo.clear();
	republicanInfo.resize(ATTRIBUTES_COUNT - 1, { 1,1,1 });
	democratCount = 3;
	republicanCount = 3;

	for(int i = 0; i < data.size(); ++i)
	{
		if(i == indexTestGroup)
		{
			i += GROUP_SIZE;
		}

		(data[i][0] == Vote::yes) ? ++democratCount : ++republicanCount;

		for(int j = 1; j < ATTRIBUTES_COUNT; ++j)
		{
			(data[i][0] == Vote::yes) ? ++democratInfo[j - 1][data[i][j]] : ++republicanInfo[j - 1][data[i][j]];
		}
	}

	for(int i = 0; i < ATTRIBUTES_COUNT - 1; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			democratInfo[i][j] = std::log(democratInfo[i][j] / democratCount);
		}
	}

	for(int i = 0; i < ATTRIBUTES_COUNT - 1; ++i)
	{
		for(int j = 0; j < 3; ++j)
		{
			republicanInfo[i][j] = std::log(republicanInfo[i][j] / republicanCount);
		}
	}

	double allCount = democratCount + republicanCount;
	democratCount = std::log(democratCount / allCount);
	republicanCount = std::log(republicanCount / allCount);
}

double testData()
{
	double democratProbability;
	double republicanProbability;
	double correctAnswers = 0;

	for(int i = indexTestGroup; i < (indexTestGroup + GROUP_SIZE); ++i)
	{
		democratProbability = democratCount;
		republicanProbability = republicanCount;
		for(int j = 1; j < ATTRIBUTES_COUNT; ++j)
		{
			democratProbability += democratInfo[j - 1][data[i][j]];
			republicanProbability += republicanInfo[j - 1][data[i][j]];
		}

		if(democratProbability > republicanProbability && data[i][0] == Vote::yes)
		{
			++correctAnswers;
		}
		else if(democratProbability < republicanProbability && data[i][0] == Vote::no)
		{
			++correctAnswers;
		}
	}

	return 100.0 * (correctAnswers / (double) GROUP_SIZE);
}

void test()
{
	double averageResult = 0;
	double currentResult = 0;

	for(int i = 0; i < ITERATIONS_COUNT; ++i)
	{
		makeModel();
		currentResult = testData();
		averageResult += currentResult;
		std::cout << (i + 1) << ": " << currentResult << std::endl;
		indexTestGroup += GROUP_SIZE;
	}

	std::cout << std::endl;
	std::cout << "average: " << averageResult / (double) ITERATIONS_COUNT << std::endl;
}

int main()
{
	srand(unsigned(time(0)));

	readData();
	std::random_shuffle(data.begin(), data.end());

	test();

	return 0;
}