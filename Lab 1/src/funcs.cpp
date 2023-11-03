#include"funcs.h"


bool isNonNegativeInteger(const std::string& input) {

    try {
        int number = stoi(input);
        if (number > 0 && (number == stof(input))) {
            return true;
        }
        else {
            return false;
        }
    }
    catch (const std::invalid_argument& e) {
        return false;
    }
    catch (const std::out_of_range& e) {
        return false;
    }
}

long  long  int inputSize()
{
    int inv;
    string userInput;

    do {
        inv = 0;
        cin >> userInput;

        if (!isNonNegativeInteger(userInput))
        {
            inv = 1;
            cout << "\n”вед≥ть коректне значенн€! ÷е повинне бути ц≥ле нев≥д'Їмне число б≥льше 0\n";
        }
    } while (inv);
    return stoll(userInput) * 1024 * 1024;
}

bool Modificated()
{
    int inv;
    string input;
    do {
        inv = 1;
        "\nќбер≥ть алгоритм (1) або (2): ";
        cin >> input;

        if (input == "1")
            return true;
        if (input == "2")
            return false;

    } while (inv);
}

FileHandler::FileHandler(char* Name, long long int size)
{
    numberOfElements = size / 4;
    FileName = Name;
    FileSize = size;
}

void FileHandler::CreateAndFillFile()
{
    const int bufferSize = 1024;
    const long long int targetSizeGB = FileSize;
    int buffer[bufferSize];

    FILE* file = fopen(FileName, "wb");

    long long int count = 0;
    while (count < targetSizeGB) {

        for (int i = 0; i < bufferSize; i++) {
            buffer[i] = rand() % (MAX_GENERATED_NUMBER - MIN_GENERATED_NUMBER + 1) + MIN_GENERATED_NUMBER;
        }
        fwrite(buffer, bufferSize * sizeof(int), 1, file);
        count += bufferSize * sizeof(int);
    }

    fclose(file);
}

int FileHandler::GetFragmentsCount(long long int RAM)
{
    if (RAM > static_cast<long long>((5 * 1024)) * (1024 * 1024))
        throw RAMSizeTooLargeException();
    if (FileSize < RAM)
        return 1;

    int k_0 = ceil(log2(FileSize / RAM));

    return pow(2, k_0);
}

char* FileHandler::GetFileName()
{
    return FileName;
}

long long int FileHandler::GetElementsNumber()
{
    return numberOfElements;
}

void FileHandler::printBinaryFile()
{
    FILE* file = fopen(FileName, "rb");

    // „итаЇмо ≥ виводимо вм≥ст файлу
    int byte;
    while (fread(&byte, sizeof(int), 1, file)) {
        std::cout << static_cast<int>(byte) << " ";
    }

    fclose(file);
}

void Presorter::Presort(int RAM)
{
    const char* temp_file_name = "temporary";
    const char* name = GetFileName();

    ifstream File(name, std::ios::binary);
    ofstream NewFile(temp_file_name, std::ios::binary);

    int data;
    vector<int> intVector;


    int counter = GetElementsNumber();
    int numberOfElements_in_fragment = GetElementsNumber() / GetFragmentsCount(RAM);

    while (counter)
    {
        auto read_start = chrono::high_resolution_clock::now();
        intVector.resize(numberOfElements_in_fragment);
        File.read(reinterpret_cast<char*>(intVector.data()), numberOfElements_in_fragment * sizeof(int));
        auto read_end = chrono::high_resolution_clock::now();
        auto read_duration = chrono::duration_cast<std::chrono::microseconds>(read_end - read_start);
        cout << "\Reading  time: " << read_duration.count() / 1000000. << " seconds" << endl;

        if (counter < numberOfElements_in_fragment)
        {
            intVector.resize(counter);
            File.read(reinterpret_cast<char*>(intVector.data()), counter * sizeof(int));
            counter = 0;
        }

        counter -= numberOfElements_in_fragment;

        auto sort_start = chrono::high_resolution_clock::now();
        sort(intVector.begin(), intVector.end());
        auto sort_end = chrono::high_resolution_clock::now();
        auto sort_duration = chrono::duration_cast<std::chrono::microseconds>(sort_end - sort_start);
        cout << "\Fragment Sort time: " << sort_duration.count() / 1000000. << " seconds" << endl;

        auto write_start = chrono::high_resolution_clock::now();
        NewFile.write(reinterpret_cast<const char*>(intVector.data()), intVector.size() * sizeof(int));

        auto write_end = chrono::high_resolution_clock::now();
        auto write_duration = chrono::duration_cast<std::chrono::microseconds>(write_end - write_start);
        cout << "\Writing  time: " << write_duration.count() / 1000000. << " seconds" << endl;
        cout << "------------------------------\n\n";

        intVector.clear();
    }

    File.close();
    NewFile.close();

    remove(name);
    rename(temp_file_name, name);
}

void FileContentDistributor::ContentDistribution(int elements_in_fragment, int numberOfOteration, char* file1, char* file2, int RAM)
{
    FILE* mainFile = fopen(this->GetFileName(), "rb");
    FILE* B = fopen(file1, "wb");
    FILE* C = fopen(file2, "wb");

    int value, isOdd = 1;
    int N = elements_in_fragment * pow(2, numberOfOteration);

    int bufferSize = RAM;
    int* buffer = new int[bufferSize];


    while (fread(&value, sizeof(int), 1, mainFile)) //перев≥р€Їмо чи файл ще не порожн≥й
    {

        while (!feof(mainFile))
        {
            int bytesRead = fread(buffer, sizeof(int), bufferSize, mainFile);

            if (isOdd)
            {
                fwrite(buffer, sizeof(int), bytesRead, B);
            }
            else
            {
                fwrite(buffer, sizeof(int), bytesRead, C);
            }

            isOdd = !isOdd;
        }
    }

    fclose(mainFile);
    fclose(B);
    fclose(C);
}

void FileMerger::MergeFiles(char* file1, char* file2, int numberOfOteration, int elements_in_fragment)
{
    FILE* B = fopen(file1, "rb");
    FILE* C = fopen(file2, "rb");

    FILE* mergeResult = fopen(this->GetFileName(), "wb");

    int value1, value2;
    int read1, read2;
    int num1, num2;

    int N = elements_in_fragment * pow(2, numberOfOteration);

    read1 = fread(&value1, sizeof(int), 1, B);
    read2 = fread(&value2, sizeof(int), 1, C);

    while (true) {

        num1 = num2 = 0;

        // якщо файли зак≥нчилис€, завершуЇмо процес
        if (read1 == 0 && read2 == 0) {
            break;
        }

        while (num1 < N || num2 < N)
        {
            if (((value1 < value2) || num2 == N) && num1 != N)
            {
                fwrite(&value1, sizeof(int), 1, mergeResult);
                num1++;
                if ((read1 = fread(&value1, sizeof(int), 1, B)) == 0)
                {
                    num1 = N;
                    if (read2)
                    {
                        do {
                            fwrite(&value2, sizeof(int), 1, mergeResult);
                        } while (read2 = fread(&value2, sizeof(int), 1, C));
                        num2 = N;
                    }
                }
            }
            else if (((value1 > value2) || num1 == N) && num2 != N)
            {
                fwrite(&value2, sizeof(int), 1, mergeResult);
                num2++;
                if ((read2 = fread(&value2, sizeof(int), 1, C)) == 0)
                {
                    num2 = N;
                    if (read1)
                    {
                        do {
                            fwrite(&value1, sizeof(int), 1, mergeResult);
                        } while (read1 = fread(&value1, sizeof(int), 1, B));
                        num1 = N;
                    }
                }
            }
            else {
                fwrite(&value1, sizeof(int), 1, mergeResult);
                fwrite(&value2, sizeof(int), 1, mergeResult);
                num1++;
                num2++;
                if ((read1 = fread(&value1, sizeof(int), 1, B)) == 0)
                    num1 = N;
                if ((read2 = fread(&value2, sizeof(int), 1, C)) == 0)
                    num2 = N;
            }
        }
    }


    fclose(B);
    fclose(C);
    fclose(mergeResult);

    remove(file1);
    remove(file2);
}

void isValidMemory(long long int Memory, long long int RAM)
{
    if (Memory > MAX_RAM && RAM > MAX_RAM)
        throw runtime_error("Runtime error: Too big RAM size");
}