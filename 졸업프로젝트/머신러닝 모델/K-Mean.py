import os
import pandas as pd
import numpy as np
from scipy.spatial.distance import cdist,pdist 
from pandas import Series, DataFrame
from sklearn.cluster import KMeans
from sklearn.metrics import silhouette_score
from sklearn.neighbors import KNeighborsClassifier
from sklearn.pipeline import Pipeline
from sklearn.preprocessing import StandardScaler

data=DataFrame(columns=('fr-num','fr-len','src-ip',
    'dst-ip','src-port','dst-port','num_get_post',
    'count-serv-src','count-serv-dst','num-packets-src-dst','num-packets-dst-src','label'))
path_dir = "C:\\Users\\admin\\Desktop\\졸업프로젝트\\csv_파일들\\"
file_list = os.listdir(path_dir)
print(file_list)
#print(type(data))

for file_name in file_list:
    file_full_name = path_dir + file_name
    temp=pd.read_csv(file_full_name,names=['fr-num','fr-len','src-ip',
    'dst-ip','src-port','dst-port','num_get_post',
    'count-serv-src','count-serv-dst','num-packets-src-dst','num-packets-dst-src','label'],encoding='CP949')
    data=pd.concat([data,temp])

data=data.dropna(axis=0)
data=data.iloc[:,1:12]
input_data_set=data.drop(["label"],axis=1)
output_data_set=data["label"]
print(input_data_set,"\n")
print(output_data_set,"\n")

k_means_fit=KMeans(n_clusters=5,init="random",max_iter=1000)
k_means_fit.fit(input_data_set)

print("\nK-means Clustering - confusion Matrix\n\n",pd.crosstab(output_data_set,k_means_fit.labels_,rownames=["Actuall"],colnames=["Predicted"]))

print("\nSilhouette-score:%0.3f" %silhouette_score(input_data_set,k_means_fit.labels_,metric='euclidean'))
