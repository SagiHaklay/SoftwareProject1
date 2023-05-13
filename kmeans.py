import math

class Point:
    def __init__(self, data=[]) -> None:
        self.data = data

    def distance(self, other):
        sum = 0
        for i in range(self.get_length()):
            sum += (self.data[i] - other.data[i])**2
        return math.sqrt(sum)
    
    def get_length(self):
        return len(self.data)
    
    def get_index(self, index):
        return self.data[index]
    
class Cluster:
    def __init__(self, centroid) -> None:
        self.centroid = centroid
        self.points = []

    def add_point(self, point):
        self.points.append(point)

    def clear(self):
        self.points = []

    def update_centroid(self, eps):
        mean = [0 for i in range(self.centroid.get_length())]
        for i in range(self.centroid.get_length()):
            for p in self.points:
                mean[i] += p.get_index(i)
            mean[i] /= len(self.points)
        new_centroid = Point(mean)
        converged = new_centroid.distance(self.centroid) < eps
        self.centroid = new_centroid
        return converged
