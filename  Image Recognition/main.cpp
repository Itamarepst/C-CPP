#include <fstream>
#include "Matrix.h"
#include "Activation.h"
#include "Dense.h"
#include "MlpNetwork.h"
#include "iostream"
#define QUIT "q"
#define INSERT_IMAGE_PATH "Please insert image path:"
#define ERROR_INAVLID_PARAMETER "Error: invalid Parameters file for layer: "
#define ERROR_INVALID_INPUT "Error: Failed to retrieve input. Exiting.."
#define ERROR_INVALID_IMG "Error: invalid image path or size: "
#define USAGE_MSG "Usage:\n" \
                  "\t./mlpnetwork w1 w2 w3 w4 b1 b2 b3 b4\n" \
                  "\twi - the i'th layer's weights\n" \
                  "\tbi - the i'th layer's biases"
#define USAGE_ERR "Error: wrong number of arguments."
#define ARGS_START_IDX 1
#define ARGS_COUNT (ARGS_START_IDX + (MLP_SIZE * 2))
#define WEIGHTS_START_IDX ARGS_START_IDX
#define BIAS_START_IDX (ARGS_START_IDX + MLP_SIZE)

/**
 * Prints program usage to stdout.
 * @param argc number of arguments given in the program
 * @throw std::domain_error in case of wrong number of arguments
 */
void usage (int argc) noexcept (false)
{
  if (argc != ARGS_COUNT)
  {
    throw std::domain_error (USAGE_ERR);
  }
  std::cout << USAGE_MSG << std::endl;
}

/**
 * Given a binary file path and a matrix,
 * reads the content of the file into the matrix.
 * file must match matrix in size in order to read successfully.
 * @param filePath - path of the binary file to read
 * @param mat -  matrix to read the file into.
 * @return boolean status
 *          true - success
 *          false - failure
 */
bool readFileToMatrix (const std::string &filePath, Matrix &mat)
{
  std::ifstream is;
  is.open (filePath, std::ios::in | std::ios::binary | std::ios::ate);
  if (!is.is_open ())
  {
    return false;
  }

  long int matByteSize = (long int) mat.get_cols () * mat.get_rows () * sizeof
      (float);
  if (is.tellg () != matByteSize)
  {
    is.close ();
    return false;
  }

  is.seekg (0, std::ios_base::beg);
  is >> mat;
  is.close ();
  return true;
}

/**
 * Loads MLP parameters from weights & biases paths
 * to Weights[] and Biases[].
 * Throws an exception upon failures.
 * @param paths array of programs arguments, expected to be mlp parameters
 *        path.
 * @param weights array of matrix, weigths[i] is the i'th layer weights matrix
 * @param biases array of matrix, biases[i] is the i'th layer bias matrix
 *          (which is actually a vector)
 *  @throw std::invalid_argument in case of problem with a certain argument
 */
void loadParameters (char *paths[ARGS_COUNT], Matrix weights[MLP_SIZE],
                     Matrix biases[MLP_SIZE]) noexcept (false)
{
  for (int i = 0; i < MLP_SIZE; i++)
  {
    weights[i] = Matrix (weights_dims[i].rows, weights_dims[i].cols);
    biases[i] = Matrix (bias_dims[i].rows, bias_dims[i].cols);

    std::string weightsPath (paths[WEIGHTS_START_IDX + i]);
    std::string biasPath (paths[BIAS_START_IDX + i]);

    if (!(readFileToMatrix (weightsPath, weights[i]) &&
          readFileToMatrix (biasPath, biases[i])))
    {
      auto msg = ERROR_INAVLID_PARAMETER + std::to_string (i + 1);
      throw std::invalid_argument (msg);
    }

  }
}

/**
 * This programs Command line interface for the mlp network.
 * Looping on: {
 *                  Retrieve user input
 *                  Feed input to mlpNetwork
 *                  print image & netowrk prediction
 *             }
 * Throws an exception on fatal errors: unable to read user input path.
 * @param mlp MlpNetwork to use in order to predict img.
 * @throw std::invalid_argument in case of problem with the user input path
 */
void mlpCli (MlpNetwork &mlp) noexcept (false)
{
  Matrix img (img_dims.rows, img_dims.cols);
  std::string imgPath;

  std::cout << INSERT_IMAGE_PATH << std::endl;
  std::cin >> imgPath;
  if (!std::cin.good ())
  {
    throw std::invalid_argument (ERROR_INVALID_INPUT);
  }

  while (imgPath != QUIT)
  {
    if (readFileToMatrix (imgPath, img))
    {
      Matrix imgVec = img;
      //imgVec.plain_print();
      digit output = mlp (imgVec.vectorize ());
      std::cout << "Image processed:" << std::endl
                << img << std::endl;
      std::cout << "Mlp result: " << output.value <<
                " at probability: " << output.probability << std::endl;
    }
    else
    {
      throw std::invalid_argument (ERROR_INVALID_IMG + imgPath);
    }

    std::cout << INSERT_IMAGE_PATH << std::endl;
    std::cin >> imgPath;
    if (!std::cin.good ())
    {
      throw std::invalid_argument (ERROR_INVALID_INPUT);
    }
  }
}

/**
 * Program's main
 * @param argc count of args
 * @param argv args values
 * @return program exit status code
 */

void testMatrixPlusEqual ()
{

  // Create two matrices
  Matrix matrix1 (3, 3);
  matrix1[0] = 10;
  matrix1 (0, 1) = 1;
  matrix1 (0, 2) = 2.0;
  matrix1 (1, 0) = 3.0;
  matrix1 (1, 1) = 4.0;
  matrix1 (1, 2) = 5.0;
  matrix1 (2, 0) = 6.0;
  matrix1 (2, 1) = 7.0;
  matrix1 (2, 2) = 8.0;

  matrix1.plain_print ();
//  //Matrix m;
//  matrix1.dot (matrix1).plain_print();
//  //plain_print ();
//  (matrix1 * matrix1).plain_print ();
//  (matrix1*0.5).plain_print();
//  (1.5 *matrix1).plain_print();
//  (matrix1 + matrix1).plain_print();
//  std::cout << "be" << std::endl;
//
//
 // activation::softmax (matrix1).plain_print();

  Matrix matrix(1, 9);
  //std::cout << std::exp(6) << std::endl;
  // Set the values in the matrix
  matrix(0, 0) = -2;
  matrix(0, 1) = -1;
  matrix(0, 2) = 0;
  matrix(0, 3) = 1;
  matrix(0, 4) = 2;
  matrix(0, 5) = 3;
  matrix(0, 6) = 4;
  matrix(0, 7) = 5;
  matrix(0, 8) = 6;
  matrix.transpose();
  activation::relu(matrix).plain_print();
  matrix.plain_print();



}

int main (int argc, char **argv)
{

  testMatrixPlusEqual ();

  try
  {
    usage (argc);
  }
  catch (const std::domain_error &domainError)
  {
    std::cerr << domainError.what () << std::endl;
    return EXIT_FAILURE;

  }

  Matrix weights[MLP_SIZE];
  Matrix biases[MLP_SIZE];

  try
  {
    loadParameters (argv, weights, biases);

  }
  catch (const std::invalid_argument &invalidArgument)
  {
    std::cerr << invalidArgument.what () << std::endl;
    return EXIT_FAILURE;
  }

  MlpNetwork mlp (weights, biases);

  try
  {
    mlpCli (mlp);
  }

  catch (const std::invalid_argument &invalidArgument)
  {
    std::cerr << invalidArgument.what () << std::endl;
    return EXIT_FAILURE;

  }

  return EXIT_SUCCESS;
}


// sum of each row
//void div_mat (Matrix &matrix ,float sum )
//{
//  // Save rows and cols
//  int row = matrix.get_rows ();
//  int cols = matrix.get_cols ();
//
//  // Divide each element by the total sum of its row
//  for (int j = 0; j < cols; j++)
//  {
//    // sum each row
//    //float row_sum = 0.0;
//
//    for (int i = 0; i < row; i++)
//    {
//
//      // Calculate the sum of the row
//      row_sum += matrix(i, j);
//    }
//
//    // Divide each element by the row sum
//    for (int i = 0; i < row; i++)
//    {
//      matrix(i,j)= matrix(i, j) / row_sum;
//    }
//  }
//}
//  // Allocate new matrix
//  _mat = new float[_rows * _cols];