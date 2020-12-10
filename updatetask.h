#ifndef UPDATETASK_H
#define UPDATETASK_H

#include <QThread>
#include<QDebug>
#include<QUdpSocket>
#include<QFile>
#include<QByteArray>
#include<QCryptographicHash>
#include<QDebug>
#include"commonfun.h"
#include"cJSON.h"
#define MAXLEN 512

typedef struct frame
{
    unsigned long fileSize;
    unsigned int frameSize;
    QString md5;
    QByteArray payload;
    char payload2[MAXLEN];
    unsigned short len;
    unsigned char token;
    unsigned int frameNo;
}FRAME;

typedef struct frame_transfer
{
    char payload[MAXLEN*2];
    unsigned long transferSize;
    unsigned long transferNo;
    QString state;
    int len;
}FRAMETRANSFER;



typedef struct frame_start
{
    unsigned long fileSize;
    unsigned long frameSize;
    QString state;
}FRAMESTART;


typedef struct frame_end
{
    QString md5;
    QString state;
}FRAMEEND;




class updatetask : public QThread
{
    Q_OBJECT

public:
    updatetask();
    void run();
    QList<FRAME>frame_list;
    QList<FRAMETRANSFER>frame_transfer_list;
    FRAMESTART frame_start;
    FRAMEEND frame_end;
    int lost_frame_no;
    bool lost_frame;


    QUdpSocket *udp_socket;
    AC_FRAME st_Temp;
    void parse_file();
    void read_file();
    void read_file2();
    QString make_md5(QString file);
    long make_filesize(QString file);
    void parse_frame(char *data,int size);
    void send_frame(int type, int cmd,int execmd,int datalen,unsigned char * buf);
    void test_frame();
    void parse_data(AC_FRAME *pr);
    void update_soft(cJSON *json);
signals:

public slots:
    void data_received();

};

#endif // UPDATETASK_H
