import math
import sys
eps = 0.001
K = int(sys.argv[1])
iter = 200 if len(sys.argv) == 3 else int(sys.argv[2])

class Point:
    def __init__(self, data=[]) -> None:
        self.data = data

    def __str__(self) -> str:
        return self.data.__str__()

    def distance(self, other):
        sum = 0
        for j in range(len(self.data)):
            sum += (self.data[j] - other.data[j]) ** 2
        return math.sqrt(sum)

    def match_cluster(self, list_of_clusters):
        min_distance = self.distance(list_of_clusters[0].centroid)
        nearest_cluster = list_of_clusters[0]
        for cur_cluster in list_of_clusters:
            distance = self.distance(cur_cluster.centroid)
            if distance < min_distance:
                min_distance = distance
                nearest_cluster = cur_cluster
        return nearest_cluster


class Cluster:
    def __init__(self, centroid) -> None:
        self.centroid = centroid
        self.points = []

    def __str__(self) -> str:
        return self.centroid.__str__()

    def add_point(self, point):
        self.points.append(point)

    def clear(self):
        self.points.clear()

    def update_centroid(self):
        mean = [0] * len(self.centroid.data)
        for i in range(len(self.centroid.data)):
            for p in self.points:
                mean[i] += p.data[i]
            mean[i] /= len(self.points)
        new_centroid = Point(mean)
        converged = new_centroid.distance(self.centroid) < eps
        self.centroid = new_centroid
        return converged


def read_from_file():
    list_of_points = []
    f = open(sys.argv.pop(), "r")
    for line in f:
        current_line = line.split(",")
        current_line = [float(x) for x in current_line]
        current_point = Point(current_line)
        list_of_points.append(current_point)
    return list_of_points


# need to add sanity check and termination conditions

datapoints = read_from_file()
clusters = []
for i in range(K):
    clusters.append(Cluster(datapoints[i]))
i = 0
converged = []
for i in range(iter):
    for point in datapoints: #adding each point to the nearest cluster's points list
        nearest_cluster = point.match_cluster(clusters)
        nearest_cluster.add_point(point)
    for cluster in clusters:
        converged.append(cluster.update_centroid())
        cluster.points.clear()
    if False not in converged:
        break
    else:
        converged.clear()
print([x.centroid.data for x in clusters])