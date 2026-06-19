#include "UI.hpp"

#include "ArraySequence.hpp"
#include "BitSequence.hpp"
#include "BitSequenceIO.hpp"
#include "DiagonalMatrix.hpp"
#include "ListSequence.hpp"
#include "MatrixIO.hpp"
#include "RectangularMatrix.hpp"
#include "SequenceIO.hpp"
#include "SquareMatrix.hpp"
#include "SparseMatrix.hpp"
#include "TriangularMatrix.hpp"
#include "Vector.hpp"
#include "VectorIO.hpp"

#include <iostream>
#include <limits>
#include <string>

namespace {
    int ReadInt(const std::string& prompt) {
        while (true) {
            std::cout << prompt;

            int value;
            if (std::cin >> value) {
                return value;
            }

            std::cout << "Invalid input. Please enter an integer.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    double ReadDouble(const std::string& prompt) {
        while (true) {
            std::cout << prompt;

            double value;
            if (std::cin >> value) {
                return value;
            }

            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::string ReadString(const std::string& prompt) {
        std::cout << prompt;

        std::string value;
        std::cin >> value;

        return value;
    }

    int MultiplyByTwo(const int& value) {
        return value * 2;
    }

    bool IsEven(const int& value) {
        return value % 2 == 0;
    }

    int SumReducer(const int& accumulator, const int& value) {
        return accumulator + value;
    }

    template <class SequenceType>
    void PrintIntSequence(const SequenceType* sequence) {
        std::cout << "Current sequence: ";

        if (sequence == nullptr) {
            std::cout << "<not created>\n";
            return;
        }

        PrintSequence(*sequence, std::cout);
        std::cout << " | length = " << sequence->GetLength()
                  << " | " << (sequence->IsMutable() ? "mutable" : "immutable")
                  << "\n";
    }

    template <class SequenceType>
    void ReplaceSequenceIfNeeded(SequenceType*& sequence, SequenceType* result) {
        if (result != sequence) {
            delete sequence;
            sequence = result;
        }
    }

    template <class SequenceType>
    void RunIntSequenceMenu(SequenceType* sequence) {
        bool running = true;

        while (running) {
            try {
                std::cout << "\n==== Sequence<int> menu ====\n";
                PrintIntSequence(sequence);

                std::cout << "1. Append(value)\n";
                std::cout << "2. Prepend(value)\n";
                std::cout << "3. InsertAt(value, index)\n";
                std::cout << "4. Get(index)\n";
                std::cout << "5. GetSubsequence(start, end)\n";
                std::cout << "6. Map: x -> x * 2\n";
                std::cout << "7. Where: keep even numbers\n";
                std::cout << "8. Reduce: sum\n";
                std::cout << "0. Back\n";

                int command = ReadInt("Command: ");

                switch (command) {
                    case 1: {
                        int value = ReadInt("value = ");
                        ReplaceSequenceIfNeeded(sequence, sequence->Append(value));
                        break;
                    }

                    case 2: {
                        int value = ReadInt("value = ");
                        ReplaceSequenceIfNeeded(sequence, sequence->Prepend(value));
                        break;
                    }

                    case 3: {
                        int value = ReadInt("value = ");
                        int index = ReadInt("index = ");
                        ReplaceSequenceIfNeeded(sequence, sequence->InsertAt(value, index));
                        break;
                    }

                    case 4: {
                        int index = ReadInt("index = ");
                        std::cout << "sequence[" << index << "] = " << sequence->Get(index) << "\n";
                        break;
                    }

                    case 5: {
                        int start = ReadInt("startIndex = ");
                        int end = ReadInt("endIndex = ");
                        auto* sub = sequence->GetSubsequence(start, end);

                        std::cout << "Subsequence: ";
                        PrintSequence(*sub, std::cout);
                        std::cout << "\n";

                        delete sub;
                        break;
                    }

                    case 6: {
                        auto* mapped = sequence->Map(MultiplyByTwo);
                        std::cout << "Map result: ";
                        PrintSequence(*mapped, std::cout);
                        std::cout << "\n";
                        delete mapped;
                        break;
                    }

                    case 7: {
                        auto* filtered = sequence->Where(IsEven);
                        std::cout << "Where result: ";
                        PrintSequence(*filtered, std::cout);
                        std::cout << "\n";
                        delete filtered;
                        break;
                    }

                    case 8: {
                        int sum = sequence->template Reduce<int>(SumReducer, 0);
                        std::cout << "Sum = " << sum << "\n";
                        break;
                    }

                    case 0:
                        running = false;
                        break;

                    default:
                        std::cout << "Unknown command.\n";
                        break;
                }
            } catch (const std::exception& ex) {
                std::cout << "Error: " << ex.what() << "\n";
            }
        }

        delete sequence;
    }

    void RunBitSequenceMenu() {
        try {
            std::string input = ReadString("Enter bit string, for example 101001: ");
            BitSequence bits(input.c_str());

            bool running = true;

            while (running) {
                try {
                    std::cout << "\n==== BitSequence menu ====\n";
                    std::cout << "Current bit sequence: " << bits << "\n";
                    std::cout << "Stored bytes = " << bits.GetByteCount() << "\n";

                    std::cout << "1. NOT\n";
                    std::cout << "2. AND with another bit string\n";
                    std::cout << "3. OR with another bit string\n";
                    std::cout << "4. XOR with another bit string\n";
                    std::cout << "5. SetBit(index, value)\n";
                    std::cout << "6. ToggleBit(index)\n";
                    std::cout << "0. Back\n";

                    int command = ReadInt("Command: ");

                    switch (command) {
                        case 1:
                            std::cout << "NOT result: " << bits.Not() << "\n";
                            break;

                        case 2: {
                            std::string otherInput = ReadString("Another bit string: ");
                            BitSequence other(otherInput.c_str());
                            std::cout << "AND result: " << bits.And(other) << "\n";
                            break;
                        }

                        case 3: {
                            std::string otherInput = ReadString("Another bit string: ");
                            BitSequence other(otherInput.c_str());
                            std::cout << "OR result: " << bits.Or(other) << "\n";
                            break;
                        }

                        case 4: {
                            std::string otherInput = ReadString("Another bit string: ");
                            BitSequence other(otherInput.c_str());
                            std::cout << "XOR result: " << bits.Xor(other) << "\n";
                            break;
                        }

                        case 5: {
                            int index = ReadInt("index = ");
                            int value = ReadInt("value 0/1 = ");
                            bits.SetBit(index, Bit(value));
                            break;
                        }

                        case 6: {
                            int index = ReadInt("index = ");
                            bits.ToggleBit(index);
                            break;
                        }

                        case 0:
                            running = false;
                            break;

                        default:
                            std::cout << "Unknown command.\n";
                            break;
                    }
                } catch (const std::exception& ex) {
                    std::cout << "Error: " << ex.what() << "\n";
                }
            }
        } catch (const std::exception& ex) {
            std::cout << "BitSequence creation error: " << ex.what() << "\n";
        }
    }

    Vector<double> ReadVector() {
        int size = ReadInt("size = ");
        Vector<double> vector(size);

        for (int i = 0; i < size; ++i) {
            vector.Set(i, ReadDouble("item[" + std::to_string(i) + "] = "));
        }

        return vector;
    }

    RectangularMatrix<double> ReadRectangularMatrix() {
        int rows = ReadInt("rows = ");
        int columns = ReadInt("columns = ");
        RectangularMatrix<double> matrix(rows, columns);

        for (int row = 0; row < rows; ++row) {
            for (int column = 0; column < columns; ++column) {
                matrix.Set(row, column, ReadDouble("item[" + std::to_string(row) + "][" + std::to_string(column) + "] = "));
            }
        }

        return matrix;
    }

    SquareMatrix<double> ReadSquareMatrix() {
        int size = ReadInt("size = ");
        SquareMatrix<double> matrix(size);

        for (int row = 0; row < size; ++row) {
            for (int column = 0; column < size; ++column) {
                matrix.Set(row, column, ReadDouble("item[" + std::to_string(row) + "][" + std::to_string(column) + "] = "));
            }
        }

        return matrix;
    }

    TriangularMatrix<double> ReadTriangularMatrix() {
        int size = ReadInt("size = ");
        int typeCommand = ReadInt("type: 1 - upper, 2 - lower: ");
        TriangleType type = typeCommand == 2 ? LowerTriangle : UpperTriangle;
        TriangularMatrix<double> matrix(size, type);

        for (int row = 0; row < size; ++row) {
            for (int column = 0; column < size; ++column) {
                bool stored = (type == UpperTriangle && row <= column) ||
                              (type == LowerTriangle && row >= column);

                if (stored) {
                    matrix.Set(row, column, ReadDouble("stored item[" + std::to_string(row) + "][" + std::to_string(column) + "] = "));
                }
            }
        }

        return matrix;
    }



    SparseMatrix<double> ReadSparseMatrix() {
        int rows = ReadInt("rows = ");
        int columns = ReadInt("columns = ");
        int storedItemsCount = ReadInt("non-zero item count = ");

        if (storedItemsCount < 0) {
            throw InvalidArgumentException("non-zero item count cannot be negative");
        }

        SparseMatrix<double> matrix(rows, columns);

        for (int i = 0; i < storedItemsCount; ++i) {
            int row = ReadInt("row for item " + std::to_string(i) + " = ");
            int column = ReadInt("column for item " + std::to_string(i) + " = ");
            double value = ReadDouble("value for item " + std::to_string(i) + " = ");
            matrix.Set(row, column, value);
        }

        return matrix;
    }

    DiagonalMatrix<double> ReadDiagonalMatrix() {
        int size = ReadInt("size = ");
        DiagonalMatrix<double> matrix(size);

        for (int i = 0; i < size; ++i) {
            matrix.Set(i, i, ReadDouble("diagonal[" + std::to_string(i) + "] = "));
        }

        return matrix;
    }

    void RunVectorMenu() {
        try {
            Vector<double> vector = ReadVector();
            bool running = true;

            while (running) {
                std::cout << "\n==== Vector<double> menu ====\n";
                std::cout << "Current vector: " << vector << "\n";
                std::cout << "Stored items = " << vector.GetStoredItemsCount() << "\n";
                std::cout << "1. Add another vector\n";
                std::cout << "2. Multiply by scalar\n";
                std::cout << "3. Norm\n";
                std::cout << "4. Dot product with another vector\n";
                std::cout << "0. Back\n";

                int command = ReadInt("Command: ");

                try {
                    switch (command) {
                        case 1: {
                            Vector<double> other = ReadVector();
                            Vector<double>* result = vector.Add(other);
                            std::cout << "Result: " << *result << "\n";
                            delete result;
                            break;
                        }

                        case 2: {
                            double scalar = ReadDouble("scalar = ");
                            Vector<double>* result = vector.MultiplyByScalar(scalar);
                            std::cout << "Result: " << *result << "\n";
                            delete result;
                            break;
                        }

                        case 3:
                            std::cout << "Norm = " << vector.Norm() << "\n";
                            break;

                        case 4: {
                            Vector<double> other = ReadVector();
                            std::cout << "Dot product = " << vector.DotProduct(other) << "\n";
                            break;
                        }

                        case 0:
                            running = false;
                            break;

                        default:
                            std::cout << "Unknown command.\n";
                            break;
                    }
                } catch (const std::exception& ex) {
                    std::cout << "Error: " << ex.what() << "\n";
                }
            }
        } catch (const std::exception& ex) {
            std::cout << "Vector creation error: " << ex.what() << "\n";
        }
    }

    template <class MatrixType>
    void PrintStoredCount(const MatrixType& matrix) {
        std::cout << "Stored items = " << matrix.GetStoredItemsCount() << "\n";
    }

    void RunRectangularMatrixMenu() {
        try {
            RectangularMatrix<double> matrix = ReadRectangularMatrix();
            bool running = true;

            while (running) {
                std::cout << "\n==== RectangularMatrix<double> menu ====\n";
                std::cout << matrix << "\n";
                PrintStoredCount(matrix);
                std::cout << "1. Add another matrix\n";
                std::cout << "2. Multiply by scalar\n";
                std::cout << "3. Norm\n";
                std::cout << "4. Swap rows\n";
                std::cout << "5. Swap columns\n";
                std::cout << "0. Back\n";

                int command = ReadInt("Command: ");

                try {
                    switch (command) {
                        case 1: {
                            RectangularMatrix<double> other = ReadRectangularMatrix();
                            RectangularMatrix<double>* result = matrix.Add(other);
                            std::cout << "Result:\n" << *result << "\n";
                            delete result;
                            break;
                        }

                        case 2: {
                            double scalar = ReadDouble("scalar = ");
                            RectangularMatrix<double>* result = matrix.MultiplyByScalar(scalar);
                            std::cout << "Result:\n" << *result << "\n";
                            delete result;
                            break;
                        }

                        case 3:
                            std::cout << "Norm = " << matrix.Norm() << "\n";
                            break;

                        case 4: {
                            int first = ReadInt("first row = ");
                            int second = ReadInt("second row = ");
                            matrix.SwapRows(first, second);
                            break;
                        }

                        case 5: {
                            int first = ReadInt("first column = ");
                            int second = ReadInt("second column = ");
                            matrix.SwapColumns(first, second);
                            break;
                        }

                        case 0:
                            running = false;
                            break;

                        default:
                            std::cout << "Unknown command.\n";
                            break;
                    }
                } catch (const std::exception& ex) {
                    std::cout << "Error: " << ex.what() << "\n";
                }
            }
        } catch (const std::exception& ex) {
            std::cout << "Matrix creation error: " << ex.what() << "\n";
        }
    }

    void RunSquareMatrixMenu() {
        try {
            SquareMatrix<double> matrix = ReadSquareMatrix();
            bool running = true;

            while (running) {
                std::cout << "\n==== SquareMatrix<double> menu ====\n";
                std::cout << matrix << "\n";
                PrintStoredCount(matrix);
                std::cout << "1. Multiply by scalar\n";
                std::cout << "2. Norm\n";
                std::cout << "3. Swap rows\n";
                std::cout << "4. Swap columns\n";
                std::cout << "0. Back\n";

                int command = ReadInt("Command: ");

                try {
                    switch (command) {
                        case 1: {
                            double scalar = ReadDouble("scalar = ");
                            SquareMatrix<double>* result = matrix.MultiplyByScalar(scalar);
                            std::cout << "Result:\n" << *result << "\n";
                            delete result;
                            break;
                        }

                        case 2:
                            std::cout << "Norm = " << matrix.Norm() << "\n";
                            break;

                        case 3: {
                            int first = ReadInt("first row = ");
                            int second = ReadInt("second row = ");
                            matrix.SwapRows(first, second);
                            break;
                        }

                        case 4: {
                            int first = ReadInt("first column = ");
                            int second = ReadInt("second column = ");
                            matrix.SwapColumns(first, second);
                            break;
                        }

                        case 0:
                            running = false;
                            break;

                        default:
                            std::cout << "Unknown command.\n";
                            break;
                    }
                } catch (const std::exception& ex) {
                    std::cout << "Error: " << ex.what() << "\n";
                }
            }
        } catch (const std::exception& ex) {
            std::cout << "Matrix creation error: " << ex.what() << "\n";
        }
    }

    void RunTriangularMatrixMenu() {
        try {
            TriangularMatrix<double> matrix = ReadTriangularMatrix();
            bool running = true;

            while (running) {
                std::cout << "\n==== TriangularMatrix<double> menu ====\n";
                std::cout << matrix << "\n";
                PrintStoredCount(matrix);
                std::cout << "1. Multiply by scalar\n";
                std::cout << "2. Norm\n";
                std::cout << "3. Set value\n";
                std::cout << "0. Back\n";

                int command = ReadInt("Command: ");

                try {
                    switch (command) {
                        case 1: {
                            double scalar = ReadDouble("scalar = ");
                            TriangularMatrix<double>* result = matrix.MultiplyByScalar(scalar);
                            std::cout << "Result:\n" << *result << "\n";
                            delete result;
                            break;
                        }

                        case 2:
                            std::cout << "Norm = " << matrix.Norm() << "\n";
                            break;

                        case 3: {
                            int row = ReadInt("row = ");
                            int column = ReadInt("column = ");
                            double value = ReadDouble("value = ");
                            matrix.Set(row, column, value);
                            break;
                        }

                        case 0:
                            running = false;
                            break;

                        default:
                            std::cout << "Unknown command.\n";
                            break;
                    }
                } catch (const std::exception& ex) {
                    std::cout << "Error: " << ex.what() << "\n";
                }
            }
        } catch (const std::exception& ex) {
            std::cout << "Matrix creation error: " << ex.what() << "\n";
        }
    }



    void RunSparseMatrixMenu() {
        try {
            SparseMatrix<double> matrix = ReadSparseMatrix();
            bool running = true;

            while (running) {
                std::cout << "\n==== SparseMatrix<double> menu ====\n";
                std::cout << matrix << "\n";
                PrintStoredCount(matrix);
                std::cout << "1. Add another sparse matrix\n";
                std::cout << "2. Multiply by scalar\n";
                std::cout << "3. Multiply by another sparse matrix\n";
                std::cout << "4. Norm\n";
                std::cout << "5. Set value\n";
                std::cout << "0. Back\n";

                int command = ReadInt("Command: ");

                try {
                    switch (command) {
                        case 1: {
                            SparseMatrix<double> other = ReadSparseMatrix();
                            SparseMatrix<double>* result = matrix.Add(other);
                            std::cout << "Result:\n" << *result << "\n";
                            PrintStoredCount(*result);
                            delete result;
                            break;
                        }

                        case 2: {
                            double scalar = ReadDouble("scalar = ");
                            SparseMatrix<double>* result = matrix.MultiplyByScalar(scalar);
                            std::cout << "Result:\n" << *result << "\n";
                            PrintStoredCount(*result);
                            delete result;
                            break;
                        }

                        case 3: {
                            SparseMatrix<double> other = ReadSparseMatrix();
                            SparseMatrix<double>* result = matrix.Multiply(other);
                            std::cout << "Result:\n" << *result << "\n";
                            PrintStoredCount(*result);
                            delete result;
                            break;
                        }

                        case 4:
                            std::cout << "Norm = " << matrix.Norm() << "\n";
                            break;

                        case 5: {
                            int row = ReadInt("row = ");
                            int column = ReadInt("column = ");
                            double value = ReadDouble("value = ");
                            matrix.Set(row, column, value);
                            break;
                        }

                        case 0:
                            running = false;
                            break;

                        default:
                            std::cout << "Unknown command.\n";
                            break;
                    }
                } catch (const std::exception& ex) {
                    std::cout << "Error: " << ex.what() << "\n";
                }
            }
        } catch (const std::exception& ex) {
            std::cout << "Sparse matrix creation error: " << ex.what() << "\n";
        }
    }

    void RunDiagonalMatrixMenu() {
        try {
            DiagonalMatrix<double> matrix = ReadDiagonalMatrix();
            bool running = true;

            while (running) {
                std::cout << "\n==== DiagonalMatrix<double> menu ====\n";
                std::cout << matrix << "\n";
                PrintStoredCount(matrix);
                std::cout << "1. Multiply by scalar\n";
                std::cout << "2. Norm\n";
                std::cout << "3. Set value\n";
                std::cout << "0. Back\n";

                int command = ReadInt("Command: ");

                try {
                    switch (command) {
                        case 1: {
                            double scalar = ReadDouble("scalar = ");
                            DiagonalMatrix<double>* result = matrix.MultiplyByScalar(scalar);
                            std::cout << "Result:\n" << *result << "\n";
                            delete result;
                            break;
                        }

                        case 2:
                            std::cout << "Norm = " << matrix.Norm() << "\n";
                            break;

                        case 3: {
                            int row = ReadInt("row = ");
                            int column = ReadInt("column = ");
                            double value = ReadDouble("value = ");
                            matrix.Set(row, column, value);
                            break;
                        }

                        case 0:
                            running = false;
                            break;

                        default:
                            std::cout << "Unknown command.\n";
                            break;
                    }
                } catch (const std::exception& ex) {
                    std::cout << "Error: " << ex.what() << "\n";
                }
            }
        } catch (const std::exception& ex) {
            std::cout << "Matrix creation error: " << ex.what() << "\n";
        }
    }
}

void RunUI() {
    bool running = true;

    while (running) {
        std::cout << "\n========== Laboratory work 3 ==========" << "\n";
        std::cout << "1. MutableArraySequence<int>\n";
        std::cout << "2. ImmutableArraySequence<int>\n";
        std::cout << "3. MutableListSequence<int>\n";
        std::cout << "4. ImmutableListSequence<int>\n";
        std::cout << "5. BitSequence\n";
        std::cout << "6. Vector<double>\n";
        std::cout << "7. RectangularMatrix<double>\n";
        std::cout << "8. SquareMatrix<double>\n";
        std::cout << "9. TriangularMatrix<double>\n";
        std::cout << "10. DiagonalMatrix<double>\n";
        std::cout << "11. SparseMatrix<double>\n";
        std::cout << "0. Exit\n";

        int command = ReadInt("Choose data structure: ");

        switch (command) {
            case 1:
                RunIntSequenceMenu(new MutableArraySequence<int>());
                break;

            case 2:
                RunIntSequenceMenu(new ImmutableArraySequence<int>());
                break;

            case 3:
                RunIntSequenceMenu(new MutableListSequence<int>());
                break;

            case 4:
                RunIntSequenceMenu(new ImmutableListSequence<int>());
                break;

            case 5:
                RunBitSequenceMenu();
                break;

            case 6:
                RunVectorMenu();
                break;

            case 7:
                RunRectangularMatrixMenu();
                break;

            case 8:
                RunSquareMatrixMenu();
                break;

            case 9:
                RunTriangularMatrixMenu();
                break;

            case 10:
                RunDiagonalMatrixMenu();
                break;

            case 11:
                RunSparseMatrixMenu();
                break;

            case 0:
                std::cout << "Program finished.\n";
                running = false;
                break;

            default:
                std::cout << "Unknown command.\n";
                break;
        }
    }
}
