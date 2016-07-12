// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2014 projectchrono.org
// All right reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================
// Authors: Radu Serban
// =============================================================================

#include <algorithm>

#include "chrono/core/ChMapMatrix.h"

namespace chrono {

ChMapMatrix::ChMapMatrix(int nrows = 1, int ncols = 1) : ChSparseMatrix(nrows, ncols), m_nnz(0) {
    m_rows.resize(nrows);
}

ChMapMatrix::ChMapMatrix(const ChMatrix<>& mat) {
    m_num_rows = mat.GetRows();
    m_num_cols = mat.GetColumns();
    m_rows.resize(mat.GetRows());
    for (int ir = 0; ir < m_num_rows; ir++) {
        for (int ic = 0; ic < m_num_cols; ic++) {
            double val = mat.GetElement(ir, ic);
            if (val != 0) {
                SetElement(ir, ic, val);
            }
        }
    }
}

ChMapMatrix::ChMapMatrix(const ChMapMatrix& other) {
    m_num_rows = other.m_num_rows;
    m_num_cols = other.m_num_cols;
    m_nnz = other.m_nnz;
    m_rows = other.m_rows;
}

ChMapMatrix::~ChMapMatrix() {}

bool ChMapMatrix::Resize(int nrows, int ncols, int nonzeros) {
    Reset(nrows, ncols, nonzeros);
    return true;
}

void ChMapMatrix::Reset(int nrows, int ncols, int nonzeros) {
    for (int ir = 0; ir < std::min(nrows, m_num_rows); ir++) {
        m_rows[ir].m_data.clear();
        m_rows[ir].m_nnz = 0;
    }
    m_rows.resize(nrows);
    m_num_rows = nrows;
    m_num_cols = ncols;
    m_nnz = 0;
}

void ChMapMatrix::SetElement(int row, int col, double elem, bool overwrite) {
    // Check if an element at this column already exists
    auto my_elem = m_rows[row].m_data.find(col);
    if (my_elem == m_rows[row].m_data.end()) {
        m_rows[row].m_data.insert(std::make_pair(col, elem));
        m_rows[row].m_nnz++;
        m_nnz++;
    } else {
        if (overwrite)
            my_elem->second = elem;
        else
            my_elem->second += elem;
    }
}

double ChMapMatrix::GetElement(int row, int col) {
    assert(row < rows);
    assert(col < columns);

    // Find the element in the row data
    auto my_elem = m_rows[row].m_data.find(col);
    if (my_elem == m_rows[row].m_data.end())
        return 0;

    // Return the value.
    return my_elem->second;
}

void ChMapMatrix::ConvertToDense(ChMatrixDynamic<double>& mat) {
    mat.Reset(m_num_rows, m_num_cols);
    for (int ir = 0; ir < m_num_rows; ir++) {
        for (auto it : m_rows[ir].m_data) {
            mat.SetElement(ir, it.first, it.second);
        }
    }
}

void ChMapMatrix::ConvertToCSR(std::vector<int>& ia, std::vector<int>& ja, std::vector<double>& a) {
    ia.resize(m_num_rows + 1);
    ja.resize(m_nnz);
    a.resize(m_nnz);

    ia[0] = 0;
    int nnz = 0;
    for (int ir = 0; ir < m_num_rows; ir++) {
        ia[ir + 1] = ia[ir] + m_rows[ir].m_nnz;

        std::vector<int> col_idx;
        col_idx.reserve(m_rows[ir].m_nnz);
        for (auto& it : m_rows[ir].m_data) {
            col_idx.push_back(it.first);
        }
        std::sort(col_idx.begin(), col_idx.end());
        for (auto ic : col_idx) {
            ja[nnz] = ic;
            a[nnz] = m_rows[ir].m_data[ic];
            nnz++;
        }
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

void ChMapMatrix::StreamOUTsparseMatlabFormat(ChStreamOutAscii& mstream) {
    for (int ii = 0; ii < m_num_rows; ii++) {
        for (int jj = 0; jj < m_num_cols; jj++) {
            double elVal = GetElement(ii, jj);
            if (elVal || (ii + 1 == m_num_rows && jj + 1 == m_num_cols)) {
                mstream << ii + 1 << " " << jj + 1 << " " << elVal << "\n";
            }
        }
    }
}

void ChMapMatrix::StreamOUT(ChStreamOutAscii& mstream) {
    mstream << "\nMatrix  nrows=" << m_num_rows << " ncols=" << m_num_cols << " nnz=" << m_nnz << "\n";
    for (int i = 0; i < ChMin(m_num_rows, 8); i++) {
        for (int j = 0; j < ChMin(m_num_cols, 8); j++)
            mstream << GetElement(i, j) << "  ";
        if (m_num_cols > 8)
            mstream << "...";
        mstream << "\n";
    }
    if (m_num_rows > 8)
        mstream << "... \n\n";
}

}  // end namespace chrono
