#ifndef TWODARRAY_H_
#define TWODARRAY_H_

#include <qdebug.h>
/**
 * Template class for a 2 dimensional array.  Supports the [][] functionality.
 * Uses a continous array of objects.
 * Example use:
 * 
 * TwoDArray array(10,10);
 * array[0][5] = 10;
 * ...
 * for(size_t rows = 0; rows <= array.rows();rows++){
 *	for(size_t cols = 0; cols <= array.cols();cols++){
 *		std::cout << array[rows][cols] << " ";
 *	}
 *   std::cout << endl;
 * }	
 */
template < class T> class TwoDArray {
public:
    TwoDArray() :
            m_row(0),m_col(0),m_data(0) {
        qDebug() << "default ctor";
    }
    TwoDArray(size_t row, size_t col) :
            m_row(row), m_col(col), m_data((row!=0&&col!=0) ? new T[row*col] : 0) {
    }
    TwoDArray(const TwoDArray& src) :
            m_row(src.m_row), m_col(src.m_col), m_data((src.m_row!=0&&src.m_col!=0) ? new T[src.m_row*src.m_col] : 0) {
        for (size_t r=0; r<m_row; ++r)
            for (size_t c=0; c<m_col; ++c)
                (*this)[r][c] = src[r][c];

    }
    ~TwoDArray() {
        delete [] m_data;
        m_data = 0;
    }
    /**
      * equality operator
      * Deletes old data and creates new storage. Then assignes all data from other.
      * \param other TwoDArray to copy from
      */
    TwoDArray& operator =(const TwoDArray &other) {
        if (m_data)
            delete[] m_data;
        m_row = other.m_row;
        m_col = other.m_col;
        m_data = (other.m_row!=0 && other.m_col!=0) ? new T[other.m_row*other.m_col] : 0;
        for (size_t r=0; r<m_row; ++r)
            for (size_t c=0; c<m_col; ++c)
                (*this)[r][c] = other[r][c];
        return *this;
    }

    /**
      * swap operator. move semantics.
      * Simply takes ownership of other vector
      * \param other TwoDArray to swap with
      */
    void swap(TwoDArray& other){
        size_t tmp_row = m_row;
        size_t tmp_col = m_col;
        m_row = other.m_row;
        m_col = other.m_col;
        other.m_row = tmp_row;
        other.m_col = tmp_col;
        T* tmp_data = m_data;
        m_data = other.m_data;
        other.m_data = tmp_data;

    }

    /**
      * horizontally concatenates the arrays;
      */
    void horzCat(const TwoDArray &other) {
        if (m_row != other.m_row){
            throw 1;
            //abort();//TODO: throw exception
        }
        TwoDArray<double> tmp(m_row, m_col+other.m_col);
        for (size_t i = 0; i < m_row; i++) {
            for (size_t j = 0; j < m_col; j++)
                tmp[i][j] = (*this)[i][j];
            for (size_t c2 = 0; c2 < other.m_col; c2++)
                tmp[i][m_col+c2] = other[i][c2];
        }
        (*this) = tmp;
    }
    /**
         * Inverses the two dimensional array
         */
    void inverse() {
        TwoDArray tmp(m_col, m_row);
        for (size_t r=0; r<m_row; ++r)
            for (size_t c=0; c<m_col; ++c)
                tmp[c][r] = (*this)[r][c];
        m_col = tmp.m_col;
        m_row =tmp.m_row;
        for (size_t r=0; r<m_row; ++r)
            for (size_t c=0; c<m_col; ++c)
                (*this)[r][c] = tmp[r][c];

    }
    inline T* operator[](size_t i) {
        return (m_data + (m_col*i));
    }
    inline T const*const operator[](size_t i) const {
        return (m_data + (m_col*i));
    }
    size_t rows() {
        return m_row;
    }
    size_t cols() {
        return m_col;
    }
    /**
      * Resizes the matrix. No values are kept
      */
    inline void resize(size_t rows, size_t cols) {
        if(m_data)
            delete[] m_data;
        m_row = rows;
        m_col = cols;
        m_data = ((rows!=0&&cols!=0) ? new T[rows*cols] : 0);
    }
private:
    size_t m_row;
    size_t m_col;
    T* m_data;
};

template <class T> void printTwoDArray(TwoDArray<T>* a) {
    for (size_t r = 0; r < a->rows(); r++) {
        QString s;
        for (size_t c = 0; c < a->cols(); c++) {
            T value = (*a)[r][c];
            QString col = QString("[%1][%2]=%3   ").arg(r).arg(c).arg(value);
            s.append(col);
        }
        qDebug() << s;
    }
}
#endif /*TWODARRAY_H_*/
