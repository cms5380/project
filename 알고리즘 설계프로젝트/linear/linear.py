# Lab 4 Multi-variable linear regression
import tensorflow as tf
import numpy as np
tf.set_random_seed(777)  # for reproducibility

xy = np.genfromtxt('data1.csv', delimiter=',')
x_data = xy[:442, :6]
y_data = xy[:442, 6:]

x_test = xy[442:, :6]
y_test = xy[442:, 6:]



# placeholders for a tensor that will be always fed.
X = tf.placeholder(tf.float32, shape=[None, 6])
Y = tf.placeholder(tf.float32, shape=[None, 9])

W = tf.Variable(tf.random_normal([6, 9]), name='weight')
b = tf.Variable(tf.random_normal([9]), name='bias')

# Hypothesis
hypothesis = tf.matmul(X, W) + b

# Simplified cost/loss function
cost = tf.reduce_mean(tf.square(hypothesis - Y))

# Minimize
optimizer = tf.train.GradientDescentOptimizer(learning_rate=0.1)
train = optimizer.minimize(cost)

prediction = tf.arg_max(hypothesis, 1)
is_correct = tf.equal(prediction, tf.arg_max(Y, 1))
accuracy = tf.reduce_mean(tf.cast(is_correct, tf.float32))

# Launch the graph in a session.
sess = tf.Session()
# Initializes global variables in the graph.
sess.run(tf.global_variables_initializer())

for step in range(40001):
    cost_val, hy_val, _ = sess.run(
        [cost, hypothesis, train], feed_dict={X: x_data, Y: y_data})
    if step % 100 == 0:
        print(step, "Cost: ", cost_val)

print("-----------------")
print("12121710최민석")
h, p, a = sess.run([hypothesis, prediction, accuracy], feed_dict={X: x_test, Y: y_test})
print("\nHypothesis: ", h)
p = sess.run(prediction, feed_dict={X: x_test})
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


