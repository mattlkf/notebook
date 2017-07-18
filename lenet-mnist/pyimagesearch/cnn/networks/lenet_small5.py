# (keras) matt@x1c:~/git/notebook/lenet-mnist$ python lenet.py -s 1 -w small5_weights.hdf5
# Using Theano backend.
# /home/matt/.virtualenvs/keras/local/lib/python2.7/site-packages/sklearn/cross_validation.py:44: DeprecationWarning: This module was deprecated in version 0.18 in favor of the model_selection module into which all the refactored classes and functions are moved. Also note that the interface of the new CV iterators are different from that of this module. This module will be removed in 0.20.
#   "This module will be removed in 0.20.", DeprecationWarning)
# [INFO] downloading MNIST...
# [INFO] compiling model...
# _________________________________________________________________
# Layer (type)                 Output Shape              Param #   
# =================================================================
# conv2d_1 (Conv2D)            (None, 2, 28, 28)         52        
# _________________________________________________________________
# activation_1 (Activation)    (None, 2, 28, 28)         0         
# _________________________________________________________________
# max_pooling2d_1 (MaxPooling2 (None, 2, 14, 14)         0         
# _________________________________________________________________
# conv2d_2 (Conv2D)            (None, 2, 14, 14)         102       
# _________________________________________________________________
# activation_2 (Activation)    (None, 2, 14, 14)         0         
# _________________________________________________________________
# max_pooling2d_2 (MaxPooling2 (None, 2, 7, 7)           0         
# _________________________________________________________________
# flatten_1 (Flatten)          (None, 98)                0         
# _________________________________________________________________
# dense_1 (Dense)              (None, 10)                990       
# _________________________________________________________________
# activation_3 (Activation)    (None, 10)                0         
# _________________________________________________________________
# dense_2 (Dense)              (None, 10)                110       
# _________________________________________________________________
# activation_4 (Activation)    (None, 10)                0         
# =================================================================
# Total params: 1,254
# Trainable params: 1,254
# Non-trainable params: 0
# _________________________________________________________________
# [INFO] training...
# Epoch 1/1
# 46900/46900 [==============================] - 9s - loss: 2.1249 - acc: 0.2649      
# [INFO] evaluating...
# 22784/23100 [============================>.] - ETA: 0s[INFO] accuracy: 44.93%
# [INFO] dumping weights to file...
# [INFO] Predicted: 8, Actual: 9
# [INFO] Predicted: 2, Actual: 2
# [INFO] Predicted: 8, Actual: 8
# [INFO] Predicted: 6, Actual: 4
# [INFO] Predicted: 1, Actual: 1
# [INFO] Predicted: 9, Actual: 6
# [INFO] Predicted: 1, Actual: 2
# [INFO] Predicted: 2, Actual: 4
# [INFO] Predicted: 7, Actual: 3
# [INFO] Predicted: 0, Actual: 0


# import the necessary packages
from keras.models import Sequential
from keras.layers.convolutional import Convolution2D
from keras.layers.convolutional import MaxPooling2D
from keras.layers.core import Activation
from keras.layers.core import Flatten
from keras.layers.core import Dense
from keras import backend as bk

class LeNet_small5:
  @staticmethod
  def build(width, height, depth, classes, weightsPath=None):
    bk.set_image_dim_ordering('th')
    # initialize the model
    model = Sequential()

    # first set of CONV => RELU => POOL
    conv1 = Convolution2D(2, (5, 5), padding="same",
      input_shape=(depth, height, width))

    model.add(conv1)
    model.add(Activation("relu"))
    model.add(MaxPooling2D(pool_size=(2, 2), strides=(2, 2)))

    # second set of CONV => RELU => POOL
    model.add(Convolution2D(2, (5, 5), padding="same"))
    model.add(Activation("relu"))
    model.add(MaxPooling2D(pool_size=(2, 2), strides=(2, 2)))

    # set of FC => RELU layers
    model.add(Flatten())
    model.add(Dense(10))
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