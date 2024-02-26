// ------------------------------ includes ------------------------------
#include "MlpNetwork.h"

MlpNetwork::MlpNetwork (const Matrix weights_array[], const Matrix
bias_array[])
    :
    layer1 (Dense (weights_array[0], bias_array[0], activation::relu)),
    layer2 (Dense (weights_array[1], bias_array[1], activation::relu)),
    layer3 (Dense (weights_array[2], bias_array[2], activation::relu)),
    layer4 (Dense (weights_array[3], bias_array[3], activation::softmax))
{}

digit MlpNetwork::operator() (Matrix &img) const
{
  // start digit
  digit ret;

  // vectorize img
  img.vectorize ();

  // apply Net
  Matrix r1 = layer1 (img);
  Matrix r2 = layer2 (r1);
  Matrix r3 = layer3 (r2);
  Matrix r4 = layer4 (r3);

  // Update digit
  ret.value = r4.argmax ();
  ret.probability = r4[ret.value];

  // return a copy , by value
  return ret;

}