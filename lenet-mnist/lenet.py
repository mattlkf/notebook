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


# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-s", "--save-model", type=int, default=-1,
  help="(optional) whether or not model should be saved to disk")
ap.add_argument("-l", "--load-model", type=int, default=-1,
  help="(optional) whether or not pre-trained model should be loaded")
ap.add_argument("-w", "--weights", type=str,
  help="(optional) path to weights file")
args = vars(ap.parse_args())

# grab the MNIST dataset (if this is your first time running this
# script, the download may take a minute -- the 55MB MNIST dataset
# will be downloaded)
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

# initialize the optimizer and model
print("[INFO] compiling model...")
opt = SGD(lr=0.01)
model = LeNet_small5.build(width=28, height=28, depth=1, classes=10,
  weightsPath=args["weights"] if args["load_model"] > 0 else None)

layer1 = model.layers[0]

model.compile(loss="categorical_crossentropy", optimizer=opt,
  metrics=["accuracy"])

# Print out the model
model.summary()

# only train and evaluate the model if we *are not* loading a
# pre-existing model
if args["load_model"] < 0:
  print("[INFO] training...")
  model.fit(trainData, trainLabels, batch_size=128, epochs=1,
    verbose=1)

  # show the accuracy on the testing set
  print("[INFO] evaluating...")
  (loss, accuracy) = model.evaluate(testData, testLabels,
    batch_size=128, verbose=1)
  print("[INFO] accuracy: {:.2f}%".format(accuracy * 100))

# check to see if the model should be saved to file
if args["save_model"] > 0:
  print("[INFO] dumping weights to file...")
  model.save_weights(args["weights"], overwrite=True)

# randomly select a few testing digits
for i in np.random.choice(np.arange(0, len(testLabels)), size=(10,)):
  # classify the digit
  probs = model.predict(testData[np.newaxis, i])
  prediction = probs.argmax(axis=1)

  # show the image and prediction
  print("[INFO] Predicted: {}, Actual: {}".format(prediction[0],
    np.argmax(testLabels[i])))

# print intermediate output
get_1st_layer_output = K.function([model.layers[0].input],
                                  [model.layers[1].output])

layer1_output = get_1st_layer_output([testData[np.newaxis, 0]])[0]

np.set_printoptions(threshold=np.inf)
# print layer1_output
# print layer1_output.shape



# print layer1_output[0][0]
# print layer1_output[0][0].shape

for i, w in enumerate(all_weights):
  if i == 0:
    # np.savetxt(f, w, fmt="%.2f")
    print w
    print w.shape

print testData[0]
print testData[0].shape
print np.argmax(testLabels[0])