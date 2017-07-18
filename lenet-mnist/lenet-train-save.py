# USAGE
# python lenet_mnist.py --save-model 1 --weights output/lenet_weights.hdf5
# python lenet_mnist.py --load-model 1 --weights output/lenet_weights.hdf5

# import the necessary packages
from pyimagesearch.cnn.networks import LeNet_small5
from sklearn.cross_validation import train_test_split
from sklearn import datasets
from keras.optimizers import SGD
from keras.utils import np_utils
from keras import backend as K
import numpy as np
import argparse

# which model to use?
model_class = LeNet_small5

# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("file", type=str, help="where to save weights + data")
ap.add_argument("-n", type=int, default=1, help="(optional) how many images to save")
ap.add_argument("-e", type=int, default=1, help="(optional) how many epochs to train for")
args = vars(ap.parse_args())

print("[INFO] downloading MNIST...")
dataset = datasets.fetch_mldata("MNIST Original")

# reshape the MNIST dataset from a flat list of 784-dim vectors, to
# 28 x 28 pixel images, then scale the data to the range [0, 1.0]
# and construct the training and testing splits
data = dataset.data.reshape((dataset.data.shape[0], 28, 28))
data = data[:, np.newaxis, :, :]
(trainData, testData, trainLabels, testLabels_orig) = train_test_split(
  data / 255.0, dataset.target.astype("int"), test_size=0.33)

# transform the training and testing labels into vectors in the
# range [0, classes] -- this generates a vector for each label,
# where the index of the label is set to `1` and all other entries
# to `0`; in the case of MNIST, there are 10 class labels
trainLabels = np_utils.to_categorical(trainLabels, 10)
testLabels = np_utils.to_categorical(testLabels_orig, 10)

# initialize the optimizer and model
print("[INFO] compiling model...")
opt = SGD(lr=0.01)
model = model_class.build(width=28, height=28, depth=1, classes=10,
  weightsPath="small5_2.hdf5")

model.compile(loss="categorical_crossentropy", optimizer=opt,
  metrics=["accuracy"])

# Print out the model
model.summary()

# print("[INFO] training...")
# model.fit(trainData, trainLabels, batch_size=128, epochs=1,
#   verbose=1)


# show the accuracy on the testing set

# print("[INFO] evaluating...")
# (loss, accuracy) = model.evaluate(testData, testLabels,
#   batch_size=128, verbose=1)
# print("[INFO] accuracy: {:.2f}%".format(accuracy * 100))


# Create a file to write the images to
f = open(args['file'], 'w')

# Print Model Weights

np.set_printoptions(threshold=np.inf)
all_weights = model.get_weights()

# for i, w in enumerate(all_weights):
print >>f, all_weights[0]
print >>f, all_weights[1]

# Print image(s)

np.savetxt(f, testData[0][0], fmt="%.2f")

# For each image, print the image data and the output of every layer

get_1st_layer_output = K.function([model.layers[0].input],
                                  [model.layers[1].output])

layer1_output = get_1st_layer_output([testData[np.newaxis, 0]])[0]

np.savetxt(f, layer1_output[0][0], fmt="%.2f")
np.savetxt(f, layer1_output[0][1], fmt="%.2f")

print layer1_output.shape


# print layer1_output[0][0]
# print layer1_output[0][0].shape

# print testData[0]
# print testData[0].shape
# print np.argmax(testLabels[0])


# print intermediate output
# get_1st_layer_output = K.function([model.layers[0].input],
#                                   [model.layers[1].output])

# layer1_output = get_1st_layer_output([testData[np.newaxis, 0]])[0]

# for i in xrange(n):
#   print >>f, np.argmax(testLabels[i])

# for i in xrange(n):
#   np.savetxt(f, testData[i][0], fmt="%.2f")