import os
import pandas as pd
import numpy as np 
import warnings 
from pandas import Series, DataFrame
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import scale, robust_scale, minmax_scale,maxabs_scale
warnings.filterwarnings('ignore')

data=DataFrame(columns=('fr-num','fr-len','src-ip',
    'dst-ip','src-port','dst-port','num_get_post',
    'count-serv-src','count-serv-dst','num-packets-src-dst','num-packets-dst-src','label'))

path_dir =  "C:\\Users\\admin\\Desktop\\졸업프로젝트\\csv_파일들\\"
#'/home/ubuntu/Mechine-learning-model/normal-data/'
file_list = os.listdir(path_dir)
print(file_list)

for file_name in file_list:
    file_full_name = path_dir + file_name
    temp=pd.read_csv(file_full_name,names=['fr-num','fr-len','src-ip',
    'dst-ip','src-port','dst-port','num_get_post',
    'count-serv-src','count-serv-dst','num-packets-src-dst','num-packets-dst-src','label'],encoding='CP949')
    data=pd.concat([data,temp])

data=data.dropna(axis=0)
features=data.iloc[:,1:11]
labels=data['label']
labels=labels.astype('int')
X_train,X_test,Y_train,Y_test = train_test_split(features,labels,test_size=0.30)
print(X_train)
print(Y_train)

forest=RandomForestClassifier(n_estimators=20,random_state=3)
forest.fit(X_train,Y_train)
forest.predict(X_test)
print("train_set결과%d \n",forest.score(X_train,Y_train))
print("test_set결과%d \n",forest.score(X_test,Y_test))
