#include "stdio.h"
#include "fcntl.h"
#include "sys/types.h"
#include "sys/stat.h"
#include <sys/ioctl.h>
#include <termios.h>

#include <unistd.h>
#include <stdlib.h>

#define SERIAL_PORT "/dev/ttyACM0"
//#define BUFFERSIZE 256
#define BUFFERSIZE 64

int main(){
    int fd;                            // ファイルディスクリプタ
    struct termios tio;                // シリアル通信設定

    int len;
    u_int8_t aTxBuffer[BUFFERSIZE];    // バッファ
    u_int8_t aRxBuffer[BUFFERSIZE];    // バッファ

    int microsecond = 1 * 1000 * 1000;

    fd = open(SERIAL_PORT, O_RDWR);    // デバイスをオープンする
    if (fd < 0) {
        printf("open error\n");
        return -1;
    }

    tio.c_cflag += CREAD;               // 受信有効
    tio.c_cflag += CLOCAL;              // ローカルライン（モデム制御なし）
    tio.c_cflag += CS8;                 // データビット:8bit
    tio.c_cflag += 0;                   // ストップビット:1bit
    tio.c_cflag += 0;                   // パリティ:None

    cfmakeraw(&tio);                    // RAWモード
    tcsetattr( fd, TCSANOW, &tio );     // デバイスに設定を行う
    ioctl(fd, TCSETS, &tio);            // ポートの設定を有効にする

    while(1){
        for(int i = 0; i < BUFFERSIZE; i++){
            aTxBuffer[i] = rand();
        }
        write(fd, aTxBuffer, BUFFERSIZE);

        len = read(fd, aRxBuffer, sizeof(aRxBuffer));

        for(int i = 0; i < BUFFERSIZE/16; i++){
            for(int j = 0; j < 16; j++){
                if(aTxBuffer[i*16+j] != aRxBuffer[i*16+j]){
                    printf("Error %i, %02x, %02x ", i*16+j, aTxBuffer[i*16+j], aRxBuffer[i*16+j]);
                } else {
                    printf("%02x ",aTxBuffer[i*16+j]);
                }
            }
            printf("\n");
        }
        printf("\n");
      
        usleep(microsecond);

    }
    return 0;
}