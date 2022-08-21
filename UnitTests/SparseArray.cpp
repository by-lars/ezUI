#include "pch.h"
#include "CppUnitTest.h"

#include "Data/SparseArray.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DataStructures
{
	TEST_CLASS(SparseArray)
	{
	public:
		TEST_METHOD(PushBackAndGet) {
			ez::SparseArray<int, 200> sparseArray;

			size_t index = sparseArray.PushBack(512);
			Assert::AreEqual((size_t)0, index, L"Index not 0 after first push");

			size_t index1 = sparseArray.PushBack(1024);
			Assert::AreEqual((size_t)0, index, L"Index not 1 after second push");

			size_t index2 = sparseArray.PushBack(2048);

			int res = sparseArray.Get(index);
			int res1 = sparseArray.Get(index1);
			int res2 = sparseArray.Get(index2);

			Assert::AreEqual(512, res, L"Value at index is not the value that was pushed into the array");
			Assert::AreEqual(1024, res1, L"Value at index is not the value that was pushed into the array");
			Assert::AreEqual(2048, res2, L"Value at index is not the value that was pushed into the array");
		}

		TEST_METHOD(GetAfterErase) {
			ez::SparseArray<int, 200> sparseArray;

			size_t index = sparseArray.PushBack(600);
			size_t index2 = sparseArray.PushBack(800);

			sparseArray.Erase(index);
			int res = sparseArray.Get(index2);

			Assert::AreEqual(800, res, L"Value at swapped element is not the same after Erase");

			size_t index3 = sparseArray.PushBack(312);
			Assert::AreEqual((size_t)2, index3, L"Wrong index after push");
		}

		TEST_METHOD(SetData) {
			ez::SparseArray<int, 200> sparseArray;

			size_t index = sparseArray.PushBack(200);
			sparseArray.Set(index, 8000);
			
			int res = sparseArray.Get(index);
			Assert::AreEqual(8000, res, L"Invalid value after set() was called");
		}

		TEST_METHOD(Clear) {
			ez::SparseArray<int, 200> sparseArray;
			sparseArray.PushBack(42);
			sparseArray.PushBack(63);
			sparseArray.PushBack(123);

			Assert::AreEqual(3, (int)sparseArray.Count(), L"Invalid item count after 3 pushes");

			sparseArray.Clear();
			Assert::AreEqual(0, (int)sparseArray.Count(), L"Invalid intem count after clear");

			int data = sparseArray.DenseData()[0];
			Assert::AreEqual(0, data, L"Array is not zeroed out after clear");
		}
	};
}
