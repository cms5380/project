# Lab 9 XOR-back_prop
import tensorflow as tf
import numpy as np

tf.set_random_seed(777)
learning_rate = 0.1
nb_classes = 9 #output number

xy = np.genfromtxt('data1.csv', delimiter=',') #read data

#학습시킬 데이터
x_data = xy[:442, :6]
y_data = xy[:442, 6:]

#테스트 할 데이터
x_test = xy[442:, :6]
y_test = xy[442:, 6:]


x_data = np.array(x_data, dtype=np.float32)
y_data = np.array(y_data, dtype=np.float32)


X = tf.placeholder(tf.float32, [None, 6])
Y = tf.placeholder(tf.float32, [None, nb_classes])


W1 = tf.Variable(tf.random_normal([6, nb_classes]), name='weight1')
b1 = tf.Variable(tf.random_normal([nb_classes]), name='bias1')
l1 = tf.sigmoid(tf.matmul(X, W1) + b1)

W2 = tf.Variable(tf.random_normal([nb_classes, nb_classes]), name='weight2')
b2 = tf.Variable(tf.random_normal([nb_classes]), name='bias2')
hypothesis = tf.sigmoid(tf.matmul(l1, W2) + b2)

# cost/loss function
cost = -tf.reduce_mean(Y * tf.log(hypothesis) + (1 - Y) * tf.log(1 - hypothesis))

# Loss derivative
d_hypothesis = (hypothesis - Y) / (hypothesis * (1.0 - hypothesis) + 1e-7)

# Layer 2
d_sigma2 = hypothesis * (1 - hypothesis)
d_a2 = d_hypothesis * d_sigma2
d_p2 = d_a2
d_b2 = d_a2
d_W2 = tf.matmul(tf.transpose(l1), d_p2)

# Mean
d_b2_mean = tf.reduce_mean(d_b2, axis=[0])
d_W2_mean = d_W2 / tf.cast(tf.shape(l1)[0], dtype=tf.float32)

# Layer 1
d_l1 = tf.matmul(d_p2, tf.transpose(W2))
d_sigma1 = l1 * (1 - l1)
d_a1 = d_l1 * d_sigma1
d_b1 = d_a1
d_p1 = d_a1
d_W1 = tf.matmul(tf.transpose(X), d_a1)

# Mean
d_W1_mean = d_W1 / tf.cast(tf.shape(X)[0], dtype=tf.float32)
d_b1_mean = tf.reduce_mean(d_b1, axis=[0])

# Weight update
step = [
  tf.assign(W2, W2 - learning_rate * d_W2_mean),
  tf.assign(b2, b2 - learning_rate * d_b2_mean),
  tf.assign(W1, W1 - learning_rate * d_W1_mean),
  tf.assign(b1, b1 - learning_rate * d_b1_mean)
]

# Accuracy computation
predicted = tf.arg_max(hypothesis, 1)
is_correct = tf.equal(predicted, tf.arg_max(Y, 1))
accuracy = tf.reduce_mean(tf.cast(is_correct, tf.float32))

# Launch graph
with tf.Session() as sess:
    # Initialize TensorFlow variables
    sess.run(tf.global_variables_initializer())

    print("shape", sess.run(tf.shape(X)[0], feed_dict={X: x_data}))


    for i in range(40001):
        sess.run([step, cost], feed_dict={X: x_data, Y: y_data})
        if i % 100 == 0:
            print(i, sess.run([cost, d_W1], feed_dict={X: x_data, Y: y_data}), sess.run([W1, W2]))

    # Accuracy report
    print('------------------')
    print("12121710최민석")
    h, c, a = sess.run([hypothesis, predicted, accuracy],feed_dict={X: x_test, Y: y_test})
    print("\nHypothesis: ", h)
    prediction = sess.run(tf.arg_max(h, 1))
    for i in prediction:
        if i == 8:
            print("Predicion: A+")
        elif i == 7:
            print("Predicion: A0")
        elif i == 6:
            print("Predicion: B+")
        elif i == 5:
            print("Predicion: B0")
        elif i == 4:
            print("Predicion: C+")
        elif i == 3:
            print("Predicion: C0")
        elif i == 2:
            print("Predicion: D+")
        elif i == 1:
            print("Predicion: D0")
        elif i == 0:
            print("Predicion: F")
    print("\nAccuracy: ", a)

