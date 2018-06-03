#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "socket_sample.h"


int main()
{
    unsigned short port = PORT_NUM;
    int server_soket = 0;
    int client_soket = 0;

    struct sockaddr_in server_addr = {0};
    struct sockaddr_in client_addr = {0};
    unsigned int client_addrSize = sizeof(client_addr);
    int ret = 0;

    // struct sockaddr_inにデータをセット
    server_addr.sin_family = AF_INET; // 常にAF_INETをセットする
    server_addr.sin_port = htons(port); // ポート番号をビッグエンディアンに変換
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // ワイルドカードアドレスを使用

    // ソケットの生成
    errno = 0;
    server_soket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_soket == -1){
        printf("socket call failed. errno : %d\n", errno);
        return -1;
    }

    // ソケットに名前をつける
    errno = 0;
    ret = bind(server_soket, (struct sockaddr*) &server_addr, sizeof(server_addr));
    if(ret == -1){
        printf("bind call failed. errno : %d\n", errno);
        return -1;
    }


    // 接続の許可
    errno = 0;
    ret = listen(server_soket, 1); // 同時通信はとりあえず1台まで
    if(ret == -1){
        printf("listen call failed. errno : %d\n", errno);
        return -1;
    }

    // 接続待ち
    printf("Waiting for connection\n");
    int retry_cnt = 0;
    while(retry_cnt < 3){
        errno = 0;
        client_soket = accept(server_soket, (struct sockaddr*)&client_addr, &client_addrSize);
        // 失敗ならリトライ
        if(client_soket == -1){
             printf("accept call failed. count : %d errno : %d\n", retry_cnt, errno);
            ++retry_cnt;
        }else{
            break;
        }

        if(retry_cnt < 3){
            // 少し待つ
            sleep(1);
        }
    }

    if(client_soket == -1){
        return -1;
    }

    printf("Connected from %s\n", inet_ntoa(client_addr.sin_addr));

    // パケット受信
    while(1){
        int numrcv = 0;
        SampleSocketData receive_data = {0};

        errno = 0;
        numrcv = recv(client_soket, &receive_data, sizeof(receive_data), 0);
        if(numrcv == 0){
            // 接続相手が正しくシャットダウンを実行した場合0が来るので抜ける
            break;
        }

        if(numrcv == -1){
            printf("recv call failed. errno : %d\n", errno);
            break;
        }

        printf("received: %04d/%02d/%02d %02d:%02d:%02d %d %s\n",
                receive_data.date.tm_year + 1900,
                receive_data.date.tm_mon + 1,
                receive_data.date.tm_mday,
                receive_data.date.tm_hour,
                receive_data.date.tm_min,
                receive_data.date.tm_sec,
                receive_data.num,
                receive_data.text);
    }

    // パケット送受信用ソケットを終了
    errno = 0;
    int status = close(client_soket);
    if(status == -1){
        printf("client_soket close call failed. errno : %d\n", errno);
    }

    // 接続要求待ち受け用ソケットを終了
    errno = 0;
    status = close(server_soket);
    if(status == -1){
        printf("server_soket close call failed. errno : %d\n", errno);
    }

    return 0;
}
