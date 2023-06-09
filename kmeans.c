#include <stdio.h>
#include <math.h>
#include <stdlib.h>
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
Cluster *matchCluster(Point point, Cluster clusters[], int k);
PointList readInput(void);
void handleError(void);

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
    if (list->pointsArr == NULL) {
        handleError();
    }
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
    double *mean = NULL;
    Point newCentroid = {NULL, 0};
    int converged, i, j;
    
    mean = calloc(cluster->centroid.length, sizeof(double));
    if (mean == NULL) {
        handleError();
    }
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
    for (i = 0; i < point.length - 1; i++) {
        printf("%.4f,", point.data[i]);
    }
    printf("%.4f\n", point.data[i]);
}

Cluster *matchCluster(Point point, Cluster clusters[], int k) {
    int i;
    double minDistance = distance(point, clusters[0].centroid);
    Cluster *nearestCluster = clusters;
    for (i = 1; i < k; i++) {
        double currDistance = distance(point, clusters[i].centroid);
        if (currDistance < minDistance){
            minDistance = currDistance;
            nearestCluster = &clusters[i];
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
        data = NULL;
        dLength = 0;
        while (ptr[currLen] != '\0') {
            while (ptr[currLen] != ',' && ptr[currLen] != '\0') {
                currLen++;
            }
            strncpy(currNum, ptr, currLen);
            currNum[currLen] = '\0';
            if (dLength == 0) {
                data = (double*)malloc(sizeof(double));
            } else {
                data = (double*)realloc(data, (dLength + 1)*sizeof(double));
            }
            if (data == NULL) {
                handleError();
            }
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

void handleError(void) {
    printf("An Error Has Occurred\n");
    exit(1);
}

int main(int argc, char *argv[]) {
    PointList points = readInput();
    Cluster *clusters;
    int k = atoi(argv[1]);
    int iter = 200, i, j, converged = 1;
    if (argc >=3) {
        iter = atoi(argv[2]);
    }
    if (k <= 1 || k >= points.length) {
        printf("Invalid number of clusters!\n");
        clearPointList(&points);
        return 1;
    }
    if (iter <= 1 || iter >= 1000) {
        printf("Invalid maximum iteration!\n");
        clearPointList(&points);
        return 1;
    }
    clusters = (Cluster *)calloc(k, sizeof(Cluster));
    if (clusters == NULL) {
        handleError();
    }
    for (i = 0; i < k; i++) {
        clusters[i].centroid = points.pointsArr[i];

    }
    for (i = 0; i < iter; i++) {
        for (j = 0; j < points.length; j++) {
            Cluster *nearestCluster = matchCluster(points.pointsArr[j], clusters, k);
            addPointToList(&nearestCluster->points, points.pointsArr[j]);
        }
        for (j = 0; j < k; j++) {
            converged = updateCentroid(&clusters[j]) && converged;
            clearPointList(&clusters[j].points);
        }
        if (converged) {
            break;
        } else {
            converged = 1;
        }
    }
    for (i = 0; i < k; i++) {
        printPoint(clusters[i].centroid);
    }
    free(clusters);
    for (i = 0; i < points.length; i++) {
        free(points.pointsArr[i].data);
    }
    clearPointList(&points);
    return 0;
}
