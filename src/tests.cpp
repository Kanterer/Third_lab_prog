#include "Tests.hpp"

#include "ArraySequence.hpp"
#include "BitSequence.hpp"
#include "DiagonalMatrix.hpp"
#include "DynamicArray.hpp"
#include "LinkedList.hpp"
#include "ListSequence.hpp"
#include "RectangularMatrix.hpp"
#include "SquareMatrix.hpp"
#include "SparseMatrix.hpp"
#include "TriangularMatrix.hpp"
#include "Vector.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <string>

namespace {
    int testsRun = 0;

    template <class Expected, class Actual>
    void ExpectEq(const Expected& expected, const Actual& actual) {
        ++testsRun;
        assert(expected == actual);
    }

    void ExpectNear(double expected, double actual) {
        ++testsRun;
        assert(std::fabs(expected - actual) < 1e-9);
    }

    void ExpectTrue(bool condition) {
        ++testsRun;
        assert(condition);
    }

    int DoubleValue(const int& value) {
        return value * 2;
    }

    int AddIndex(const int& value, int index) {
        return value + index;
    }

    bool IsEven(const int& value) {
        return value % 2 == 0;
    }

    int SumReducer(const int& accumulator, const int& value) {
        return accumulator + value;
    }

    bool GreaterThanThree(const int& value) {
        return value > 3;
    }

    bool GreaterThanHundred(const int& value) {
        return value > 100;
    }

    std::string BitSequenceToString(const BitSequence& bits) {
        std::string result;

        for (int i = 0; i < bits.GetLength(); ++i) {
            result += bits.GetBit(i).ToInt() == 0 ? '0' : '1';
        }

        return result;
    }

    void TestDynamicArray() {
        int source[] = {1, 2, 3};
        DynamicArray<int> array(source, 3);

        ExpectEq(3, array.GetSize());
        ExpectEq(2, array.Get(1));
        ExpectTrue(array.GetCapacity() >= array.GetSize());

        array.Set(1, 10);
        ExpectEq(10, array.Get(1));

        array.Resize(5);
        ExpectEq(5, array.GetSize());
        ExpectEq(1, array.Get(0));
        ExpectEq(10, array.Get(1));
        ExpectEq(3, array.Get(2));

        array.Resize(2);
        ExpectEq(2, array.GetSize());
        ExpectEq(10, array.Get(1));

        bool indexExceptionWasThrown = false;
        try {
            array.Get(2);
        } catch (const IndexOutOfRangeException&) {
            indexExceptionWasThrown = true;
        }
        ExpectTrue(indexExceptionWasThrown);
    }

    void TestLinkedList() {
        int source[] = {2, 3, 4};
        LinkedList<int> list(source, 3);

        list.Prepend(1);
        list.Append(5);
        list.InsertAt(100, 2);

        ExpectEq(6, list.GetLength());
        ExpectEq(1, list.GetFirst());
        ExpectEq(5, list.GetLast());
        ExpectEq(100, list.Get(2));

        LinkedList<int>* sub = list.GetSubList(1, 3);

        ExpectEq(3, sub->GetLength());
        ExpectEq(2, sub->Get(0));
        ExpectEq(3, sub->Get(2));

        delete sub;
    }

    void TestSequences() {
        int source[] = {1, 2, 3, 4, 5};
        MutableArraySequence<int> sequence(source, 5);

        MutableArraySequence<int>* mapped = sequence.Map(DoubleValue);
        ExpectEq(2, mapped->Get(0));
        ExpectEq(10, mapped->Get(4));
        delete mapped;

        MutableArraySequence<int>* indexed = sequence.MapIndexed(AddIndex);
        ExpectEq(1, indexed->Get(0));
        ExpectEq(7, indexed->Get(3));
        delete indexed;

        MutableArraySequence<int>* even = sequence.Where(IsEven);
        ExpectEq(2, even->GetLength());
        ExpectEq(2, even->Get(0));
        ExpectEq(4, even->Get(1));
        delete even;

        int sum = sequence.Reduce<int>(SumReducer, 0);
        ExpectEq(15, sum);

        Option<int> found = sequence.TryGetFirst(GreaterThanThree);
        ExpectTrue(found.HasValue());
        ExpectEq(4, found.Value());

        Option<int> notFound = sequence.TryGetLast(GreaterThanHundred);
        ExpectTrue(notFound.IsNone());
    }

    void TestBitSequence() {
        BitSequence a("1010");
        BitSequence b("1100");

        ExpectEq(std::string("1000"), BitSequenceToString(a.And(b)));
        ExpectEq(std::string("1110"), BitSequenceToString(a.Or(b)));
        ExpectEq(std::string("0110"), BitSequenceToString(a.Xor(b)));
        ExpectEq(std::string("0101"), BitSequenceToString(a.Not()));

        ExpectEq(1, a.GetByteCount());

        BitSequence manyBits("101010101");
        ExpectEq(9, manyBits.GetLength());
        ExpectEq(2, manyBits.GetByteCount());
    }

    void TestVector() {
        double leftItems[] = {1.0, 2.0, 2.0};
        double rightItems[] = {3.0, 4.0, 5.0};

        Vector<double> left(leftItems, 3);
        Vector<double> right(rightItems, 3);

        Vector<double>* sum = left.Add(right);
        ExpectNear(4.0, sum->Get(0));
        ExpectNear(6.0, sum->Get(1));
        ExpectNear(7.0, sum->Get(2));
        delete sum;

        Vector<double>* scaled = left.MultiplyByScalar(2.0);
        ExpectNear(2.0, scaled->Get(0));
        ExpectNear(4.0, scaled->Get(1));
        ExpectNear(4.0, scaled->Get(2));
        delete scaled;

        ExpectNear(21.0, left.DotProduct(right));
        ExpectNear(3.0, left.Norm());
        ExpectEq(3, left.GetStoredItemsCount());
    }

    void TestRectangularMatrix() {
        double leftItems[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
        double rightItems[] = {10.0, 20.0, 30.0, 40.0, 50.0, 60.0};

        RectangularMatrix<double> left(2, 3, leftItems);
        RectangularMatrix<double> right(2, 3, rightItems);

        ExpectEq(6, left.GetStoredItemsCount());
        ExpectNear(6.0, left.Get(1, 2));

        RectangularMatrix<double>* sum = left.Add(right);
        ExpectNear(11.0, sum->Get(0, 0));
        ExpectNear(66.0, sum->Get(1, 2));
        delete sum;

        left.SwapRows(0, 1);
        ExpectNear(4.0, left.Get(0, 0));
        ExpectNear(1.0, left.Get(1, 0));

        left.SwapColumns(0, 2);
        ExpectNear(6.0, left.Get(0, 0));
        ExpectNear(4.0, left.Get(0, 2));
    }

    void TestSquareMatrix() {
        double items[] = {1.0, 2.0, 3.0, 4.0};
        SquareMatrix<double> matrix(2, items);

        ExpectEq(4, matrix.GetStoredItemsCount());
        ExpectNear(4.0, matrix.Get(1, 1));
        ExpectNear(std::sqrt(30.0), matrix.Norm());

        SquareMatrix<double>* scaled = matrix.MultiplyByScalar(2.0);
        ExpectNear(8.0, scaled->Get(1, 1));
        delete scaled;
    }

    void TestTriangularMatrix() {
        TriangularMatrix<double> upper(3, UpperTriangle);

        upper.Set(0, 0, 1.0);
        upper.Set(0, 1, 2.0);
        upper.Set(0, 2, 3.0);
        upper.Set(1, 1, 4.0);
        upper.Set(1, 2, 5.0);
        upper.Set(2, 2, 6.0);

        ExpectEq(6, upper.GetStoredItemsCount());
        ExpectNear(0.0, upper.Get(2, 0));
        ExpectNear(5.0, upper.Get(1, 2));
        ExpectNear(std::sqrt(91.0), upper.Norm());

        bool badSetWasThrown = false;
        try {
            upper.Set(2, 0, 7.0);
        } catch (const InvalidArgumentException&) {
            badSetWasThrown = true;
        }
        ExpectTrue(badSetWasThrown);

        TriangularMatrix<double>* scaled = upper.MultiplyByScalar(2.0);
        ExpectNear(10.0, scaled->Get(1, 2));
        ExpectNear(0.0, scaled->Get(2, 0));
        delete scaled;

        TriangularMatrix<double> lower(4, LowerTriangle);
        ExpectEq(10, lower.GetStoredItemsCount());
        lower.Set(3, 0, 9.0);
        ExpectNear(9.0, lower.Get(3, 0));
        ExpectNear(0.0, lower.Get(0, 3));
    }



    void TestSparseMatrix() {
        double items[] = {
            1.0, 0.0, 0.0,
            0.0, 2.0, 0.0,
            3.0, 0.0, 4.0
        };

        SparseMatrix<double> matrix(3, 3, items);

        ExpectEq(4, matrix.GetStoredItemsCount());
        ExpectNear(1.0, matrix.Get(0, 0));
        ExpectNear(0.0, matrix.Get(0, 1));
        ExpectNear(3.0, matrix.Get(2, 0));
        ExpectNear(std::sqrt(30.0), matrix.Norm());

        matrix.Set(0, 1, 5.0);
        ExpectEq(5, matrix.GetStoredItemsCount());
        ExpectNear(5.0, matrix.Get(0, 1));

        matrix.Set(0, 1, 0.0);
        ExpectEq(4, matrix.GetStoredItemsCount());
        ExpectNear(0.0, matrix.Get(0, 1));

        SparseMatrix<double>* scaled = matrix.MultiplyByScalar(2.0);
        ExpectEq(4, scaled->GetStoredItemsCount());
        ExpectNear(8.0, scaled->Get(2, 2));
        delete scaled;

        SparseMatrix<double>* zeroScaled = matrix.MultiplyByScalar(0.0);
        ExpectEq(0, zeroScaled->GetStoredItemsCount());
        ExpectNear(0.0, zeroScaled->Get(2, 2));
        delete zeroScaled;

        double otherItems[] = {
            0.0, 10.0, 0.0,
            0.0, 0.0, 20.0,
            30.0, 0.0, 0.0
        };

        SparseMatrix<double> other(3, 3, otherItems);
        SparseMatrix<double>* sum = matrix.Add(other);
        ExpectEq(6, sum->GetStoredItemsCount());
        ExpectNear(10.0, sum->Get(0, 1));
        ExpectNear(20.0, sum->Get(1, 2));
        ExpectNear(33.0, sum->Get(2, 0));
        delete sum;

        SparseMatrix<double>* product = matrix.Multiply(other);
        ExpectEq(4, product->GetStoredItemsCount());
        ExpectNear(10.0, product->Get(0, 1));
        ExpectNear(40.0, product->Get(1, 2));
        ExpectNear(120.0, product->Get(2, 0));
        ExpectNear(30.0, product->Get(2, 1));
        delete product;

        bool badGetWasThrown = false;
        try {
            matrix.Get(3, 0);
        } catch (const IndexOutOfRangeException&) {
            badGetWasThrown = true;
        }
        ExpectTrue(badGetWasThrown);

        SparseMatrix<double> incompatible(2, 2);
        bool badMultiplyWasThrown = false;
        try {
            matrix.Multiply(incompatible);
        } catch (const InvalidArgumentException&) {
            badMultiplyWasThrown = true;
        }
        ExpectTrue(badMultiplyWasThrown);
    }

    void TestDiagonalMatrix() {
        double diagonalItems[] = {1.0, 2.0, 3.0};
        DiagonalMatrix<double> matrix(diagonalItems, 3);

        ExpectEq(3, matrix.GetStoredItemsCount());
        ExpectNear(2.0, matrix.Get(1, 1));
        ExpectNear(0.0, matrix.Get(0, 2));
        ExpectNear(std::sqrt(14.0), matrix.Norm());

        bool badSetWasThrown = false;
        try {
            matrix.Set(0, 2, 5.0);
        } catch (const InvalidArgumentException&) {
            badSetWasThrown = true;
        }
        ExpectTrue(badSetWasThrown);

        matrix.Set(0, 2, 0.0);
        ExpectNear(0.0, matrix.Get(0, 2));

        DiagonalMatrix<double>* scaled = matrix.MultiplyByScalar(3.0);
        ExpectNear(9.0, scaled->Get(2, 2));
        delete scaled;
    }
}

void RunAllTests() {
    testsRun = 0;

    TestDynamicArray();
    TestLinkedList();
    TestSequences();
    TestBitSequence();
    TestVector();
    TestRectangularMatrix();
    TestSquareMatrix();
    TestTriangularMatrix();
    TestDiagonalMatrix();
    TestSparseMatrix();

    std::cout << "All strict tests passed: " << testsRun << " checks." << std::endl;
}
