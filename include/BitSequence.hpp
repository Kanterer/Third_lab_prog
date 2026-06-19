#ifndef BIT_SEQUENCE_HPP
#define BIT_SEQUENCE_HPP

#include "Bit.hpp"
#include "DynamicArray.hpp"
#include "Sequence.hpp"

class BitSequence : public Sequence<Bit, BitSequence> {
private:
    DynamicArray<unsigned char> bytes;
    int bitCount;

    static int ByteCountForBits(int bits) {
        if (bits <= 0) {
            return 0;
        }

        return (bits + 7) / 8;
    }

    static int ByteIndex(int bitIndex) {
        return bitIndex / 8;
    }

    static unsigned char BitMask(int bitIndex) {
        return static_cast<unsigned char>(1 << (bitIndex % 8));
    }

    void ValidateIndex(int index) const {
        if (index < 0 || index >= bitCount) {
            throw IndexOutOfRangeException("bit index is outside BitSequence bounds");
        }
    }

    void ValidateSameLength(const BitSequence& other) const {
        if (GetLength() != other.GetLength()) {
            throw InvalidArgumentException("bit sequences must have equal length");
        }
    }

    void EnsureStorageForBits(int requiredBitCount) {
        int requiredByteCount = ByteCountForBits(requiredBitCount);

        if (requiredByteCount > bytes.GetSize()) {
            bytes.Resize(requiredByteCount);
        }
    }

    Bit GetBitRaw(int index) const {
        unsigned char byte = bytes.Get(ByteIndex(index));
        unsigned char mask = BitMask(index);

        return Bit((byte & mask) != 0);
    }

    void SetBitRaw(int index, Bit value) {
        int byteIndex = ByteIndex(index);
        unsigned char byte = bytes.Get(byteIndex);
        unsigned char mask = BitMask(index);

        if (value.ToBool()) {
            byte = static_cast<unsigned char>(byte | mask);
        } else {
            byte = static_cast<unsigned char>(byte & static_cast<unsigned char>(~mask));
        }

        bytes.Set(byteIndex, byte);
    }

    void AppendInternal(Bit item) {
        EnsureStorageForBits(bitCount + 1);

        SetBitRaw(bitCount, item);
        ++bitCount;
    }

    void PrependInternal(Bit item) {
        EnsureStorageForBits(bitCount + 1);

        for (int i = bitCount; i > 0; --i) {
            SetBitRaw(i, GetBitRaw(i - 1));
        }

        SetBitRaw(0, item);
        ++bitCount;
    }

    void InsertAtInternal(Bit item, int index) {
        if (index < 0 || index > bitCount) {
            throw IndexOutOfRangeException("insert index is outside BitSequence bounds");
        }

        EnsureStorageForBits(bitCount + 1);

        for (int i = bitCount; i > index; --i) {
            SetBitRaw(i, GetBitRaw(i - 1));
        }

        SetBitRaw(index, item);
        ++bitCount;
    }

protected:
    static int NormalizeSliceStart(int startIndex, int length) {
        int normalizedStart = startIndex;

        if (normalizedStart < 0) {
            normalizedStart = length + normalizedStart;
        }

        return normalizedStart;
    }

    static void ValidateSliceRange(int normalizedStart, int count, int length) {
        if (count < 0) {
            throw InvalidArgumentException("slice count cannot be negative");
        }

        if (normalizedStart < 0 || normalizedStart > length) {
            throw IndexOutOfRangeException("slice start index is outside BitSequence bounds");
        }

        if (normalizedStart + count > length) {
            throw IndexOutOfRangeException("slice removes more bits than BitSequence contains");
        }
    }

public:
    BitSequence() : bytes(), bitCount(0) {}

    explicit BitSequence(int size) : bytes(), bitCount(0) {
        if (size < 0) {
            throw InvalidArgumentException("bit sequence size cannot be negative");
        }

        bitCount = size;
        bytes.Resize(ByteCountForBits(bitCount));
    }

    BitSequence(Bit* bits, int count) : bytes(), bitCount(0) {
        if (count < 0) {
            throw InvalidArgumentException("bit sequence size cannot be negative");
        }

        if (count > 0 && bits == nullptr) {
            throw InvalidArgumentException("source bit array is null while count is positive");
        }

        for (int i = 0; i < count; ++i) {
            AppendInternal(bits[i]);
        }
    }

    explicit BitSequence(const char* bits) : bytes(), bitCount(0) {
        if (bits == nullptr) {
            throw InvalidArgumentException("bit string pointer is null");
        }

        int index = 0;

        while (bits[index] != '\0') {
            char ch = bits[index];

            if (ch == '0') {
                AppendInternal(Bit(false));
            } else if (ch == '1') {
                AppendInternal(Bit(true));
            } else {
                throw InvalidArgumentException("bit string can contain only characters 0 and 1");
            }

            ++index;
        }
    }

    BitSequence(const BitSequence& other) : bytes(other.bytes), bitCount(other.bitCount) {}

    BitSequence(BitSequence&& other) noexcept : bytes(static_cast<DynamicArray<unsigned char>&&>(other.bytes)), bitCount(other.bitCount) {
        other.bitCount = 0;
    }

    BitSequence& operator=(const BitSequence& other) {
        if (this == &other) {
            return *this;
        }

        bytes = other.bytes;
        bitCount = other.bitCount;

        return *this;
    }

    BitSequence& operator=(BitSequence&& other) noexcept {
        if (this == &other) {
            return *this;
        }

        bytes = static_cast<DynamicArray<unsigned char>&&>(other.bytes);
        bitCount = other.bitCount;

        other.bitCount = 0;

        return *this;
    }

    BitSequence* Instance() {
        return this;
    }

    BitSequence* CloneImpl() const {
        return new BitSequence(*this);
    }

    BitSequence* EmptyMutableImpl() const {
        return new BitSequence();
    }

    bool IsMutableImpl() const {
        return true;
    }

    int GetLengthImpl() const {
        return bitCount;
    }

    int GetByteCount() const {
        return bytes.GetSize();
    }

    Bit GetFirstImpl() const {
        if (bitCount == 0) {
            throw EmptySequenceException("cannot get first bit from empty BitSequence");
        }

        return GetBit(0);
    }

    Bit GetLastImpl() const {
        if (bitCount == 0) {
            throw EmptySequenceException("cannot get last bit from empty BitSequence");
        }

        return GetBit(bitCount - 1);
    }

    Bit GetImpl(int index) const {
        return GetBit(index);
    }

    Bit GetBit(int index) const {
        ValidateIndex(index);

        return GetBitRaw(index);
    }

    void SetBit(int index, Bit value) {
        ValidateIndex(index);

        SetBitRaw(index, value);
    }

    void ToggleBit(int index) {
        SetBit(index, ~GetBit(index));
    }

    BitSequence* GetSubsequenceImpl(int startIndex, int endIndex) const {
        if (startIndex < 0 || endIndex < 0 || startIndex >= bitCount || endIndex >= bitCount) {
            throw IndexOutOfRangeException("subsequence boundaries are outside BitSequence bounds");
        }

        if (startIndex > endIndex) {
            throw InvalidArgumentException("startIndex cannot be greater than endIndex");
        }

        int resultLength = endIndex - startIndex + 1;
        BitSequence* result = new BitSequence(resultLength);

        for (int i = 0; i < resultLength; ++i) {
            result->SetBit(i, GetBit(startIndex + i));
        }

        return result;
    }

    BitSequence* AppendImpl(Bit item) {
        BitSequence* target = Instance();

        target->AppendInternal(item);

        return target;
    }

    BitSequence* PrependImpl(Bit item) {
        BitSequence* target = Instance();

        target->PrependInternal(item);

        return target;
    }

    BitSequence* InsertAtImpl(Bit item, int index) {
        BitSequence* target = Instance();

        target->InsertAtInternal(item, index);

        return target;
    }

    template <class OtherDerived>
    BitSequence* ConcatImpl(const OtherDerived& sequence) {
        BitSequence* target = Instance();
        int length = sequence.GetLength();

        for (int i = 0; i < length; ++i) {
            target->AppendInternal(sequence.Get(i));
        }

        return target;
    }

    BitSequence* MapImpl(Bit (*mapper)(const Bit&)) const {
        BitSequence* result = new BitSequence(bitCount);

        for (int i = 0; i < bitCount; ++i) {
            result->SetBit(i, mapper(GetBit(i)));
        }

        return result;
    }

    BitSequence* MapIndexedImpl(Bit (*mapper)(const Bit&, int)) const {
        BitSequence* result = new BitSequence(bitCount);

        for (int i = 0; i < bitCount; ++i) {
            result->SetBit(i, mapper(GetBit(i), i));
        }

        return result;
    }

    BitSequence* WhereImpl(bool (*predicate)(const Bit&)) const {
        int resultLength = 0;

        for (int i = 0; i < bitCount; ++i) {
            Bit bit = GetBit(i);

            if (predicate(bit)) {
                ++resultLength;
            }
        }

        BitSequence* result = new BitSequence(resultLength);
        int resultIndex = 0;

        for (int i = 0; i < bitCount; ++i) {
            Bit bit = GetBit(i);

            if (predicate(bit)) {
                result->SetBit(resultIndex, bit);
                ++resultIndex;
            }
        }

        return result;
    }

    BitSequence* SliceWithoutReplacementImpl(int startIndex, int count) const {
        int normalizedStart = NormalizeSliceStart(startIndex, bitCount);

        ValidateSliceRange(normalizedStart, count, bitCount);

        int resultLength = bitCount - count;
        BitSequence* result = new BitSequence(resultLength);
        int resultIndex = 0;

        for (int i = 0; i < normalizedStart; ++i) {
            result->SetBit(resultIndex, GetBit(i));
            ++resultIndex;
        }

        for (int i = normalizedStart + count; i < bitCount; ++i) {
            result->SetBit(resultIndex, GetBit(i));
            ++resultIndex;
        }

        return result;
    }

    template <class ReplacementDerived>
    BitSequence* SliceImpl(int startIndex, int count, const ReplacementDerived* replacement) const {
        int normalizedStart = NormalizeSliceStart(startIndex, bitCount);

        ValidateSliceRange(normalizedStart, count, bitCount);

        int replacementLength = replacement == nullptr ? 0 : replacement->GetLength();
        int resultLength = bitCount - count + replacementLength;

        BitSequence* result = new BitSequence(resultLength);
        int resultIndex = 0;

        for (int i = 0; i < normalizedStart; ++i) {
            result->SetBit(resultIndex, GetBit(i));
            ++resultIndex;
        }

        if (replacement != nullptr) {
            for (int i = 0; i < replacementLength; ++i) {
                result->SetBit(resultIndex, replacement->Get(i));
                ++resultIndex;
            }
        }

        for (int i = normalizedStart + count; i < bitCount; ++i) {
            result->SetBit(resultIndex, GetBit(i));
            ++resultIndex;
        }

        return result;
    }

    BitSequence And(const BitSequence& other) const {
        ValidateSameLength(other);

        BitSequence result(GetLength());

        for (int i = 0; i < GetLength(); ++i) {
            result.SetBit(i, GetBit(i) & other.GetBit(i));
        }

        return result;
    }

    BitSequence Or(const BitSequence& other) const {
        ValidateSameLength(other);

        BitSequence result(GetLength());

        for (int i = 0; i < GetLength(); ++i) {
            result.SetBit(i, GetBit(i) | other.GetBit(i));
        }

        return result;
    }

    BitSequence Xor(const BitSequence& other) const {
        ValidateSameLength(other);

        BitSequence result(GetLength());

        for (int i = 0; i < GetLength(); ++i) {
            result.SetBit(i, GetBit(i) ^ other.GetBit(i));
        }

        return result;
    }

    BitSequence Not() const {
        BitSequence result(GetLength());

        for (int i = 0; i < GetLength(); ++i) {
            result.SetBit(i, ~GetBit(i));
        }

        return result;
    }
};

#endif
