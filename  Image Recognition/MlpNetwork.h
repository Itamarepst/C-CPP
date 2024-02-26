//MlpNetwork.h

#ifndef MLPNETWORK_H
#define MLPNETWORK_H

// ------------------------------ includes ------------------------------

#include "Dense.h"

// ------------------------------ const & macros -----------------------------

#define MLP_SIZE 4

/**
 * @struct digit
 * @brief Identified (by Mlp network) digit with
 *        the associated probability.
 * @var value - Identified digit value
 * @var probability - identification probability
 */
typedef struct digit
{
    unsigned int value;
    float probability;
} digit;

const matrix_dims img_dims = {28, 28};
const matrix_dims weights_dims[] = {{128, 784},
                                    {64,  128},
                                    {20,  64},
                                    {10,  20}};
const matrix_dims bias_dims[] = {{128, 1},
                                 {64,  1},
                                 {20,  1},
                                 {10,  1}};

// Insert MlpNetwork class here...
// ------------------------------ Class -----------------------------
class MlpNetwork
{
 private:
  // layers of net
  Dense layer1;
  Dense layer2;
  Dense layer3;
  Dense layer4;



 public:
  // ----------------- Constructors -------------------
/**
 * @class MlpNetwork
 * @brief starts all the layer matrix's
 */
  MlpNetwork(const Matrix weights_array[] ,const Matrix bias_array[]);

/**
 * @operator ()
 * runs the layer
 */
  digit operator()( Matrix & matrix) const  ;


};

#endif // MLPNETWORK_H