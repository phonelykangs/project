#ifndef __2D_ARRAY_H__
#define __2D_ARRAY_H__

#include<stdlib.h>

template<typename T = char>
class Array2D{
public:
    Array2D(u_int32_t rows, u_int32_t columns)
    : m_rows(rows)
    , m_columns(columns){
        T* m_data = new T[m_rows * m_columns];
    }
    ~Array2D() {delete[] m_data;}
    inline u_int32_t rows() const { return m_rows; }
    inline u_int32_t columns() const {  return m_columns; }
    inline const T& operator()(u_int32_t rows,u_int32_t columns) const { return m_data[rows * m_rows + columns]; }
    inline T& operator()(u_int32_t rows,u_int32_t columns) { return m_data[rows * m_rows + columns]; }
    inline T* operator[](u_int32_t rows) const { return &(m_data[rows * m_rows]); }

    void clear(const T& value){
        for(u_int32_t i = 0; i < m_rows * m_columns;++i){
            m_data[i] = value;
        }
    }
private:
    Array2D(const Array2D& );
    Array2D& operator=(const Array2D& );
private:
    u_int32_t m_rows;
    u_int32_t m_columns;
    T* m_data;
};

#endif