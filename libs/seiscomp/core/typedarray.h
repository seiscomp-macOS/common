/***************************************************************************
 * Copyright (C) gempa GmbH                                                *
 * All rights reserved.                                                    *
 * Contact: gempa GmbH (seiscomp-dev@gempa.de)                             *
 *                                                                         *
 * GNU Affero General Public License Usage                                 *
 * This file may be used under the terms of the GNU Affero                 *
 * Public License version 3.0 as published by the Free Software Foundation *
 * and appearing in the file LICENSE included in the packaging of this     *
 * file. Please review the following information to ensure the GNU Affero  *
 * Public License version 3.0 requirements will be met:                    *
 * https://www.gnu.org/licenses/agpl-3.0.html.                             *
 *                                                                         *
 * Other Usage                                                             *
 * Alternatively, this file may be used in accordance with the terms and   *
 * conditions contained in a signed written agreement between you and      *
 * gempa GmbH.                                                             *
 ***************************************************************************/


#ifndef SC_CORE_TYPEDARRAY_H
#define SC_CORE_TYPEDARRAY_H

#include <vector>
#include <string>
#include <complex>
#include <cstdint>

#include <seiscomp/core/datetime.h>
#include <seiscomp/core/array.h>
#include <seiscomp/core/arrayfactory.h>


#ifdef WIN32
	#if defined(min)
		#undef min
	#endif
	#if defined(max)
		#undef max
	#endif
#endif


namespace Seiscomp {


/**
 * Class defining different types of arrays (such as CharArray, IntArray, FloatArray, DoubleArray).
 */
template<typename T>
class TypedArray : public Array {
	DECLARE_SC_CLASS(TypedArray);
	DECLARE_SERIALIZATION;

	public:
		typedef typename std::vector<T> DataArray;
		typedef typename DataArray::iterator iterator;
		typedef typename DataArray::const_iterator const_iterator;

		typedef T Type;
		static DataType ArrayType;

		//! Default Constructor
		TypedArray();

		//! Initializing Constructor
		TypedArray(int size);

		//! Initializing Constructor
		TypedArray(int size, const T* data);
		
		//! Copy Constructor
		TypedArray(const TypedArray &array);
		
		//! Destructor
		virtual ~TypedArray();
		
		//! Assignment operator
		TypedArray& operator=(const TypedArray &array);

		//! Index operator
		T operator[](int index) const;
		T& operator[](int index);
		
		//! Returns an array of the specified data type.
		Array* copy(DataType dt) const override;

		//! Sets the data from a typed memory chunk
		void setData(int size, const T* data);
		
		//! Returns the data address pointer.
		const void* data() const override;

		//! Returns the typed data pointer
		T* typedData();
		const T* typedData() const;
		
		//! Returns the size of the array.
		int size() const override;

		//! Returns the number of bytes of an array element.
		int elementSize() const override;

		//! Resizes the array
		void resize(int size) override;

		//! Drops all elements.
		void clear() override;

		//! Sets all values to v
		void fill(const T &v);

		//! Sets the element at a given index
		void set(int index, T v);

		//! Returns the element at the given index.
		T get(int index) const;

		//! Concatenates the given array to this array.
		void append(const Array *array) override;

		//! Concatenates the given array to this array.
		void append(int size, const T* data);

		//! Appends value n-times to this array.
		void append(int n, T value);

		//! Concatenates this array to the given array and store the result
		//! in this array.
		void prepend(const Array *array);

		//! Concatenates this array to the given array and store the result
		//! in this array.
		void prepend(int size, const T* data);

		//! Prepends value n-times to this array.
		void prepend(int n, T value);

		//! Returns the slice m...n-1 of the array
		TypedArray<T>* slice(int m, int n) const override;

		//! Returns an iterator to the first element of the array
		iterator begin();

		//! Returns an const_iterator to the first element of the array
		const_iterator begin() const;

		//! Returns an iterator just past the end of the array
		iterator end();

		//! Returns an const_iterator just past the end of the array
		const_iterator end() const;

		const DataArray &impl() const { return _data; }
		DataArray &impl() { return _data; }


	protected:
		DataArray _data;

	friend class ArrayFactory;
};


template<typename T>
class NumericArray : public TypedArray<T> {
	DECLARE_SC_CLASS(NumericArray);

	public:
		//! Default Constructor
		NumericArray();

		//! Initializing Constructor
		NumericArray(int size);

		//! Initializing Constructor
		NumericArray(int size, const T* data);
		
		//! Copy Constructor
		NumericArray(const NumericArray &array);
		
		//! Destructor
		virtual ~NumericArray();
		
		//! Assignment operator
		NumericArray& operator=(const NumericArray &array);

		//! Returns the maximum value of the array elements.
		T max() const;

		//! Returns the first local maximum value of the array elements.
		T firstMax() const;

		//! Returns the maximum of the absolute values of the array elements.
		T absMax() const;

		//! Returns the minimum value of the array elements.
		T min() const;

		//! Returns the median value of the array elements.
		T median() const;

		//! Returns the mean value of the array elements.
		T mean() const;

		//! Returns the rms of the data using an offset if given.
		T rms(T offset = 0) const;

		//! Returns the slice m...n-1 of the array
		NumericArray<T>* slice(int m, int n) const override;

		NumericArray<T> &operator+=(T v);
		NumericArray<T> &operator-=(T v);
		NumericArray<T> &operator*=(T v);
		NumericArray<T> &operator/=(T v);
};



typedef NumericArray<char> CharArray;
TYPEDEF_SMARTPOINTER(CharArray);
TYPEDEF_CONST_SMARTPOINTER(CharArray);

typedef NumericArray<std::int32_t> Int32Array;
TYPEDEF_SMARTPOINTER(Int32Array);
TYPEDEF_CONST_SMARTPOINTER(Int32Array);

typedef Int32Array IntArray;
typedef Int32ArrayPtr IntArrayPtr;
typedef Int32ArrayCPtr IntArrayCPtr;

typedef NumericArray<float> FloatArray;
TYPEDEF_SMARTPOINTER(FloatArray);
TYPEDEF_CONST_SMARTPOINTER(FloatArray);

typedef NumericArray<double> DoubleArray;
TYPEDEF_SMARTPOINTER(DoubleArray);
TYPEDEF_CONST_SMARTPOINTER(DoubleArray);

typedef TypedArray<Core::Time> DateTimeArray;
TYPEDEF_SMARTPOINTER(DateTimeArray);
TYPEDEF_CONST_SMARTPOINTER(DateTimeArray);

typedef TypedArray<std::string> StringArray;
TYPEDEF_SMARTPOINTER(StringArray);
TYPEDEF_CONST_SMARTPOINTER(StringArray);

typedef TypedArray< std::complex<float> > ComplexFloatArray;
TYPEDEF_SMARTPOINTER(ComplexFloatArray);
TYPEDEF_CONST_SMARTPOINTER(ComplexFloatArray);

typedef TypedArray< std::complex<double> > ComplexDoubleArray;
TYPEDEF_SMARTPOINTER(ComplexDoubleArray);
TYPEDEF_CONST_SMARTPOINTER(ComplexDoubleArray);


template<typename T>
inline T TypedArray<T>::operator[](int index) const {
	return _data[index];
}

template<typename T>
inline T& TypedArray<T>::operator[](int index) {
	return _data[index];
}

template<typename T>
inline int TypedArray<T>::size() const {
	return _data.size();
}

template<typename T>
inline void TypedArray<T>::set(int index, T v) {
	_data[index] = v;
}

template<typename T>
inline T TypedArray<T>::get(int index) const {
	return _data.at(index);
}

template<typename T>
inline typename TypedArray<T>::iterator TypedArray<T>::begin() {
	return _data.begin();
}

template<typename T>
inline typename TypedArray<T>::const_iterator TypedArray<T>::begin() const {
	return _data.begin();
}

template<typename T>
inline typename TypedArray<T>::iterator TypedArray<T>::end() {
	return _data.end();
}

template<typename T>
inline typename TypedArray<T>::const_iterator TypedArray<T>::end() const {
	return _data.end();
}


}


#endif
