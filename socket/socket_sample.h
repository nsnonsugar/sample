#ifndef SOCKET_SAMPLE_H_
#define SOCKET_SAMPLE_H_
#include <time.h>

// ソケット通信で受け渡しするデータ
typedef struct{
    struct tm date;
    int num;
    char text[256];
} SampleSocketData;

// ポート番号
// とりあえず適当に9000番を使用
#define PORT_NUM 9000

#endif // SOCKET_SAMPLE_H_