#include"funcs.h"



int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	srand(time(NULL));

	try {
		long long int FileSize, RAM_restriction;
		int iteration, elements_in_fragment = 1;

		char FileName[] = "A";

		cout << "------------------------------\n";
		cout << "Файл якого розміру створити? (у мегабайтах) ---> ";

		FileSize = inputSize();


		cout << "------------------------------\n";
		cout << "Встановіть обмеження використання оперативної пам'яті (у мегабайтах) ---> ";

		RAM_restriction = inputSize();

		isValidMemory(FileSize, RAM_restriction);

		FileHandler NewFile(FileName, FileSize);
		NewFile.CreateAndFillFile();
		iteration = log2(NewFile.GetElementsNumber());

		cout << "------------------------------\n";
		cout << "(1) ---> Модифікований алгоритм\n(2) ---> Звичайний\n";

		auto start = chrono::high_resolution_clock::now();
		if (Modificated())
		{
			auto Pre_sort_start = chrono::high_resolution_clock::now();
			Presorter File(FileName, FileSize);
			cout << "Файл буде поділено на " << File.GetFragmentsCount(RAM_restriction) << " частинок\n";
			File.Presort(RAM_restriction);
			elements_in_fragment = File.GetElementsNumber() / File.GetFragmentsCount(RAM_restriction);
			iteration = log2(File.GetFragmentsCount(RAM_restriction));

			auto Pre_sort_end = chrono::high_resolution_clock::now();
			auto Pre_sort_duration = chrono::duration_cast<std::chrono::microseconds>(Pre_sort_end - Pre_sort_start);

			cout << "------------------------------\n";
			cout << "\Pre-Sort time: " << Pre_sort_duration.count() / 1000000. << " seconds" << endl;
			cout << "------------------------------\n";
		}

		ExternalSorter file(FileName, FileSize);
		auto external_start = chrono::high_resolution_clock::now();

		for (int i = 0; i < iteration; i++)
		{
			// Тимчасові файли
			char temp1[] = "B";
			char temp2[] = "C";

			auto distribution_start = chrono::high_resolution_clock::now();
			file.ContentDistribution(elements_in_fragment, i, temp1, temp2, RAM_restriction);
			auto distribution_end = chrono::high_resolution_clock::now();
			auto distribution_duration = chrono::duration_cast<std::chrono::microseconds>(distribution_end - distribution_start);
			cout << i + 1 << " Distribution time: " << distribution_duration.count() / 1000000. << " seconds" << endl;


			auto merge_start = chrono::high_resolution_clock::now();
			file.MergeFiles(temp1, temp2, i, elements_in_fragment);
			auto merge_end = chrono::high_resolution_clock::now();
			auto merge_duration = chrono::duration_cast<std::chrono::microseconds>(merge_end - merge_start);
			cout << i + 1 << " Merge time: " << merge_duration.count() / 1000000. << " seconds" << endl;
		}

		auto external_end = chrono::high_resolution_clock::now();
		auto external_duration = chrono::duration_cast<std::chrono::microseconds>(external_end - external_start);
		cout << "------------------------------\n";

		auto end = chrono::high_resolution_clock::now();
		auto duration = chrono::duration_cast<std::chrono::microseconds>(end - start);
		cout << "\Sorting time: " << duration.count() / 1000000. << " seconds" << endl;


	}
	catch (const std::exception& e) {
		// Обробка виключення типу std::exception
		std::cerr << "Помилка: " << e.what() << std::endl;
	}
	//NewFile.printBinaryFile();
	return 0;
}