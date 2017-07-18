# Epoch 1/1
# 46900/46900 [==============================] - 114s - loss: 1.3398 - acc: 0.5671     
# [INFO] evaluating...
# 23040/23100 [============================>.] - ETA: 0s[INFO] accuracy: 86.41%
# [INFO] dumping weights to file...
# [INFO] Predicted: 5, Actual: 5
# [INFO] Predicted: 9, Actual: 9
# [INFO] Predicted: 9, Actual: 9
# [INFO] Predicted: 0, Actual: 0
# [INFO] Predicted: 3, Actual: 3
# [INFO] Predicted: 9, Actual: 9
# [INFO] Predicted: 7, Actual: 7
# [INFO] Predicted: 4, Actual: 4
# [INFO] Predicted: 7, Actual: 7
# [INFO] Predicted: 0, Actual: 0



# import the necessary packages
from keras.models import Sequential
from keras.layers.convolutional import Convolution2D
from keras.layers.convolutional import MaxPooling2D
from keras.layers.core import Activation
from keras.layers.core import Flatten
from keras.layers.core import Dense
from keras import backend as bk

class LeNet_small3:
  @staticmethod
  def build(width, height, depth, classes, weightsPath=None):
    bk.set_image_dim_ordering('th')
    # initialize the model
    model = Sequential()

    # first set of CONV => RELU => POOL
    model.add(Convolution2D(20, (5, 5), padding="same",
      input_shape=(depth, height, width)))
    model.add(Activation("relu"))
    model.add(MaxPooling2D(pool_size=(2, 2), strides=(2, 2)))

    # second set of CONV => RELU => POOL
    model.add(Convolution2D(10, (5, 5), padding="same"))
    model.add(Activation("relu"))
    model.add(MaxPooling2D(pool_size=(2, 2), strides=(2, 2)))

    # set of FC => RELU layers
    model.add(Flatten())
    model.add(Dense(50))
    model.add(Activation("relu"))

    # softmax classifier
    model.add(Dense(classes))
    model.add(Activation("softmax"))

    # if a weights path is supplied (inicating that the model was
    # pre-trained), then load the weights
    if weightsPath is not None:
      model.load_weights(weightsPath)

    # return the constructed network architecture
    return model