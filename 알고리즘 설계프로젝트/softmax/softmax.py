import tensorflow as tf
import numpy as np
tf.set_random_seed(777)

xy = np.genfromtxt('data1.csv', delimiter=',')
x_data = xy[:442, :6]
y_data = xy[:442, 6:]

x_test = xy[442:, :6]
y_test = xy[442:, 6:]

X = tf.placeholder(tf.float32, shape=[None, 6])
Y = tf.placeholder(tf.float32, shape=[None, 9])  # label의 갯수 : 9개

nb_classes = 9

W = tf.Variable(tf.random_normal([6, nb_classes]), name='weight')
b = tf.Variable(tf.random_normal([nb_classes]), name='bias')

hypothesis = tf.nn.softmax(tf.matmul(X, W) + b)
cost = tf.reduce_mean(-tf.reduce_sum(Y * tf.log(hypothesis), axis=1))
optimizer = tf.train.GradientDescentOptimizer(learning_rate=0.1).minimize(cost)

prediction = tf.arg_max(hypothesis, 1)
is_correct = tf.equal(prediction, tf.arg_max(Y, 1))
accuracy = tf.reduce_mean(tf.cast(is_correct, tf.float32))

with tf.Session() as sess:
    sess.run(tf.global_variables_initializer())

    for step in range(40001):
        sess.run(optimizer, feed_dict={X: x_data, Y: y_data})
        if step % 100 == 0:
            cost_val, W_val, _ = sess.run([cost, W, optimizer], feed_dict={X: x_data, Y: y_data})
            print('step:', step, 'cost:', cost_val)

    print('------------------')
    print("12121710최민석")
    h, p, a = sess.run([hypothesis, prediction, accuracy], feed_dict={X: x_test, Y: y_test})
    print("\nHypothesis: ", h)
    p = sess.run(tf.arg_max(h, 1))
    for i in p:
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
    print("Accuracy:", sess.run(accuracy, feed_dict={X: x_test, Y: y_test}))
