import os
import time
import threading
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler
import numpy as np
import pandas as pd
from pandas import Series, DataFrame
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import accuracy_score, classification_report
from sklearn.model_selection import train_test_split
from sklearn.neighbors import KNeighborsClassifier
from sklearn.model_selection import KFold
import matplotlib.pyplot as plt


# warnings.filterwarnings('ignore')

def predicting(knn, data):
    print("Daemon Thread Start!\n")
    result = knn.predict(data)
    print(result)
    S = sum(result)
    n1 = len(data.index)
    if (S / len(result) < 0.5):
        print("normal")
    else:
        print("malware")
    print("Daemon Thread end!\n")


class Target:
    watchDir = "C:\\Users\\cms53\\Desktop\\올빼미\\어치\\csv_file\\"

    def __init__(self):
        self.observer = Observer()

    def run(self):
        event_handler = Handler()
        self.observer.schedule(event_handler, self.watchDir, recursive=True)
        self.observer.start()
        try:
            while True:
                time.sleep(1)
        except KeyboardInterrupt:
            self.observer.stop()
            print("Error")
            self.observer.join()


class Handler(FileSystemEventHandler):
    def on_moved(self, event):
        print(event)

    def on_created(self, event):
        global data_x
        global new_knn
        print(event)
        print("파일생성됨!\n")
        new_path_dir = "C:\\Users\\cms53\\Desktop\\올빼미\\어치\\csv_file\\"
        new_file_list = os.listdir(new_path_dir)
        new_data = DataFrame(columns=('fr-num', 'fr-len', 'src-ip',
                                      'dst-ip', 'src-port', 'dst-port', 'num_get_post',
                                      'count-serv-src', 'count-serv-dst', 'num-packets-src-dst', 'num-packets-dst-src'))
        for file_name in new_file_list:
            file_full_name = new_path_dir + file_name
            temp = pd.read_csv(file_full_name, names=['fr-num', 'fr-len', 'src-ip',
                                                      'dst-ip', 'src-port', 'dst-port', 'num_get_post',
                                                      'count-serv-src', 'count-serv-dst', 'num-packets-src-dst',
                                                      'num-packets-dst-src'], encoding='utf_8')
            new_data = pd.concat([new_data, temp])
            os.remove(file_full_name)
        new_data = new_data.dropna(axis=0)
        n = len(data_x.index)
        new_data_x = pd.concat([data_x, new_data])
        new_data_x = new_data_x.drop('fr-num', axis=1)
        new_data_x_stdscle = StandardScaler().fit_transform(new_data_x.values)
        new_data_x_stdscle_df = pd.DataFrame(new_data_x_stdscle, index=new_data_x.index, columns=new_data_x.columns)
        new_data_x_stdscle_df = new_data_x_stdscle_df[n:]
        new_data_x_1 = new_data_x[n:]
        th = threading.Thread(target=predicting, args=(new_knn, new_data_x_1))
        th.daemon = True
        th.start()

    def on_deleted(self, event):
        print(event)

    def on_modified(self, event):
        print(event)


data = DataFrame(columns=('fr-num', 'fr-len', 'src-ip',
                          'dst-ip', 'src-port', 'dst-port', 'num_get_post',
                          'count-serv-src', 'count-serv-dst', 'num-packets-src-dst', 'num-packets-dst-src', 'label'))

path_dir = "C:\\Users\\cms53\\Desktop\\올빼미\\어치\\csv_파일들\\"
# '/home/ubuntu/Mechine-learning-model/normal-data/'
file_list = os.listdir(path_dir)

for file_name in file_list:
    file_full_name = path_dir + file_name
    temp = pd.read_csv(file_full_name, names=['fr-num', 'fr-len', 'src-ip',
                                              'dst-ip', 'src-port', 'dst-port', 'num_get_post',
                                              'count-serv-src', 'count-serv-dst', 'num-packets-src-dst',
                                              'num-packets-dst-src', 'label'], encoding='utf_8')
    data = pd.concat([data, temp])

data = data.dropna(axis=0)  # NAN 값이 있으면 그 행을 없엔다.
data_x = data.drop(['fr-num', 'label'], axis=1)  # 훈련 및 구별하는데 사용되어질 parameter들
data_y = data['label'].astype('int')  # 도출되어질 결과값들 malware=1 normal=0

data_x_stdscle = StandardScaler().fit_transform(data_x.values)  # 각 parameter들이 평균이 0 표준편차가 1 인 정규분포를 가지도록
# 정규화 해줌
data_x_stdscle_df = pd.DataFrame(data_x_stdscle, index=data_x.index, columns=data_x.columns)
X_train, X_test, Y_train, Y_test = train_test_split(data_x, data_y, train_size=0.7,
                                                    random_state=42)  # train과 test set 을 7:3 비율로 나눠줌

# k값 튜닝
dummyarray = np.empty((5, 3))
k_valchart = pd.DataFrame(dummyarray)
k_valchart.columns = ["K_value", "Train_acc", "Test_acc"]

k_vals = np.arange(1, 19, 2)
optimun_k = 17
min = 99999

'''
min=99999
for i in range(len(k_vals)):
    knn=KNeighborsClassifier(n_neighbors=k_vals[i],p=2,metric='minkowski')
    knn.fit(X_train,Y_train)

    tr_accscore=round(accuracy_score(Y_train,knn.predict(X_train)),3)
    print("\nK-Nearest-Neighbors -Train Confusion Matrix\n\n",pd.crosstab(Y_train,knn.predict(X_train),rownames=["Actuall"],colnames=["Predicted"]) )
    print("\nK-Nearest Neighbors - Train accuracy:",tr_accscore)
    print("\nK-Nearest-Neighbors - Train Classification Report\n",classification_report(Y_train,knn.predict(X_train)))

    ts_accscore=round(accuracy_score(Y_test,knn.predict(X_test)),3)
    print("\nK-Nearest-Neighbors -Test Confusion Matrix\n\n",pd.crosstab(Y_test,knn.predict(X_test),rownames=["Actuall"],colnames=["Predicted"]) )
    print("\nK-Nearest Neighbors - Test accuracy:",ts_accscore)
    print("\nK-Nearest-Neighbors - Test Classification Report\n",classification_report(Y_test,knn.predict(X_test)))

    if(min>tr_accscore-ts_accscore):
        min=tr_accscore-ts_accscore
        optimun_k=i

    k_valchart.loc[i,'K_value']=k_vals[i]
    k_valchart.loc[i,'Train_acc']=tr_accscore
    k_valchart.loc[i,'Test_acc']=ts_accscore


plt.figure()
plt.xlabel('K-value')
plt.ylabel('Accuracy')
plt.plot(k_valchart["K_value"],k_valchart["Train_acc"])
plt.plot(k_valchart["K_value"],k_valchart["Test_acc"])
plt.axis([0.9,5, 0.95, 1.005])
plt.xticks([3,5,7,9,11,13,15,17,19,21])

for a,b in zip(k_valchart["K_value"],k_valchart["Train_acc"]):
    plt.text(a,b,str(b),fontsize=10)

for a,b in zip(k_valchart["K_value"],k_valchart["Test_acc"]):
    plt.text(a,b,str(b),fontsize=10)

plt.legend(loc='upper right')
plt.show()
'''
new_knn = KNeighborsClassifier(n_neighbors=optimun_k, p=3, metric='minkowski')
new_knn.fit(data_x, data_y)

ts_accscore = round(accuracy_score(data_y, new_knn.predict(data_x)), 3)
print("\nK-Nearest-Neighbors -Test Confusion Matrix\n\n",
      pd.crosstab(data_y, new_knn.predict(data_x), rownames=["Actuall"], colnames=["Predicted"]))
print("\nK-Nearest Neighbors - Test accuracy:", ts_accscore)
print("\nK-Nearest-Neighbors - Test Classification Report\n", classification_report(data_y, new_knn.predict(data_x)))

# 여기서부터 실시간
print("실시간 감시 시작!\n")
w = Target()
w.run()


