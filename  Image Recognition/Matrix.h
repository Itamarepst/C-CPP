// Matrix.h
#ifndef MATRIX_H
#define MATRIX_H


// ------------------------------ includes ------------------------------
#include <iostream>

// ------------------------------ const & macros -----------------------------

#define DEFAULT_ROW 1
#define DEFAULT_COLS 1
#define LOWER_BOUND 0.1
#define STARS "**"
#define DOUBLE_SPACE "  "
#define SINGLE_SPACE " "

# define MSG_OUT_OF_RANGE "List out of range"
# define MSG_INVALID_INDEX_ARGS "Invalid argument: incorrect size."
# define MSG_INVALID_MATRIX_SIZES "Invalid matrix sizes for math operation"
# define MSG_READ_INPUT_FAIL "Error: Failed to read matrix element."



// ------------------------------ struct -----------------------------

/**
 * @struct matrix_dims
 * @brief Matrix dimensions container. Used in MlpNetwork.h and main.cpp
 */
typedef struct matrix_dims
{
    int rows, cols;
} matrix_dims;

// Insert Matrix class here...

// ------------------------------ Class -----------------------------
class Matrix
{
 private:
  // The amount rows of the Matrix
  int _rows;

  // The amount columns of the Matrix
  int _cols;

  // The matrix
  float *_mat;


  // -------------- Function ----------------
  //bool equal_size(const Matrix& a, const Matrix&b);

  //bool valid_mult_size(const Matrix& a, const Matrix&b);


 public:

  // ----------------- Constructors -------------------
  /**
 * Constructor:
 * Starts Matrix of size n × m.
 * Inits the single element to 0.
 */
  Matrix (int n, int m);

  /**
   * Default Constructor:
   * Starts Matrix of size 1 × 1.
   * Inits the single element to 0.
   */
  Matrix ();

  /**
* Copy Constructor:
* Constructs a matrix from another Matrix m.
*/
  Matrix (const Matrix &other);

  // Destructor
  ~Matrix ();

  // -------------- getter and setter ----------------
  /**
 * get_rows
 * return returns the amount of rows as int.
 */
  int get_rows () const;

  /**
* get_cols
* return returns the amount of cols as int.
*/
  int get_cols () const;

  // ------------- Methods & Functions --------------
  /**
* transpose
* Turn the members matrix in its transpose matrix
* return member for chaining
*/
  Matrix &transpose ();

  /**
* vectorize:
* Transforms Amatrix into Acolumn vector
* return member for chaining
*/
  Matrix &vectorize ();

  /**
 * Prints matrix elements:
 * Prints space after each element
 * Prints newline after each row
 */
  void plain_print () const;

  /**
*@name dot
* @param Matrix - A Matrix object
* @return Amatrix which is the element-wise multiplication
*/
  Matrix dot (const Matrix &other) const;

  /**
* @name norm
* @param this - the Matrix object
* @return Returns the Frobenius norm of the given matrix:
*/
  float norm () const;

  /**
* @name rref
* @param this - the Matrix object
* @return Returns A new Matrix that is the reduced
 * row echelon form of the original.
*/
  Matrix rref () const;

  /**
* @name argmax
* @param this - the Matrix object
* @return Returns index of the largest number in the matrix.
*/
  unsigned int argmax () const;

  /**
* @name sum
* @param this - the Matrix object
* @return Returns the sum of all elements in matrix
*/
  float sum () const;


  // ----------------- Operators -------------------

/**
     * @overload
     * Matrix A+ B;
     * @param other
     * @return Matrix
     */
  Matrix operator+ (const Matrix &other) const;

  /**
     * @overload
     * Matrix A= B;
     * @param other
     * @return Matrix
     */
  Matrix &operator= (const Matrix &other);

  /**
     * @overload
     * Matrix A* B;
     * @param other
     * @return Matrix
     */
  Matrix operator* (const Matrix &other) const;

  /**
     * @overload
     * Matrix A* c;
     * @param c scale
     * @return this Matrix
     */
  Matrix operator* (float c) const;

  /**
   * @overload
   * Matrix  c * A;
   * @param c scale
   * @return this Matrix
   */
  friend Matrix operator* (float c, const Matrix &A);

  /**
     * @overload
     * Matrix A(i , j);
     * @param i - the row index
     * @param j - the cols index
     * @return return the element in cell (i,j)
     */
  float &operator() (int i, int j);

  const float &operator() (int i, int j) const;

  /**
   * @overload
   * Matrix A[k];
   * @param k - the k element index
   * @return will return the k'th element as if m is represented as a vector
   */
  float &operator[] (int k);

  const float &operator[] (int k) const;

  /**
     * @overload
     * Matrix A+= B;
     * @param other
     * @return Matrix
     */
  Matrix &operator+= (const Matrix &other);

  friend std::istream &operator>> (std::istream &is, Matrix &matrix);

  friend std::ostream &operator<< (std::ostream &os, const Matrix &matrix);

};

#endif //MATRIX_H