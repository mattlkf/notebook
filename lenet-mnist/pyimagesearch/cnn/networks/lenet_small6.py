# import the necessary packages
from keras.models import Sequential
from keras.layers.convolutional import Convolution2D
from keras.layers.convolutional import MaxPooling2D
from keras.layers.core import Activation
from keras.layers.core import Flatten
from keras.layers.core import Dense
from keras import backend as bk

class LeNet_small6:
  @staticmethod
  def build(width, height, depth, classes, weightsPath=None):
    bk.set_image_dim_ordering('th')
    # initialize the model
    model = Sequential()

    # first set of CONV => RELU => POOL
    model.add(Convolution2D(5, (5, 5), padding="same",
      input_shape=(depth, height, width)))
    model.add(Activation("relu"))
    model.add(MaxPooling2D(pool_size=(2, 2), strides=(2, 2)))

    # second set of CONV => RELU => POOL
    model.add(Convolution2D(5, (5, 5), padding="same"))
    model.add(Activation("relu"))
    model.add(MaxPooling2D(pool_size=(2, 2), strides=(2, 2)))

    # set of FC => RELU layers
    model.add(Flatten())
    model.add(Dense(20))
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


# (keras) matt@x1c:~/git/notebook/lenet-mnist$ python lenet.py -s 1 -w small6_weights.hdf5
# Using Theano backend.
# /home/matt/.virtualenvs/keras/local/lib/python2.7/site-packages/sklearn/cross_validation.py:44: DeprecationWarning: This module was deprecated in version 0.18 in favor of the model_selection module into which all the refactored classes and functions are moved. Also note that the interface of the new CV iterators are different from that of this module. This module will be removed in 0.20.
#   "This module will be removed in 0.20.", DeprecationWarning)
# [INFO] downloading MNIST...
# [INFO] compiling model...
# _________________________________________________________________
# Layer (type)                 Output Shape              Param #   
# =================================================================
# conv2d_1 (Conv2D)            (None, 5, 28, 28)         130       
# _________________________________________________________________
# activation_1 (Activation)    (None, 5, 28, 28)         0         
# _________________________________________________________________
# max_pooling2d_1 (MaxPooling2 (None, 5, 14, 14)         0         
# _________________________________________________________________
# conv2d_2 (Conv2D)            (None, 5, 14, 14)         630       
# _________________________________________________________________
# activation_2 (Activation)    (None, 5, 14, 14)         0         
# _________________________________________________________________
# max_pooling2d_2 (MaxPooling2 (None, 5, 7, 7)           0         
# _________________________________________________________________
# flatten_1 (Flatten)          (None, 245)               0         
# _________________________________________________________________
# dense_1 (Dense)              (None, 20)                4920      
# _________________________________________________________________
# activation_3 (Activation)    (None, 20)                0         
# _________________________________________________________________
# dense_2 (Dense)              (None, 10)                210       
# _________________________________________________________________
# activation_4 (Activation)    (None, 10)                0         
# =================================================================
# Total params: 5,890
# Trainable params: 5,890
# Non-trainable params: 0
# _________________________________________________________________
# [INFO] training...
# Epoch 1/5
# 46900/46900 [==============================] - 31s - loss: 1.9977 - acc: 0.3690     
# Epoch 2/5
# 46900/46900 [==============================] - 31s - loss: 0.5848 - acc: 0.8241     
# Epoch 3/5
# 46900/46900 [==============================] - 31s - loss: 0.3450 - acc: 0.8953     
# Epoch 4/5
# 46900/46900 [==============================] - 31s - loss: 0.2795 - acc: 0.9145     
# Epoch 5/5
# 46900/46900 [==============================] - 31s - loss: 0.2423 - acc: 0.9259     
# [INFO] evaluating...
# 22912/23100 [============================>.] - ETA: 0s[INFO] accuracy: 92.62%
# [INFO] dumping weights to file...
# [INFO] Predicted: 6, Actual: 6
# [INFO] Predicted: 1, Actual: 1
# [INFO] Predicted: 4, Actual: 4
# [INFO] Predicted: 6, Actual: 6
# [INFO] Predicted: 2, Actual: 2
# [INFO] Predicted: 9, Actual: 9
# [INFO] Predicted: 7, Actual: 7
# [INFO] Predicted: 2, Actual: 2
# [INFO] Predicted: 9, Actual: 9
# [INFO] Predicted: 2, Actual: 2
