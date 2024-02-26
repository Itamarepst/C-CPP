// ------------------------------ includes ------------------------------
#include "Activation.h"
#include <cmath>


// ------------------------------ functions -----------------------------
/**
 * Apply exp for each matrix element
 * @param matrix  matrix,
 * @return float sum -  the sum of the matrix, exp
 */
float exp_mat (Matrix &matrix)
{
  // Save rows and cols
  int row = matrix.get_rows ();
  int cols = matrix.get_cols ();
  float sum =0.0;

  // Iterate over the rows and columns and Compute the exponential
  // exp(matrix(i, j)
  for (int i = 0; i < row; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      // Add element to sum
      matrix (i, j) = std::exp (matrix (i, j));
      sum +=matrix(i,j);
    }
  }
  return sum;
}

/**
 * divedes each matrix element with sum
 * @param matrix  matrix,
 */
void div_mat (Matrix &matrix ,float sum )
{
  // Save rows and cols
  int row = matrix.get_rows ();
  int cols = matrix.get_cols ();

  // Divide each element by the total sum of its row
  for (int j = 0; j < cols; j++)
  {
    // sum each row
    //float row_sum = 0.0;

    for (int i = 0; i < row; i++)
    {

      // Calculate the sum of the row
      matrix(i,j)= matrix(i, j) / sum;
    }

    // Divide each element by the row sum

  }
}

Matrix activation::relu (const Matrix &matrix)
{
  // copy matrix
  Matrix new_mat (matrix);

  // Save rows and cols
  int row = matrix.get_rows ();
  int cols = matrix.get_cols ();

  // Iterate over the rows and columns of the matrix implement relu
  for (int i = 0; i < row; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      // check if element in (i , j) negative
      if (new_mat (i, j) < 0)
      {
        // Change value to zero
        new_mat (i, j) = 0;
      }
    }
  }
  return new_mat;
}

Matrix activation::softmax (const Matrix &matrix)
{
  // copy matrix
  Matrix new_mat (matrix);
  float sum =0.0;

  // Compute the exponential (e^x) of each element in the row.
  sum = exp_mat (new_mat);

  // Divide the matrix
  div_mat (new_mat , sum );

  return new_mat;
}



