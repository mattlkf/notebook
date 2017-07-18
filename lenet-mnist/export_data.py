from sklearn import datasets
from sklearn.cross_validation import train_test_split
from keras.utils import np_utils
import numpy as np
import argparse

# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("n", type=int, help="how many images to write to file")
args = vars(ap.parse_args())

n = args['n']

print "n: ", n

# Create a file to write the images to
f = open('mnist_data', 'w')

print("[INFO] downloading MNIST...")
dataset = datasets.fetch_mldata("MNIST Original")

# reshape the MNIST dataset from a flat list of 784-dim vectors, to
# 28 x 28 pixel images, then scale the data to the range [0, 1.0]
# and construct the training and testing splits
data = dataset.data.reshape((dataset.data.shape[0], 28, 28))
data = data[:, np.newaxis, :, :]
(trainData, testData, trainLabels, testLabels) = train_test_split(
  data / 255.0, dataset.target.astype("int"), test_size=0.33)

# transform the training and testing labels into vectors in the
# range [0, classes] -- this generates a vector for each label,
# where the index of the label is set to `1` and all other entries
# to `0`; in the case of MNIST, there are 10 class labels
trainLabels = np_utils.to_categorical(trainLabels, 10)
testLabels = np_utils.to_categorical(testLabels, 10)

print testData[0][0]
print testData[0][0].shape
print np.argmax(testLabels[0])




for i in xrange(n):
  print >>f, np.argmax(testLabels[i])

for i in xrange(n):
  np.savetxt(f, testData[i][0], fmt="%.2f")