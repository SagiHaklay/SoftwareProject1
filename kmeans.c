#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

typedef struct PointType {
    double *data;
    int length;
} Point;

typedef struct ListType {
    Point *pointsArr;
    int length;
} PointList;

typedef struct ClusterType {
    Point centroid;
    PointList points;
} Cluster;

double eps = 0.001;

double distance(Point p1, Point p2);
void addPointToList(PointList*, Point);
void printPointList(PointList);
void clearPointList(PointList*);
int updateCentroid(Cluster*);
void printPoint(Point);
Cluster matchCluster(Point point, Cluster clusters[], int k);
PointList readInput(void);

double distance(Point p1, Point p2) {
    double sum = 0.0;
    for (int i = 0; i < p1.length; i++){
        sum += pow(p1.data[i] - p2.data[i], 2);
    }
    return sqrt(sum);
}

void addPointToList(PointList *list, Point point) {
    if (list->length == 0) {
        list->pointsArr = (Point *)malloc(sizeof(Point));
    } else {
        list->pointsArr = (Point *)realloc(list->pointsArr, (list->length + 1)*sizeof(Point));
    }
    assert(list->pointsArr != NULL);
    list->pointsArr[list->length] = point;
    list->length++;
}

void printPointList(PointList list) {
    for (int i = 0; i < list.length; i++){
        printPoint(list.pointsArr[i]);
    }
}

void clearPointList(PointList* list) {
    free(list->pointsArr);
    list->pointsArr = NULL;
    list->length = 0;
}

int updateCentroid(Cluster* cluster){
    double *mean = calloc(cluster->centroid.length, sizeof(double));
    assert(mean != NULL);
    for (int i = 0; i < cluster->centroid.length; i++) {
        for (int j = 0; j < cluster->points.length; j++) {
            mean[i] += cluster->points.pointsArr[j].data[i];
        }
        mean[i] /= cluster->points.length;
    }
    Point newCentroid = {mean, cluster->centroid.length};
    int converged = distance(cluster->centroid, newCentroid) < eps;
    cluster->centroid = newCentroid;
    return converged;
}

void printPoint(Point point) {
    for (int i = 0; i < point.length; i++) {
        printf("%.4f ", point.data[i]);
    }
    printf("\n");
}

Cluster matchCluster(Point point, Cluster clusters[], int k) {
    double minDistance = distance(point, clusters[0].centroid);
    Cluster nearestCluster = clusters[0];
    for (int i = 1; i < k; i++) {
        double currDistance = distance(point, clusters[i].centroid);
        if (currDistance < minDistance){
            minDistance = currDistance;
            nearestCluster = clusters[i];
        }
    }
    return nearestCluster;
}

PointList readInput(void) {
    PointList list = {NULL, 0};
    Point p = {NULL, 0};
    char *line;
    
}

int main(int argc, char *argv[]) {
    
    return 0;
}
