// ------------------------------ includes ------------------------------
#include "Dense.h"


// ------------------------------ functions -----------------------------

Dense::Dense (const Matrix& weights, const Matrix& bias, func type) :
     _bias (bias), _weights (weights), _type (type)
{}

// -------------- getter and setter ----------------

Matrix Dense::get_weights () const
{
  return _weights;
}

Matrix Dense::get_bias () const
{
  return _bias;
}

func Dense::get_activation () const
{
  return _type;
}

// ----------------- Operators -------------------
Matrix Dense::operator() (const Matrix &x) const
{
  // Copy matrix
  Matrix output (x);

  // f(W* x + b )
  //(W*X)
  output = _weights * x;

  // (+b)
  output += _bias;

  //f()
  return _type (output);
}