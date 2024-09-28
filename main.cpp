#include <vector>
#include <iostream>
#include <sstream>
#include <ctime>
#include <random>
#include <thread>
#include <omp.h>
#include <fstream>

#define CLEAR() (std::system("cls"))

struct ArraySorter {
public:
	std::vector<int> runSort(std::vector<int> arr, int type, int* count) {
		int lenArray = findLen(arr);
		switch (type)
		{
			case (1):
			{
				bool swapped = false;
				for (int i = 0; i < lenArray; ++i) {
					swapped = false;
					for (int j = 0; j < lenArray - i - 1; ++j) {
						if (arr[j] > arr[j + 1]) {
							std::swap(arr[j], arr[j + 1]);
							*count += 1;
							swapped = true;
						}
					}
					if (!swapped) {
						break;
					}
				}
				return arr;
			}
			break;


		case (2):
			for (int i = 1; i < lenArray; ++i) {
				auto key = arr[i];
				int j = i - 1;
				while (j >= 0 && key < arr[j])
				{
					std::swap(arr[j + 1], arr[j]);
					j -= 1;
					*count += 1;

				}
				std::swap(arr[j + 1], key);
				*count += 1;
			}
			return arr;
			break;

		case (3):
			return quickSort(arr, lenArray, count);

		case (4):
			for (int i = 0; i < lenArray; ++i) {
				int index = i;
				for (int j = i + 1; j < lenArray; ++j) {
					if (arr[j] < arr[index]) {
						index = j;

					}
				}
				std::swap(arr[i], arr[index]);
				*count += 1;
			}
			return arr; break;
		case (5):
			return mergeSort(arr, count);
		case (6):
				int i, j, k;
				for (i = 0; i < lenArray;) {
					for (j = i + 1; j < lenArray; j++) {
						if (arr[j] < arr[j - 1])
						{
							std::swap(arr[j], arr[j - 1]);
							*count += 1;
						}
					}
					lenArray--;
					for (k = lenArray - 1; k > i; k--) {
						if (arr[k] < arr[k - 1])
						{
							std::swap(arr[k], arr[k - 1]);
							*count += 1;
						}
					}
					i++;
				}
				return arr;


		default:
			break;
		}
	}
public:
	static std::string GenerateOutput(std::vector<int>& arr) {
		std::stringstream output;
		for (const auto& elem : arr) {
			output << "|" << elem << " ";
		}
		return output.str();
	}
private:
	std::vector<int> mergeSort(std::vector<int> arr, int* count) {
		if (arr.size() <= 1) {
			return arr;
		}

		int mid = arr.size() / 2;
		std::vector<int> lefthalf(arr.begin(), arr.begin() + mid);
		std::vector<int> righthalf(arr.begin() + mid, arr.end());

		lefthalf = mergeSort(lefthalf, count);
		righthalf = mergeSort(righthalf, count);

		std::vector<int> result;
		int i = 0, j = 0;
		while (i < lefthalf.size() && j < righthalf.size()) {
			if (lefthalf[i] < righthalf[j]) {
				result.push_back(lefthalf[i++]);
				*count += 1;
			}
			else {
				result.push_back(righthalf[j++]);
				*count += 1;
			}
		}

		while (i < lefthalf.size()) {
			result.push_back(lefthalf[i++]);
			*count += 1;
		}

		while (j < righthalf.size()) {
			result.push_back(righthalf[j++]);
			*count += 1;
		}

		return result;
	}



	std::vector<int> quickSort(std::vector<int> arr, int lenArray, int* count) {
		if (lenArray <= 1) {
			return arr;
		}
		std::vector<int> left{};
		std::vector<int> middle{};
		std::vector<int> right{};
		int pivot = arr[lenArray / 2];
		for (const auto& elem : arr) {
			if (elem < pivot) {
				left.push_back(elem);
				*count += 1;
			}
			else if (elem == pivot) {
				middle.push_back(elem);
				*count += 1;
			}
			else {
				right.push_back(elem);
				*count += 1;
			}
		}
		std::vector<int> result;
		result = quickSort(left, left.size(), count);
		result.insert(result.end(), middle.begin(), middle.end());
		std::vector<int> right_sorted = quickSort(right, right.size(), count);
		result.insert(result.end(), right_sorted.begin(), right_sorted.end());
		return result;
	}
private:
	static int findLen(const std::vector<int>& arr) {
		int len{};
		for (const auto& elem : arr) {
			++len;
		}
		return len;
	}

};
std::vector<int> initiateArray(int len) {
	if (len > 9999999) {

		std::cout << "Array length cannot be > 10 000 000 element! ";
		abort();

	}
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 10000000);

	std::vector<int> arr(len);
	#pragma omp parallel for
	for (int i = 0; i < len; ++i) {
		arr[i] = dis(gen);
	}
	return arr;
}



int main() {

	setlocale(LC_ALL, "rus");
	std::vector<int> arr{};
	std::string input{};
	int type;
	std::cout << "You need autogenerate list(1) or manual enter(2) ?: ";
	std::cin >> type;
	CLEAR();
	if (type == 2) {
		std::cout << "Enter array text without commas or spaces (Example: 67293): ";
		std::cin >> input;
		CLEAR();
		for (int i = 0; i < input.size(); ++i) {
			if (isdigit(input[i])) {
				arr.push_back(static_cast<int>(input[i] - '0'));
			}
		}
	}
	else if (type == 1) {
		int len;
		std::cout << "Enter the length of array: ";
		std::cin >> len;
		CLEAR();
		arr = initiateArray(len);

	}




	std::cout << "Select sort Method:\n1. - (Bubble Sort) 2. - (Insertion Sort) 3. - (Quick Sort) 4. - (Selection Sort) 5. - (Merge Sort) 6. - (Shacker Sort)" << std::endl;
	int sortType; std::cin >> sortType;
	CLEAR();
	const clock_t start = clock();
	ArraySorter sort;
	int count = 0;
	std::vector<int> SortedArray = sort.runSort(arr, sortType, &count);
	std::ofstream output_file("output.txt");
	output_file << "Method Output: " << ArraySorter::GenerateOutput(SortedArray) << "\n" << "Source Array:  " << ArraySorter::GenerateOutput(arr) << "\n\n";
	output_file.close();
	clock_t end = clock();
	double seconds = (double)(end - start) / CLOCKS_PER_SEC;
	std::cout << "Number of permutations: " << count << std::endl;
	std::cout << "Program execution time: " << seconds << " Seconds" << std::endl;

	return 0;

}
