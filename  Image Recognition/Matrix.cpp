// ------------------------------ includes ------------------------------
#include "Matrix.h"
#include "math.h"


// ------------------------------ functions -----------------------------

/**
 * Check, if the two matrix oh, equal sizes
 * @param a -  matrix
 * @param b -  matrix
 * @return boolean:
 *          true - equal
 *          false - Not
 */
bool equal_size (const Matrix &a, const Matrix &b)
{
  // Check size
  if (a.get_rows () != b.get_rows () || a.get_cols () != b.get_cols ())
  {
    return false;
  }
  return true;
}

bool valid_mult_size (const Matrix &a, const Matrix &b)
{
  // Check if the number of columns in matrix1(this) is equal to the number of
  // rows in matrix2(other)
  if (a.get_cols () != b.get_rows ())
  {
    return false;
  }
  return true;

}


// ------------------------------ Class -----------------------------

// Parameterized Constructor
Matrix::Matrix (int n, int m) : _rows (n), _cols (m)
{
  // check index are valid
  if (n <= 0 || m <= 0)
  {
    throw std::length_error (MSG_INVALID_INDEX_ARGS);
  }

  // inits all elements to 0
  _mat = new float[_rows * _cols] ();

}

// Default Constructor
Matrix::Matrix () : Matrix (DEFAULT_ROW, DEFAULT_COLS)
{}

// Copy Constructor
Matrix::Matrix (const Matrix &other) : _rows (other._rows), _cols (other
                                                                       ._cols)
{

  // init a a array
  _mat = new float[_rows * _cols];

  // copy array
  for (int i = 0; i < _rows * _cols; i++)
  {
    _mat[i] = other[i];
  }
}

// Destructor
Matrix::~Matrix ()
{
  // free matrix
  delete[] _mat;

}

// -------------- getter and setter ----------------

int Matrix::get_rows () const
{
  return _rows;
}

int Matrix::get_cols () const
{
  return _cols;
}

// ------------- Methods & Functions --------------

Matrix &Matrix::transpose ()
{
  // start temporary matrix
  Matrix temp (_cols, _rows);

  // Running on all matrix inputs
  for (int j = 0; j < _cols; j++)
  {
    for (int i = 0; i < _rows; i++)
    {
      // Running transpose
      temp (j, i) = (*this) (i, j);
    }
  }

  // save this as transpose
  *this = temp;
  return *this;
}

Matrix &Matrix::vectorize ()
{
  // change _row index
  _rows = _rows * _cols;

  // change _cols index
  this->_cols = 1;
  return (*this);
}

void Matrix::plain_print () const
{
  // Running on all matrix inputs
  for (int i = 0; i < _rows; i++)
  {
    for (int j = 0; j < _cols; j++)
    {
      // saving the matrix input value
      float elem = (*this) (i, j);

      // Print space
      std::cout << elem << SINGLE_SPACE;
    }
    // Print new line
    std::cout << std::endl;
  }

}

Matrix Matrix::dot (const Matrix &other) const
{

  // Check if matrix sizes are compatible for dot product
  if (!equal_size ((*this), other))
  {
    throw std::length_error ("Invalid matrix sizes for dot product.");
  }

  // Create the result matrix
  Matrix result (other.get_rows (), other.get_cols ());

  // Iterate over the rows and columns of the matrix
  for (int i = 0; i < _rows; i++)
  {
    for (int j = 0; j < other._cols; j++)
    {
      result (i, j) = (*this) (i, j) * other (i, j);
    }

  }

  return result;
}

float Matrix::norm () const
{

  // start sum
  float sum = 0.0;

  // Iterate over the rows and columns of the matrix
  for (int i = 0; i < (*this)._rows; i++)
  {
    for (int j = 0; j < (*this)._cols; j++)
    {
      sum += ((*this) (i, j)) * ((*this) (i, j));
    }
  }
  // Take the square root of the sum (pos because pow)
  return std::sqrt (sum);

}
Matrix Matrix::rref() const
{
  Matrix new_matrix(*this); // copy the matrix
  // RREF Algorithm
  int lead = 0;
  for (int r = 0; r < new_matrix._rows; ++r)
  {
    if (lead >= new_matrix._cols)
    {
      return new_matrix;
    }
    int i = r;
    while (new_matrix(i, lead) == 0)
    {
      i = i + 1;
      if (i == new_matrix._rows)
      {
        i = r;
        lead = lead + 1;
        if (lead == new_matrix._cols)
        {
          return new_matrix;
        }
      }
    }// Swap rows i and r
    for (int j = 0; j < new_matrix._cols; ++j)
    {
      float tmp = new_matrix(i, j);
      new_matrix(i, j) = new_matrix(r, j);
      new_matrix(r, j) = tmp;
    }
    float val = new_matrix(r, lead);
    // div all row
    for (int j = 0; j < new_matrix._cols; ++j)
    {
      new_matrix(r, j) = new_matrix(r, j) / val;
    }// neg all cols
    for (int i = 0; i < new_matrix._rows; ++i)
    {
      if (i != r)
      {val = new_matrix(i, lead);
        for (int j = 0; j < new_matrix._cols; ++j)
        {new_matrix(i, j) = new_matrix(i, j) - (val * new_matrix(r, j));}
      }
    }
    lead = lead + 1;}
  return new_matrix;}

unsigned int Matrix::argmax () const
{
  // starting num
  unsigned int index = 0;
  float num = (*this)[0];
  float max = num;

  // Running on all matrix inputs
  for (int i = 1; i < _rows * _cols; i++)
  {
    num = (*this)[i];

    // Checking max
    if (num > max)
    {
      max = num;
      index = i;
    }
  }
  return index;

}

float Matrix::sum () const
{
  float mat_sum = 0;

  // Running on all matrix inputs
  for (int i = 0; i < _rows * _cols; i++)
  {
    // Sum all element
    mat_sum += (*this)[i];

  }
  return mat_sum;
}

// ----------------- Operators -------------------

Matrix Matrix::operator+ (const Matrix &other) const
{
  // check if valid size
  if (!equal_size ((*this), other))
  {
    throw std::length_error (MSG_INVALID_MATRIX_SIZES);
  }
  // Make a copy of matrix
  Matrix temp (*this);

  // use += operator to add matrix b
  temp += other;

  return temp;
}

Matrix &Matrix::operator= (const Matrix &other)
{
  // check if A = A
  if (this == &other)
  {
    return *this;
  }

  // Save this new data
  _rows = other._rows;
  _cols = other._cols;

  // Allocate new matrix
  float *temp_mat = new float[_rows * _cols];

  // copy array
  for (int i = 0; i < _rows * _cols; i++)
  {
    temp_mat[i] = other[i];
  }

  // Delete Matrix
  delete[] this->_mat;

  _mat = temp_mat;

  return *this;
}

Matrix Matrix::operator* (const Matrix &other) const
{
  // Check valid size
  if (!valid_mult_size ((*this), other))
  {
    throw std::length_error (MSG_INVALID_MATRIX_SIZES);
  }

  // declare a temp function
  Matrix temp (this->_rows, other.get_cols ());

  // declare sum
  float sum = 0;

  // run on A rows
  for (int i = 0; i < (*this)._rows; i++)
  {
    // run in B cols
    for (int j = 0; j < other._cols; j++)
    {
      for (int k = 0; k < _cols; k++)
      {
        sum += ((*this) (i, k) * other (k, j));
      }
      // change value element by element
      temp (i, j) = (sum);
      sum = 0;
    }

  }
  return temp;
}

Matrix Matrix::operator* (float c) const
{
  // make a copy of matrix
  Matrix temp (*this);

  // apply mult with scale
  for (int i = 0; i < _rows * _cols; i++)
  {
    temp[i] *= c;
  }
  return temp;

}

Matrix operator* (const float c, const Matrix &A)
{
  // apply operator
  return A * c;
}

const float &Matrix::operator() (int i, int j) const
{
  // check if index in range
  if (i < 0 || i > _rows - 1 || j < 0 || j > _cols - 1)
  {
    throw std::out_of_range (MSG_OUT_OF_RANGE);
  }
  return _mat[i * _cols + j];

}

float &Matrix::operator() (int i, int j)
{
  // check if index in range
  if (i < 0 || i > _rows - 1 || j < 0 || j > _cols - 1)
  {
    throw std::out_of_range (MSG_OUT_OF_RANGE);
  }
  return _mat[i * _cols + j];

}

const float &Matrix::operator[] (int k) const
{
  // check index are valid
  if (k < 0 || k > _rows * _cols)
  {
    throw std::out_of_range (MSG_OUT_OF_RANGE);
  }

  return _mat[k];
}

float &Matrix::operator[] (int k)
{
  // check index are valid
  if (k >= 0 && k < _rows * _cols)
  {
    return _mat[k];

  }
  throw std::out_of_range (MSG_OUT_OF_RANGE);
}

Matrix& Matrix::operator+= (const Matrix &other)
{
  // check valid size
  if (!equal_size ((*this), other))
  {
    throw std::length_error (MSG_INVALID_MATRIX_SIZES);
  }

  // Running on all matrix inputs
  for (int i = 0; i < _rows * _cols; i++)
  {
    // adding to all matrix inputs
    (*this)[i] += other[i];
  }
  return *this;

}

std::istream& operator>> (std::istream &is, Matrix &matrix)
{
  // moves current position - seeks to the end of the input stream
  is.seekg (0, std::istream::end);

  // saves the current position in the input stream, which corresponds to the
  // length of the stream.
  int len = (int) is.tellg ();

  // moves current position - seeks back to the beginning of the input stream
  is.seekg (0, std::istream::beg);

  // Check size of stream
  if (len < matrix.get_cols () * matrix.get_rows ())
  {
    // Error: Failed to read element from the input stream
    throw std::runtime_error (MSG_READ_INPUT_FAIL);
  }

  is.read ((char *) matrix._mat,
           (matrix._rows * matrix._cols) * sizeof (float));

  if (is.fail ())
  {
    // Error: Failed to read element from the input stream
    throw std::runtime_error (MSG_READ_INPUT_FAIL);
  }
  return is;
}

std::ostream &operator<< (std::ostream &os, const Matrix &matrix)
{
  // Running on all matrix inputs
  for (int i = 0; i < matrix._rows; i++)
  {
    for (int j = 0; j < matrix._cols; j++)
    {
      // saving the matrix input value
      //float &elem = matrix (i, j);

      // check if mat > LOWER_BOUND
      if (matrix (i, j) > LOWER_BOUND)
      {
        std::cout << STARS;
      }
      else
      {
        std::cout << DOUBLE_SPACE;
      }
    }
    // Print new line
    std::cout << std::endl;
  }
  return os;
}





