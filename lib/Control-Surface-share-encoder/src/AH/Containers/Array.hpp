/* ✔ */

#pragma once

#include <AH/Settings/Warnings.hpp>

AH_DIAGNOSTIC_WERROR() // Enable errors on warnings

#include <AH/Error/Error.hpp>
#include <AH/STL/iterator>
#include <AH/STL/type_traits> // conditional
#include <stddef.h>           // size_t

BEGIN_AH_NAMESPACE

template <class T>
constexpr T abs_diff(const T &a, const T &b) {
    return a < b ? b - a : a - b;
}

/// @addtogroup AH_Containers
/// @{

template <class T, size_t N, bool Reverse, bool Const>
class ArraySlice;

/**
 * @brief   An array wrapper for easy copying, comparing, and iterating.
 * 
 * @tparam  T
 *          The type of the elements in the array.
 * @tparam  N 
 *          The number of elements in the array.
 */
template <class T, size_t N>
struct Array {
    T data[N];
    using type = T;
    constexpr static size_t length = N;

    /**
     * @brief   Get the element at the given index.
     * 
     * @note    Bounds checking is performed. If fatal errors are disabled, the
     *          last element is returned if the index is out of bounds. 
     * 
     * @param   index
     *          The (zero-based) index of the element to return.
     */
    T &operator[](size_t index) {
        if (index >= N) { // TODO
            ERROR(F("Index out of bounds: ") << index << F(" ≥ ") << N, 0xEDED);
            index = N - 1; // LCOV_EXCL_LINE
        }                  // LCOV_EXCL_LINE
        return data[index];
    }

    /**
     * @brief   Get the element at the given index.
     * 
     * @note    Bounds checking is performed. If fatal errors are disabled, the
     *          last element is returned if the index is out of bounds. 
     * 
     * @param   index
     *          The (zero-based) index of the element to return.
     */
    const T &operator[](size_t index) const {
        if (index >= N) { // TODO
            ERROR(F("Index out of bounds: ") << index << F(" ≥ ") << N, 0xEDED);
            index = N - 1; // LCOV_EXCL_LINE
        }                  // LCOV_EXCL_LINE
        return data[index];
    }

    /**
     * @brief   Get a pointer to the first element.
     */
    T *begin() { return &data[0]; }

    /**
     * @brief   Get a pointer to the first element.
     */
    const T *begin() const { return &data[0]; }

    /**
     * @brief   Get a pointer to the memory beyond the array.
     */
    T *end() { return &data[N]; }

    /**
     * @brief   Get a pointer to the memory beyond the array.
     */
    const T *end() const { return &data[N]; }

    /**
     * @brief   Check the equality of all elements in two arrays.
     * 
     * @param   rhs 
     *          The array to compare this array to.
     */
    bool operator==(const Array<T, N> &rhs) const {
        if (this == &rhs)
            return true;
        for (size_t i = 0; i < N; i++)
            if ((*this)[i] != rhs[i])
                return false;
        return true;
    }

    /**
     * @brief   Check the inequality of all elements in two arrays.
     * 
     * @param   rhs 
     *          The array to compare this array to.
     */
    bool operator!=(const Array<T, N> &rhs) const { return !(*this == rhs); }

  public:
    /**
     * @brief   Get a view on a slice of the Array.
     * 
     * Doesn't copy the contents of the array, it's just a reference to the 
     * original array.
     * 
     * @tparam  Start
     *          The start index of the slice.
     * @tparam  End
     *          The end index of the slice.
     */
    template <size_t Start = 0, size_t End = N - 1>
    ArraySlice<T, abs_diff(Start, End) + 1, (End < Start), false> slice();

    /**
     * @brief   Get a read-only view on a slice of the Array.
     * @copydetails     slice()
     */
    template <size_t Start = 0, size_t End = N - 1>
    ArraySlice<T, abs_diff(Start, End) + 1, (End < Start), true> slice() const;

    /**
     * @brief   Get a read-only view on a slice of the Array.
     * @copydetails     slice()
     */
    template <size_t Start = 0, size_t End = N - 1>
    ArraySlice<T, abs_diff(Start, End) + 1, (End < Start), true>
    cslice() const {
        const Array<T, N> *This = this;
        return This->slice();
    }
};

/**
 * @brief   Class for a view on a slice of an array.
 * 
 * Doesn't copy the contents of the array, it's just a reference to the original
 * array.
 * 
 * @tparam  T
 *          The type of elements of the Array.
 * @tparam  N 
 *          The size of the slice.
 * @tparam  Reverse
 *          Whether the slice is reversed or not.
 * @tparam  Const
 *          Whether to save a read-only or mutable reference to the Array.
 */
template <class T, size_t N, bool Reverse = false, bool Const = true>
class ArraySlice {
    using ElementRefType =
        typename std::conditional<Const, const T &, T &>::type;
    using ElementPtrType =
        typename std::conditional<Const, const T *, T *>::type;

  public:
    /// Constructor
    ArraySlice(ElementPtrType array) : array{array} {}

    /// Implicit conversion from slice to new array (creates a copy).
    operator Array<T, N>() const { return asArray(); }

    Array<T, N> asArray() const {
        Array<T, N> slice = {{}};
        for (size_t i = 0; i < N; ++i)
            slice[i] = (*this)[i];
        return slice;
    }

    class Iterator {
      public:
        Iterator(ElementPtrType ptr) : ptr(ptr) {}

        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = ElementPtrType;
        using reference = ElementRefType;
        using iterator_category = std::random_access_iterator_tag;

        bool operator!=(Iterator rhs) const { return ptr != rhs.ptr; }
        bool operator==(Iterator rhs) const { return ptr == rhs.ptr; }

        reference operator*() const { return *ptr; }

        Iterator &operator++() {
            Reverse ? --ptr : ++ptr;
            return *this;
        }

        Iterator &operator--() {
            Reverse ? ++ptr : --ptr;
            return *this;
        }

        difference_type operator-(Iterator rhs) const {
            return Reverse ? rhs.ptr - ptr : ptr - rhs.ptr;
        }

        Iterator operator+(difference_type rhs) const {
            return Reverse ? ptr - rhs : ptr + rhs;
        }

        Iterator operator-(difference_type rhs) const {
            return Reverse ? ptr + rhs : ptr - rhs;
        }

        bool operator<(Iterator rhs) const {
            return Reverse ? rhs.ptr < ptr : ptr < rhs.ptr;
        }

      private:
        ElementPtrType ptr;
    };

    /**
     * @brief   Get the element at the given index.
     * 
     * @note    Bounds checking is performed. If fatal errors are disabled, the
     *          last element is returned if the index is out of bounds. 
     * 
     * @param   index
     *          The (zero-based) index of the element to return.
     */
    ElementRefType operator[](size_t index) const {
        if (index >= N) { // TODO
            ERROR(F("Index out of bounds: ") << index << F(" ≥ ") << N, 0xEDEF);
            index = N - 1; // LCOV_EXCL_LINE
        }                  // LCOV_EXCL_LINE
        if (Reverse)
            return array[-index];
        else
            return array[index];
    }

    Iterator begin() const {
        if (Reverse)
            return array;
        else
            return array;
    }

    Iterator end() const {
        if (Reverse)
            return array - N;
        else
            return array + N;
    }

    template <size_t Start, size_t End>
    ArraySlice<T, abs_diff(End, Start) + 1, Reverse ^ (End < Start), Const>
    slice() const;

  private:
    ElementPtrType array;
};

template <class T, size_t N>
template <size_t Start, size_t End>
inline ArraySlice<T, abs_diff(Start, End) + 1, (End < Start), false>
Array<T, N>::slice() {
    static_assert(Start < N, "");
    static_assert(End < N, "");
    return &(*this)[Start];
}

template <class T, size_t N>
template <size_t Start, size_t End>
inline ArraySlice<T, abs_diff(Start, End) + 1, (End < Start), true>
Array<T, N>::slice() const {
    static_assert(Start < N, "");
    static_assert(End < N, "");
    return &(*this)[Start];
}

template <class T, size_t N, bool Reverse, bool Const>
template <size_t Start, size_t End>
ArraySlice<T, abs_diff(End, Start) + 1, Reverse ^ (End < Start), Const>
ArraySlice<T, N, Reverse, Const>::slice() const {
    static_assert(Start < N, "");
    static_assert(End < N, "");
    return &(*this)[Start];
}

// Equality ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/// Slice == Slice
template <class T1, class T2, size_t N1, size_t N2, bool Reverse1,
          bool Reverse2, bool Const1, bool Const2>
bool operator==(ArraySlice<T1, N1, Reverse1, Const1> a,
                ArraySlice<T2, N2, Reverse2, Const2> b) {
    static_assert(N1 == N2, "Error: sizes do not match");
    for (size_t i = 0; i < N1; ++i)
        if (a[i] != b[i])
            return false;
    return true;
}

/// Array == Slice
template <class T1, class T2, size_t N1, size_t N2, bool Reverse2, bool Const2>
bool operator==(const Array<T1, N1> &a,
                ArraySlice<T2, N2, Reverse2, Const2> b) {
    return a.slice() == b;
}

/// Slice == Array
template <class T1, class T2, size_t N1, size_t N2, bool Reverse1, bool Const1>
bool operator==(ArraySlice<T1, N1, Reverse1, Const1> a,
                const Array<T2, N2> &b) {
    return a == b.slice();
}

// Inequality ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/// Slice != Slice
template <class T1, class T2, size_t N1, size_t N2, bool Reverse1,
          bool Reverse2, bool Const1, bool Const2>
bool operator!=(ArraySlice<T1, N1, Reverse1, Const1> a,
                ArraySlice<T2, N2, Reverse2, Const2> b) {
    return !(a == b);
}

/// Array != Slice
template <class T1, class T2, size_t N1, size_t N2, bool Reverse2, bool Const2>
bool operator!=(const Array<T1, N1> &a,
                ArraySlice<T2, N2, Reverse2, Const2> b) {
    return a.slice() != b;
}

/// Slice != Array
template <class T1, class T2, size_t N1, size_t N2, bool Reverse1, bool Const1>
bool operator!=(ArraySlice<T1, N1, Reverse1, Const1> a,
                const Array<T2, N2> &b) {
    return a != b.slice();
}

// Addition ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/// Slice + Slice
template <class T1, class T2, size_t N1, size_t N2, bool Reverse1,
          bool Reverse2, bool Const1, bool Const2>
Array<decltype(T1{} + T2{}), N1>
operator+(ArraySlice<T1, N1, Reverse1, Const1> a,
          ArraySlice<T2, N2, Reverse2, Const2> b) {
    static_assert(N1 == N2, "Error: sizes do not match");
    Array<decltype(T1{} + T2{}), N1> result = {{}};
    for (size_t i = 0; i < N1; ++i)
        result[i] = a[i] + b[i];
    return result;
}

/// Array + Array
template <class T1, class T2, size_t N1, size_t N2>
Array<decltype(T1{} + T2{}), N1> operator+(const Array<T1, N1> &a,
                                           const Array<T2, N2> &b) {
    return a.slice() + b.slice();
}

/// Slice += Slice
template <class T1, class T2, size_t N1, size_t N2, bool Reverse1,
          bool Reverse2, bool Const1, bool Const2>
const ArraySlice<T1, N1, Reverse1, Const1> &
operator+=(const ArraySlice<T1, N1, Reverse1, Const1> &a,
           const ArraySlice<T2, N2, Reverse2, Const2> &b) {
    static_assert(N1 == N2, "Error: sizes do not match");
    for (size_t i = 0; i < N1; ++i)
        a[i] += b[i];
    return a;
}

/// Array += Array
template <class T1, class T2, size_t N1, size_t N2>
Array<T1, N1> &operator+=(Array<T1, N1> &a, const Array<T2, N2> &b) {
    a.slice() += b.slice();
    return a;
}

// Subtraction :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/// Slice - Slice
template <class T1, class T2, size_t N1, size_t N2, bool Reverse1,
          bool Reverse2, bool Const1, bool Const2>
Array<decltype(T1{} - T2{}), N1>
operator-(ArraySlice<T1, N1, Reverse1, Const1> a,
          ArraySlice<T2, N2, Reverse2, Const2> b) {
    static_assert(N1 == N2, "Error: sizes do not match");
    Array<decltype(T1{} - T2{}), N1> result = {{}};
    for (size_t i = 0; i < N1; ++i)
        result[i] = a[i] - b[i];
    return result;
}

/// Array - Array
template <class T1, class T2, size_t N1, size_t N2>
Array<decltype(T1{} - T2{}), N1> operator-(const Array<T1, N1> &a,
                                           const Array<T2, N2> &b) {
    return a.slice() - b.slice();
}

/// Slice -= Slice
template <class T1, class T2, size_t N1, size_t N2, bool Reverse1,
          bool Reverse2, bool Const1, bool Const2>
const ArraySlice<T1, N1, Reverse1, Const1> &
operator-=(const ArraySlice<T1, N1, Reverse1, Const1> &a,
           const ArraySlice<T2, N2, Reverse2, Const2> &b) {
    static_assert(N1 == N2, "Error: sizes do not match");
    for (size_t i = 0; i < N1; ++i)
        a[i] -= b[i];
    return a;
}

/// Array -= Array
template <class T1, class T2, size_t N1, size_t N2>
Array<T1, N1> &operator-=(Array<T1, N1> &a, const Array<T2, N2> &b) {
    a.slice() -= b.slice();
    return a;
}

// Scalar Multiplication :::::::::::::::::::::::::::::::::::::::::::::::::::::::

/// Slice * Scalar
template <class T1, class T2, size_t N1, bool Reverse1, bool Const1>
Array<decltype(T1{} * T2{}), N1>
operator*(ArraySlice<T1, N1, Reverse1, Const1> a, T2 b) {
    Array<decltype(T1{} * T2{}), N1> result = {{}};
    for (size_t i = 0; i < N1; ++i)
        result[i] = a[i] * b;
    return result;
}

/// Array * Scalar
template <class T1, class T2, size_t N1>
Array<decltype(T1{} * T2{}), N1> operator*(const Array<T1, N1> &a, T2 b) {
    return a.slice() * b;
}

/// Scalar * Slice
template <class T1, class T2, size_t N2, bool Reverse2, bool Const2>
Array<decltype(T1{} * T2{}), N2>
operator*(T1 a, ArraySlice<T2, N2, Reverse2, Const2> b) {
    Array<decltype(T1{} * T2{}), N2> result = {{}};
    for (size_t i = 0; i < N2; ++i)
        result[i] = a * b[i];
    return result;
}

/// Scalar * Array
template <class T1, class T2, size_t N2>
Array<decltype(T1{} * T2{}), N2> operator*(T1 a, const Array<T2, N2> &b) {
    return a * b.slice();
}

/// Slice *= Scalar
template <class T1, class T2, size_t N1, bool Reverse1, bool Const1>
const ArraySlice<T1, N1, Reverse1, Const1> &
operator*=(const ArraySlice<T1, N1, Reverse1, Const1> &a, T2 b) {
    for (size_t i = 0; i < N1; ++i)
        a[i] *= b;
    return a;
}

/// Array *= Scalar
template <class T1, class T2, size_t N1>
Array<T1, N1> &operator*=(Array<T1, N1> &a, T2 b) {
    a.slice() *= b;
    return a;
}

// Scalar Division :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/// Slice / Scalar
template <class T1, class T2, size_t N1, bool Reverse1, bool Const1>
Array<decltype(T1{} / T2{}), N1>
operator/(ArraySlice<T1, N1, Reverse1, Const1> a, T2 b) {
    Array<decltype(T1{} / T2{}), N1> result = {{}};
    for (size_t i = 0; i < N1; ++i)
        result[i] = a[i] / b;
    return result;
}

/// Array / Scalar
template <class T1, class T2, size_t N1>
Array<decltype(T1{} / T2{}), N1> operator/(const Array<T1, N1> &a, T2 b) {
    return a.slice() / b;
}

/// Slice /= Scalar
template <class T1, class T2, size_t N1, bool Reverse1, bool Const1>
const ArraySlice<T1, N1, Reverse1, Const1> &
operator/=(const ArraySlice<T1, N1, Reverse1, Const1> &a, T2 b) {
    for (size_t i = 0; i < N1; ++i)
        a[i] /= b;
    return a;
}

/// Array /= Scalar
template <class T1, class T2, size_t N1>
Array<T1, N1> &operator/=(Array<T1, N1> &a, T2 b) {
    a.slice() /= b;
    return a;
}

// Negation ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/// -Slice
template <class T, size_t N, bool Reverse, bool Const>
Array<decltype(-T{}), N> operator-(ArraySlice<T, N, Reverse, Const> a) {
    Array<decltype(-T{}), N> result = {{}};
    for (size_t i = 0; i < N; ++i)
        result[i] = -a[i];
    return result;
}

/// -Array
template <class T, size_t N>
Array<decltype(-T{}), N> operator-(const Array<T, N> &a) {
    return -a.slice();
}

// Type aliases ::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

/// An easy alias for two-dimensional Arrays.
template <class T, size_t nb_rows, size_t nb_cols>
using Array2D = Array<Array<T, nb_cols>, nb_rows>;

/// @}

END_AH_NAMESPACE

AH_DIAGNOSTIC_POP()