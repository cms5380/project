from IPython.core.interactiveshell import InteractiveShell
InteractiveShell.ast_node_interactivity = "all"
import os
import datetime as dt
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from tqdm import tqdm
import cv2
import ast
import tensorflow
from pyexpat import model
import keras
from keras import Sequential
from keras.callbacks import EarlyStopping, ReduceLROnPlateau
from keras.layers import Conv2D, MaxPooling2D, Dropout, Dense, Activation, Flatten
from keras.metrics import top_k_categorical_accuracy, categorical_accuracy
from keras.backend import categorical_crossentropy
from keras.optimizers import Adam
from keras.utils import multi_gpu_model
start = dt.datetime.now()


#shuffle-csvs
"""
https://www.kaggle.com/gaborfodor/shuffle-csvs
"""
# def f2cat(filename: str) -> str:
#     return filename.split('.')[0]
#
# class Simplified():
#     def __init__(self, input_path='D:/'):
#         self.input_path = input_path
#
#     def list_all_categories(self):
#         files = os.listdir(os.path.join(self.input_path, 'train_simplified'))
#         return sorted([f2cat(f) for f in files], key=str.lower)
#
#     def read_training_csv(self, category, nrows=None, usecols=None, drawing_transform=False):
#         df = pd.read_csv(os.path.join(self.input_path, 'train_simplified', category + '.csv'),
#                          nrows=nrows, parse_dates=['timestamp'], usecols=usecols)
#         if drawing_transform:
#             df['drawing'] = df['drawing'].apply(json.loads)
#         return df
#
# start = dt.datetime.now()
# s = Simplified('D:/')
# NCSVS = 100
# categories = s.list_all_categories()
# print(len(categories))
#
# for y, cat in tqdm(enumerate(categories)):
#     df = s.read_training_csv(cat, nrows=30000)
#     df['y'] = y
#     df['cv'] = (df.key_id // 10 ** 7) % NCSVS
#     for k in range(NCSVS):
#         filename = 'train_k{}.csv'.format(k)
#         chunk = df[df.cv == k]
#         chunk = chunk.drop(['key_id'], axis=1)
#         if y == 0:
#             chunk.to_csv(filename, index=False)
#         else:
#             chunk.to_csv(filename, mode='a', header=False, index=False)
#
# for k in tqdm(range(NCSVS)):
#     filename = 'train_k{}.csv'.format(k)
#     if os.path.exists(filename):
#         df = pd.read_csv(filename)
#         df['rnd'] = np.random.rand(len(df))
#         df = df.sort_values(by='rnd').drop('rnd', axis=1)
#         df.to_csv(filename + '.gz', compression='gzip', index=False)
#         os.remove(filename)
# print(df.shape)
#
# end = dt.datetime.now()
# print('Latest run {}.\nTotal time {}s'.format(end, (end - start).seconds))

DP_DIR = './input/csvs/'
INPUT_DIR = './input/quickdraw-doodle-recognition/'

BASE_SIZE = 256
NCSVS = 100
cnt_classes = 340
np.random.seed(seed=1987)

STEPS = 600
size = 32
batchsize = 680

def f2cat(filename: str) -> str:
    return filename.split('.')[0]

def list_all_categories():
    files = os.listdir(os.path.join('D:/', 'train_simplified'))
    return sorted([f2cat(f) for f in files], key=str.lower)

def apk(actual, predicted, k=3):
    """
    Source: https://github.com/benhamner/Metrics/blob/master/Python/ml_metrics/average_precision.py
    """
    if len(predicted) > k:
        predicted = predicted[:k]

    score = 0.0
    num_hits = 0.0

    for i, p in enumerate(predicted):
        if p in actual and p not in predicted[:i]:
            num_hits += 1.0
            score += num_hits / (i + 1.0)

    if not actual:
        return 0.0

    return score / min(len(actual), k)

def mapk(actual, predicted, k=3):
    """
    Source: https://github.com/benhamner/Metrics/blob/master/Python/ml_metrics/average_precision.py
    """
    return np.mean([apk(a, p, k) for a, p in zip(actual, predicted)])


def preds2catids(predictions):
    return pd.DataFrame(np.argsort(-predictions, axis=1)[:, :3], columns=['a', 'b', 'c'])

def top_3_accuracy(y_true, y_pred):
    return top_k_categorical_accuracy(y_true, y_pred, k=3)

#이미지
"""
https://www.kaggle.com/gaborfodor/black-white-cnn-lb-0-77 참고
"""
def draw_cv2(raw_strokes, size=256, lw=6):
    img = np.zeros((BASE_SIZE, BASE_SIZE), np.uint8)
    for stroke in raw_strokes:
        for i in range(len(stroke[0]) - 1):
            _ = cv2.line(img, (stroke[0][i], stroke[1][i]), (stroke[0][i + 1], stroke[1][i + 1]), 255, lw)
    if size != BASE_SIZE:
        return cv2.resize(img, (size, size))
    else:
        return img

def image_generator(size, batchsize, ks, lw=6):
    while True:
        for k in np.random.permutation(ks):
            filename = os.path.join(DP_DIR, 'train_k{}.csv.gz'.format(k))
            for df in pd.read_csv(filename, chunksize=batchsize):
                df['drawing'] = df['drawing'].apply(ast.literal_eval)
                x = np.zeros((len(df), size, size))
                for i, raw_strokes in enumerate(df.drawing.values):
                    x[i] = draw_cv2(raw_strokes, size=size, lw=lw)
                x = x / 255.
                x = x.reshape((len(df), size, size, 1)).astype(np.float32)
                y = keras.utils.to_categorical(df.y, num_classes=cnt_classes)
                yield x, y

def df_to_image_array(df, size, lw=6):
    df['drawing'] = df['drawing'].apply(ast.literal_eval)
    x = np.zeros((len(df), size, size))
    for i, raw_strokes in enumerate(df.drawing.values):
        x[i] = draw_cv2(raw_strokes, size=size, lw=lw)
    x = x / 255.
    x = x.reshape((len(df), size, size, 1)).astype(np.float32)
    return x

valid_df = pd.read_csv(os.path.join(DP_DIR, 'train_k{}.csv.gz'.format(NCSVS - 1)), nrows=10**5)
x_valid = df_to_image_array(valid_df, size)
y_valid = keras.utils.to_categorical(valid_df.y, num_classes=cnt_classes)
print(x_valid.shape, y_valid.shape)
print('Validation array memory {:.2f} GB'.format(x_valid.nbytes / 1024.**3 ))
train_datagen = image_generator(size=size, batchsize=batchsize, ks=range(NCSVS - 1))
x, y = next(train_datagen)
n = 8
fig, axs = plt.subplots(nrows=n, ncols=n, sharex=True, sharey=True, figsize=(12, 12))
for i in range(n**2):
    ax = axs[i // n, i % n]
    ax.imshow(x[i, :, :, 0], cmap=plt.cm.gray)
    ax.axis('off')
plt.tight_layout()
fig.savefig('bw.png', dpi=300)
plt.show();


#cnn
model = Sequential()
model.add(Conv2D(32, kernel_size=(3, 3), padding='same', activation='relu', input_shape=(size, size, 1)))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Conv2D(64, kernel_size=(3, 3), padding='same', activation='relu'))
model.add(MaxPooling2D(pool_size=(2, 2)))
model.add(Dropout(0.2))
model.add(Flatten())
model.add(Dense(512, activation='relu'))
model.add(Dropout(0.2))
model.add(Dense(cnt_classes, activation='softmax'))

model.compile(optimizer=Adam(lr=0.002), loss='categorical_crossentropy',
              metrics=[categorical_crossentropy, categorical_accuracy, top_3_accuracy])
print(model.summary())

callbacks = [
    EarlyStopping(monitor='val_categorical_accuracy', patience=6, min_delta=0.001, mode='max'),
    ReduceLROnPlateau(monitor='val_categorical_accuracy', factor=0.5, patience=4, min_delta=0.005,
                      mode='max', cooldown=3)
]
hist = model.fit_generator(
    train_datagen, steps_per_epoch=STEPS, epochs=16, verbose=1,
    validation_data=(x_valid, y_valid),
    callbacks = callbacks
)

valid_predictions = model.predict(x_valid, batch_size=128, verbose=1)
map3 = mapk(valid_df[['y']].values, preds2catids(valid_predictions).values)
print('Map3: {:.3f}'.format(map3))

test = pd.read_csv(os.path.join(INPUT_DIR, 'test_simplified.csv'))
test.head()
x_test = df_to_image_array(test, size)
print(test.shape, x_test.shape)
print('Test array memory {:.2f} GB'.format(x_test.nbytes / 1024.**3 ))

test_predictions = model.predict(x_test, batch_size=128, verbose=1)

top3 = preds2catids(test_predictions)
top3.head()
top3.shape


cats = list_all_categories()
id2cat = {k: cat.replace(' ', '_') for k, cat in enumerate(cats)}
top3cats = top3.replace(id2cat)
top3cats.head()
top3cats.shape
test['word'] = top3cats['a'] + ' ' + top3cats['b'] + ' ' + top3cats['c']
submission = test[['key_id', 'word']]
submission.to_csv('bw_cnn_submission_{}.csv'.format(int(map3 * 10**4)), index=False)
submission.head()
submission.shape