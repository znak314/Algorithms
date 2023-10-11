#include "pch.h"
#include "CppUnitTest.h"
#include "..\External Merge Sort\funcs.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(ConstructorTest)
		{
			char* fileName = "test.txt";
			long long int fileSize = 9223372036854775807; // Максимальне значення для long long int
			FileHandler fileHandler(fileName, fileSize);

			Assert::AreEqual(fileSize, fileHandler.GetSize());
		}

		TEST_METHOD(MaxMemoryLimitTest)
		{
			try
			{
				char* fileName = "test.txt";
				long long int RAM = (16 * 1024) * (1024 * 1024);
				long long int fileSize = (16 * 1024) * (1024 * 1024);
				Presorter presorter(fileName, fileSize);

				isValidMemory(fileSize, RAM);
			}
			catch (const exception& ex)
			{
				Assert::IsTrue(std::string(ex.what()).find("Runtime error: Too big RAM size") != std::string::npos);
			}
		}

		TEST_METHOD(BinaryFileSorting)
		{
			char filename[] = "A";
			std::ifstream file(filename, std::ios::binary);

			if (!file.is_open()) {
				Assert::Fail;
			}


			int prev, current;
			if (file.read(reinterpret_cast<char*>(&prev), sizeof(prev))) {
				while (file.read(reinterpret_cast<char*>(&current), sizeof(current))) {
					if (current < prev) {
						file.close();
						Assert::Fail;
					}
					prev = current;
				}
			}
			file.close();
		}
	};
}
