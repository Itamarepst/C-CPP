
#ifndef ACTIVATION_H
#define ACTIVATION_H


// ------------------------------ includes ------------------------------
#include "Matrix.h"

// ------------------------------ const & macros -----------------------------
typedef Matrix (*func) (const Matrix & mat) ;


// ------------------------------ namespace -----------------------------

namespace activation
{
    // The relu function - Matrix relu(Matrix & matrix );
    Matrix relu (const Matrix &matrix);

    // The softmax function - Matrix softmax(Matrix & matrix );
    Matrix softmax (const Matrix &matrix);

}



#endif //ACTIVATION_H