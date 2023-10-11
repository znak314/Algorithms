#include<Windows.h>
#include<iostream>
#include <string>
#include<fstream>
#include<vector>
#include<algorithm>
#include<chrono>
#include <stdexcept>


#pragma warning(disable : 4996) // Allows to work in C style with fopen() function

using namespace std;


int const MAX_GENERATED_NUMBER = 1000000;
int const MIN_GENERATED_NUMBER = -1000000;
long long const int MAX_RAM = (5 * 1024) * (1024 * 1024);

bool isNonNegativeInteger(const std::string& input);
long long  int inputSize();
bool Modificated();
void isValidMemory(long long int Memory, long long int RAM);

class FileHandler
{
	char* FileName;
	long long int FileSize;
	long long int numberOfElements;
public:
	FileHandler(char* Name, long long int size);
	FileHandler() {};
	void CreateAndFillFile();
	int GetFragmentsCount(long long int RAM);
	char* GetFileName();
	long long int GetElementsNumber();
	void printBinaryFile();
	long long int GetSize() { return FileSize; }
};


class Presorter : public FileHandler
{
public:
	Presorter(char* Name, int size) : FileHandler(Name, size) {};
	void Presort(int RAM);
};


class FileContentDistributor :virtual public FileHandler
{
public:
	void ContentDistribution(int elements_in_fragment, int numberOfOteration, char* file1, char* file2, int RAM);
};

class FileMerger :virtual public FileHandler
{
public:
	void MergeFiles(char* file1, char* file2, int numberOfOteration, int elements_in_fragment);
};

class ExternalSorter : public FileContentDistributor, public FileMerger
{
public:
	ExternalSorter(char* Name, int size) : FileHandler(Name, size), FileContentDistributor(), FileMerger() {};
};

class RAMSizeTooLargeException : public std::runtime_error {
public:
	RAMSizeTooLargeException() : std::runtime_error("File size is too large") {}
};
