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

// 通信で渡すデータを作成する関数
// 適当にデータを入れている
SampleSocketData create_send_data(void)
{
    static int cnt = 1;
    char *text = "Test Text";
    SampleSocketData data = {0};
    time_t timer = time(NULL);
    struct tm* date = localtime(&timer);

    data.date = *date;
    data.num = cnt;
    sprintf(data.text, "%s number : %d", text, cnt);
    ++cnt;

    return data;
}

int main()
{
    char destination_ip[80] = {0};
    unsigned short port = PORT_NUM;
    int client_socket = 0;
    struct sockaddr_in client_addr = {0};

    // 送信先アドレスの入力 0を入力した場合localhostに送信する
    printf("Connect to ? : (name or IP address)　if input 0, send to local host.");
    scanf("%s", destination_ip);
    if(strcmp(destination_ip, "0") == 0){
        strncpy(destination_ip, "127.0.0.1", sizeof(destination_ip));
    }


    // struct sockaddr_inにデータをセット
    client_addr.sin_family = AF_INET; // 常にAF_INETをセットする
    client_addr.sin_port = htons(port); // ビッグエンディアンに変換
    client_addr.sin_addr.s_addr = inet_addr(destination_ip); //送信先のアドレス
 
    // ソケットの生成
    errno = 0;
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(client_socket == -1){
        printf("socket call failed. errno : %d\n", errno);
        return -1;
    }

    // 接続
    printf("Trying to connect to %s: \n", destination_ip);
    int ret = connect(client_socket, (struct sockaddr*) &client_addr, sizeof(client_addr));
    if(ret == -1){
        printf("connect call failed. errno : %d\n", errno);
        return -1;
    }


    // パケット送信
    for(int i=0; i<10; i++) {
        int sent_byte = 0;
        // 送信データを生成
        SampleSocketData send_data = create_send_data();

        printf("sending\n");
        errno = 0;
        sent_byte = send(client_socket, &send_data, sizeof(send_data), 0);
        if(sent_byte == -1){
            printf("send call failed. errno : %d\n", errno);
        }

        sleep(1);
    }

    // ソケット終了
    int status = close(client_socket);
    if(status == -1){
        printf("close call failed. errno : %d\n", errno);
        return -1;
    }

    return 0;
}


