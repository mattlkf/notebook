# import the necessary packages
from pyimagesearch.cnn.networks import LeNet_small4
from sklearn.cross_validation import train_test_split
from sklearn import datasets
from keras.optimizers import SGD
from keras.utils import np_utils
import numpy as np
import argparse

# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-w", "--weights", type=str,
  help="(optional) path to weights file")
args = vars(ap.parse_args())

# initialize the optimizer and model
print("[INFO] compiling model...")
opt = SGD(lr=0.01)
model = LeNet_small4.build(width=28, height=28, depth=1, classes=10,
  weightsPath=args["weights"])
model.compile(loss="categorical_crossentropy", optimizer=opt,
  metrics=["accuracy"])

# Print summary of model
model.summary()

# Print the weights of layer 1
all_weights = model.get_weights()

np.set_printoptions(threshold=np.inf)

for w in all_weights:
#   print w
  print w.shape

# print all_weights[0]
# print all_weights[0].shape
# print all_weights[1]
