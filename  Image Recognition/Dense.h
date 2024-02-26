#ifndef DENSE_H
#define DENSE_H


// ------------------------------ includes ------------------------------
#include "Activation.h"

// ------------------------------ const & macros -----------------------------

class Dense
{
 private:
  Matrix _bias;

  Matrix _weights;

  func _type;

 public:
// ----------------- Constructors -------------------
  Dense (const Matrix& weights,const Matrix& bias, func type);

  // -------------- getter and setter ----------------
  /**
* get_weights
* return returns the weights
*/
  Matrix get_weights () const;

  /**
* get_bias
* return returns the bias
*/
  Matrix get_bias () const;

  /**
* get_activation
* return returns the activation
*/
  func get_activation () const;

  // ----------------- Operators -------------------
  Matrix operator() (const Matrix &x) const;

};

#endif //DENSE_H
