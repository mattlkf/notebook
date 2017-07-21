# Save the model weights first, followed by images.

# import the necessary packages
from pyimagesearch.cnn.networks import LeNet_valid
from sklearn.cross_validation import train_test_split
from sklearn import datasets
from keras.optimizers import SGD
from keras.utils import np_utils
from keras import backend as K
import numpy as np
import argparse
import sys
# which model to use?
model_class = LeNet_valid

# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("file", type=str, help="where to save weights + data")
ap.add_argument("-n", type=int, default=1, help="(optional) how many images to save")
ap.add_argument("-e", type=int, default=0, help="(optional) how many epochs to train for")
ap.add_argument("-s", type=float, default=1.0, help="(optional) how much to downscale bias + images by")
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

print "testData shape: ", testData.shape

# transform the training and testing labels into vectors in the
# range [0, classes] -- this generates a vector for each label,
# where the index of the label is set to `1` and all other entries
# to `0`; in the case of MNIST, there are 10 class labels
trainLabels = np_utils.to_categorical(trainLabels, 10)
testLabels = np_utils.to_categorical(testLabels_orig, 10)

# initialize the optimizer and model
print("[INFO] compiling model...")
opt = SGD(lr=0.01)
# model = model_class.build(width=28, height=28, depth=1, classes=10, weightsPath="small5_2.hdf5")
model = model_class.build(width=28, height=28, depth=1, classes=10)

model.compile(loss="categorical_crossentropy", optimizer=opt,
  metrics=["accuracy"])

# Print out the model
model.summary()

# print("[INFO] training...")
model.fit(trainData, trainLabels, batch_size=128, epochs=args['e'], verbose=1)

# show the accuracy on the testing set

if args['e'] > 0:
  print("[INFO] evaluating...")
  (loss, accuracy) = model.evaluate(testData, testLabels,
    batch_size=128, verbose=1)
  print("[INFO] accuracy: {:.2f}%".format(accuracy * 100))

# Create a file to write to
f = open(args['file'], 'w')

# Print Model Weights

np.set_printoptions(threshold=np.inf)
all_weights = model.get_weights()

for i, w in enumerate(all_weights):
  print w.shape
  # If this is a bias vector, scale it down
  if i % 2 == 1:
    w = w / args['s']
  elif i < 5:
    w = np.moveaxis(w, [2,3], [1,0])
    w = np.flip(w,3)
    w = np.flip(w,2)
  else:
    w = np.transpose(w)
  print >>f, w

print >>f, args['n']

# Print image(s) by randomly selecting a few digits
for i in np.random.choice(np.arange(0, len(testLabels)), size=(args['n'],)):
  # classify the digit
  probs = model.predict(testData[np.newaxis, i])
  prediction = probs.argmax(axis=1)

  # save the image data
  np.savetxt(f, testData[i][0] / args['s'], fmt="%.6f")
  print >>f, prediction, testLabels_orig[i]

  # show the image and prediction
  print("[INFO] Predicted: {}, Actual: {}".format(prediction[0],
    np.argmax(testLabels[i])))

  # get_layer_output = K.function([model.layers[0].input],
  #                               [model.layers[11].output])

  # layer_output = get_layer_output([testData[np.newaxis, i]])[0]


  # np.savetxt(sys.stdout, layer_output[0], fmt="%.3f")

