#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

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
    int i;
    for (i = 0; i < p1.length; i++){
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
    int i;
    for (i = 0; i < list.length; i++){
        printPoint(list.pointsArr[i]);
    }
}

void clearPointList(PointList* list) {
    free(list->pointsArr);
    list->pointsArr = NULL;
    list->length = 0;
}

int updateCentroid(Cluster* cluster){
    Point newCentroid = {NULL, 0};
    int converged, i, j;
    double *mean = calloc(cluster->centroid.length, sizeof(double));
    assert(mean != NULL);
    for (i = 0; i < cluster->centroid.length; i++) {
        for (j = 0; j < cluster->points.length; j++) {
            mean[i] += cluster->points.pointsArr[j].data[i];
        }
        mean[i] /= cluster->points.length;
    }
    newCentroid.data = mean;
    newCentroid.length = cluster->centroid.length;
    converged = distance(cluster->centroid, newCentroid) < eps;
    cluster->centroid = newCentroid;
    return converged;
}

void printPoint(Point point) {
    int i;
    for (i = 0; i < point.length; i++) {
        printf("%.4f ", point.data[i]);
    }
    printf("\n");
}

Cluster matchCluster(Point point, Cluster clusters[], int k) {
    int i;
    double minDistance = distance(point, clusters[0].centroid);
    Cluster nearestCluster = clusters[0];
    for (i = 1; i < k; i++) {
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
    char line[100], *ptr, currNum[10];
    int dLength = 0, currLen = 0;
    double *data = NULL;
    while (scanf("%s\n", line) > 0) {
        ptr = line;
        currLen = 0;
        while (ptr[currLen] != '\0') {
            while (ptr[currLen] != ',' && ptr[currLen] != '\0') {
                currLen++;
            }
            strncpy(currNum, ptr, currLen);
            if (dLength == 0) {
                data = (double*)malloc(sizeof(double));
            } else {
                data = (double*)realloc(data, (dLength + 1)*sizeof(double));
            }
            assert(data != NULL);
            data[dLength] = atof(currNum);
            dLength++;
            if (ptr[currLen] == ','){
                ptr += currLen + 1;
                currLen = 0;
            }
            
        }
        p.data = data;
        p.length = dLength;
        addPointToList(&list, p);
        
    }
    return list;
}

int main(void) {
    PointList points = readInput();
    printPointList(points);
    return 0;
}
