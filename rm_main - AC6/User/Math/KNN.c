#include "stdio.h"
#include "arm_math.h"
#include "KNN.h"
#include "math.h"


//#define KNN_INFINITY 0x3f3f3f3f
#define CUSTOM_INFINITY 1.0e+308
#define N 75  // 训练数据点的数量
#define M 12    // 特征数量
#define K 8    // 选择最近邻的数量


// 存储训练数据和标签

// 结构体用于存储距离和对应的标签


float training_data[N][M] = {
                            {0.17,0.99,-6.25,0.03,0.20,-0.15,0.21,-0.09,0.06,0.13,56.67,57.30},
                            {0.19,1.01,-6.25,0.05,0.20,-0.08,0.21,-0.03,0.07,0.12,56.30,56.97},
                            {0.21,1.03,-6.25,0.04,0.20,-0.04,0.21,-0.14,0.07,0.11,57.12,57.32},
                            {0.22,1.06,-6.25,0.01,0.20,-0.12,0.21,-0.20,0.07,0.11,53.60,52.53},
                            {0.24,1.12,-6.25,-0.01,0.19,-0.29,0.20,-0.31,0.07,0.09,42.21,44.08},
                            {0.26,1.21,-6.25,0.00,0.19,-0.34,0.20,-0.41,0.07,-0.02,28.86,31.19},
                            {0.28,1.29,-6.25,0.03,0.18,-0.46,0.19,-0.49,0.07,-0.08,27.78,27.21},
                            {0.31,1.34,-6.25,0.01,0.17,-0.49,0.18,-0.39,0.07,-0.06,22.67,22.67},
                            {0.32,1.36,-6.25,0.03,0.16,-0.47,0.17,-0.40,0.06,-0.06,24.11,23.15},
                            {0.35,1.37,-6.25,0.04,0.15,-0.70,0.16,-0.67,0.06,-0.09,20.43,19.96},
                            {0.37,1.37,-6.25,0.04,0.14,-0.71,0.15,-0.61,0.06,-0.04,20.73,19.74},
                            {0.39,1.35,-6.25,-0.01,0.14,-0.23,0.15,-0.49,0.06,0.06,24.19,18.90},
                            {0.41,1.29,-6.25,0.05,0.14,0.08,0.15,-0.57,0.07,0.19,82.47,72.83},
                            {0.43,1.36,-6.25,0.03,0.13,-0.87,0.14,-0.74,0.08,0.43,38.69,42.40},
                            {0.45,1.51,-6.25,-0.01,0.12,-0.83,0.13,-1.13,0.09,0.60,21.47,36.62},
                            {0.48,1.53,-6.24,0.15,0.11,-1.51,0.11,-1.21,0.10,0.58,42.88,45.11},
                            {0.50,1.53,-6.24,0.09,0.09,-1.43,0.09,-1.22,0.11,0.64,48.00,51.81},
                            {0.53,1.59,-6.26,0.04,0.06,-1.58,0.07,-1.55,0.12,0.51,55.47,60.11},
                            {0.55,1.66,-6.25,0.08,0.04,-1.87,0.04,-1.87,0.12,0.27,63.10,67.96},
                            {0.58,1.71,-6.24,0.14,-0.00,-2.06,0.00,-1.93,0.11,0.06,66.10,74.88},
                            {0.60,1.71,-6.24,0.11,-0.03,-1.86,-0.03,-1.91,0.11,-0.06,69.87,72.88},
                            {0.63,1.70,-6.24,0.06,-0.06,-1.82,-0.06,-1.95,0.11,-0.20,73.05,76.03},
                            {0.65,1.67,-6.24,0.06,-0.09,-1.73,-0.09,-1.80,0.10,-0.31,74.15,78.74},
                            {0.68,1.62,-6.24,0.04,-0.12,-1.50,-0.11,-1.54,0.09,-0.39,74.58,77.92}, 
                            {0.70,1.57,-6.24,0.01,-0.14,-1.37,-0.13,-1.29,0.09,-0.41,74.87,76.53},//下台阶
                            

                            {0.54,0.22,-6.24,0.06,-0.10,0.15,-0.09,0.15,0.03,0.09,57.47,52.95},
                            {0.58,0.11,-6.23,-0.01,-0.08,0.12,-0.07,-0.01,0.05,0.01,57.82,53.08},
                            {0.59,0.10,-6.23,-0.02,-0.07,0.10,-0.07,0.03,0.05,0.01,57.26,53.07},
                            {0.59,0.10,-6.23,-0.02,-0.07,0.14,-0.07,0.07,0.05,0.00,57.49,53.47},
                            {0.59,0.09,-6.23,-0.01,-0.07,0.15,-0.07,0.14,0.05,-0.01,57.62,52.76},
                            {0.59,0.08,-6.23,0.01,-0.07,0.13,-0.06,0.05,0.05,-0.01,57.72,52.34},
                            {0.59,0.08,-6.23,0.01,-0.07,0.08,-0.06,0.09,0.05,-0.01,56.88,52.54},
                            {0.59,0.08,-6.23,0.01,-0.07,0.06,-0.06,0.13,0.05,-0.01,56.40,52.41},
                            {0.59,0.08,-6.23,0.02,-0.07,0.02,-0.06,0.09,0.05,-0.02,56.52,52.13},
                            {0.59,0.07,-6.23,0.02,-0.07,0.05,-0.06,0.06,0.05,-0.03,56.76,50.90},
                            {0.59,0.07,-6.23,0.02,-0.06,0.08,-0.06,0.04,0.05,-0.03,56.40,52.87},
                            {0.60,0.06,-6.23,0.02,-0.06,0.03,-0.06,0.09,0.04,-0.03,56.28,52.49},
                            {0.60,0.05,-6.23,0.00,-0.06,0.13,-0.06,0.05,0.04,-0.02,56.46,51.63},
                            {0.60,0.04,-6.23,-0.00,-0.06,0.13,-0.05,0.12,0.04,-0.01,55.92,52.34},
                            {0.60,0.03,-6.23,0.00,-0.06,0.13,-0.05,0.07,0.04,0.00,56.07,53.19},
                            {0.60,0.03,-6.23,0.01,-0.05,0.16,-0.05,0.08,0.04,0.02,56.79,51.72},
                            {0.60,0.04,-6.23,-0.01,-0.05,0.24,-0.05,0.17,0.05,0.02,51.14,46.15},
                            {0.60,0.03,-6.23,-0.02,-0.05,0.20,-0.05,0.23,0.05,0.02,33.93,32.27},
                            {0.60,0.02,-6.23,-0.02,-0.05,0.24,-0.04,0.25,0.05,0.01,29.08,28.11},
                            {0.60,0.01,-6.23,-0.01,-0.05,0.34,-0.04,0.33,0.05,0.00,26.44,25.53},
                            {0.60,0.01,-6.23,-0.00,-0.04,0.36,-0.04,0.39,0.05,-0.01,24.03,24.23},
                            {0.60,0.00,-6.23,-0.01,-0.03,0.51,-0.03,0.46,0.04,-0.02,24.76,25.47},
                            {0.60,0.00,-6.23,-0.01,-0.03,0.63,-0.02,0.62,0.04,-0.04,26.49,26.53},
                            {0.60,0.00,-6.23,-0.01,-0.02,0.76,-0.02,0.77,0.04,-0.05,28.50,27.73},
                            {0.60,0.02,-6.23,-0.02,-0.01,0.84,-0.00,0.81,0.04,-0.05,30.69,28.95},//平地
                            
                            {0.28,-0.00,-6.22,-0.08,0.02,-0.03,0.00,-0.07,0.01,-0.03,6.91,6.62},
                            {0.27,0.02,-6.23,-0.00,-0.02,0.17,-0.01,0.05,-0.01,-0.05,54.11,54.67},
                            {0.27,0.04,-6.23,-0.00,-0.01,0.04,-0.01,0.13,-0.01,-0.02,39.07,35.97},
                            {0.27,0.06,-6.23,-0.00,-0.02,-0.14,-0.01,-0.02,-0.01,-0.05,16.01,23.64},
                            {0.27,0.06,-6.23,-0.07,-0.01,0.31,-0.01,-0.19,-0.01,0.03,10.30,9.88},
                            {0.27,0.07,-6.23,0.07,-0.01,0.13,-0.02,-0.14,-0.01,-0.00,19.47,35.15},
                            {0.27,0.07,-6.23,0.01,-0.01,-0.02,-0.02,-0.05,-0.01,-0.03,49.53,41.89},
                            {0.27,0.06,-6.23,0.03,-0.01,0.06,-0.01,0.11,-0.01,0.01,16.60,33.07},
                            {0.27,0.05,-6.22,-0.01,-0.01,0.02,-0.01,-0.11,-0.01,0.03,11.69,8.88},
                            {0.27,0.04,-6.22,0.01,-0.01,0.01,-0.01,-0.08,-0.01,0.09,10.11,10.45},
                            {0.27,0.04,-6.24,0.03,-0.01,0.04,-0.01,-0.03,-0.00,0.08,16.82,10.58},
                            {0.27,0.04,-6.24,0.03,-0.01,0.13,-0.01,0.17,-0.00,0.09,16.13,11.45},
                            {0.28,0.03,-6.22,0.09,-0.01,0.06,-0.01,0.09,-0.00,0.08,4.65,12.42},
                            {0.28,0.03,-6.22,0.10,-0.01,0.12,-0.01,0.08,-0.00,0.10,7.14,8.67},
                            {0.28,0.02,-6.22,0.03,-0.00,0.22,-0.00,0.11,0.00,0.14,3.14,6.60},
                            {0.28,0.01,-6.22,0.03,0.00,0.16,-0.00,0.11,0.00,0.13,2.00,4.85},
                            {0.28,0.01,-6.22,0.03,0.00,0.14,-0.00,0.00,0.01,0.11,1.53,2.37},
                            {0.28,0.01,-6.22,0.05,0.00,0.18,0.00,0.07,0.01,0.12,-0.49,1.02},
                            {0.28,0.00,-6.22,0.04,0.01,0.15,0.00,0.12,0.01,0.15,-1.11,0.23},
                            {0.28,0.00,-6.22,0.04,0.01,0.21,0.00,0.13,0.01,0.14,0.06,0.68},
                            {0.28,0.00,-6.22,0.02,0.01,0.25,0.01,0.14,0.01,0.12,0.80,1.75},
                            {0.28,0.00,-6.22,0.01,0.01,0.20,0.01,0.07,0.01,0.09,2.03,1.78},
                            {0.28,0.00,-6.22,0.01,0.01,0.08,0.01,0.05,0.01,0.05,2.94,2.51},
                            {0.28,0.00,-6.22,0.02,0.01,0.13,0.01,0.05,0.01,0.04,3.73,3.33},
                            {0.28,0.00,-6.22,0.01,0.02,0.07,0.01,0.02,0.02,0.03,4.13,3.52},


  }
//台阶下地
;
int labels[N] ={0,0,0,0,0,
                0,0,0,0,0,
                0,0,0,0,0,
                0,0,0,0,0,
                0,0,0,0,0,//下台阶
                
                0,0,0,0,0,
                0,0,0,0,0,
                0,0,0,0,0,
                0,0,0,0,0,
                0,0,0,0,0,//平地
    
                1,1,1,1,1,
                1,1,1,1,1,
                1,1,1,1,1,
                1,1,1,1,1,
                1,1,1,1,1,

                
};



// 计算两个数据点之间的欧氏距离
float calculate_distance(float *data1, float *data2, int length) {
    double distance = 0.0;
    int i ;
        for ( i = 0; i < length; i++) {
        distance += (data1[i] - data2[i]) * (data1[i] - data2[i]);
//        distance += powf((data1[i] - data2[i]),2);
    }
    return sqrt(distance);
}

// 计算两个数据点之间的欧氏距离
float calculate_distance_Manhattan(float *data1, float *data2, int length) {
    float distance = 0.0;
    int i ;
        for ( i = 0; i < length; i++) {
        distance += fabs(data1[i] - data2[i]);
//        distance += powf((data1[i] - data2[i]),2);
    }
    return distance;
}



// 简单的插入排序找到最小的K个距离
void find_nearest_neighbors(Neighbor *neighbors, int k, float distance, int label) {
	int i ;
    for ( i = 0; i < k; i++) {
        if (distance < neighbors[i].distance) {
            // 插入当前的邻居
            int j;
            for ( j = k - 1; j > i; j--) {
                neighbors[j] = neighbors[j - 1];
            }
            neighbors[i].distance = distance;
            neighbors[i].label = label;
            break;
        }
    }
}

int knn_classify(float *input_data, int k) {
    Neighbor neighbors[K];
    int i;
    // 初始化距离为最大值
    for (i = 0; i < K; i++) {
        neighbors[i].distance = CUSTOM_INFINITY;
        neighbors[i].label = -1;
    }

    // 计算所有训练数据点的距离并找到最近邻
    int j;
    for (j = 0; j < N; j++) {  // 修正循环条件
//        float distance = calculate_distance(input_data, training_data[j], M);
        float distance = calculate_distance_Manhattan(input_data, training_data[j], M);//Manhattan距离
        find_nearest_neighbors(neighbors, K, distance, labels[j]);
    }

    // 投票分类
    int vote_count[2] = {0};  // 假设标签只有0和1
    for (i = 0; i < K; i++) {
        if (neighbors[i].label != -1) {
            vote_count[neighbors[i].label]++;
        }
    }

    // 返回投票最多的标签
    return (vote_count[0] > vote_count[1]) ? 0 : 1;
}


//float input_data[M] = {1000,10,20};
float input_data[M] ;

//int main(void){

//int state = knn_classify(input_data, K);
//if (state == 1) {
//    printf("1");
//} else {
//    printf("0");// 机器人接地
//}

//}
