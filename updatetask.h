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

#define MAXLEN 512

typedef struct frame
{
    int fileSize;
    QString md5;
    QByteArray payload;
    char payload2[MAXLEN];
    unsigned short len;
    unsigned char token;
}FRAME;


class updatetask : public QThread
{
    Q_OBJECT

public:
    updatetask();
    void run();
    QList<FRAME>frame_list;
    QUdpSocket *udp_socket;
    AC_FRAME st_Temp;
    void parse_file();
    void read_file();
    QString make_md5(QString file);
    long make_filesize(QString file);
    void send_frame(int type, int cmd,int execmd,int datalen,unsigned char * buf);
    void test_frame();
signals:

public slots:
    void data_received();

};

#endif // UPDATETASK_H
