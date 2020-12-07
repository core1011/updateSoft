#include "updatetask.h"

updatetask::updatetask()
{
    udp_socket = new QUdpSocket(this);
    udp_socket->bind(8001, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    connect(udp_socket, SIGNAL(readyRead()), this, SLOT(data_received()));
}
void updatetask::run()
{

    read_file();
    //test_frame();

    int size = frame_list.size();
    int len = 0;
    char buf[1024]={0};

    while (1)
    {
        FRAME tmp ;
        for(int i= 0;i<size;i++)
        {
            tmp = frame_list.at(i);
            QByteArray array =QByteArray(tmp.payload2,tmp.len);
            len = sprintf(buf,"{\"data\":{\"fileSize\":%d,\"payload\":\"%s\",\"md5\":\"%s\",\"token\":%d},\"meta\":{\"isMessage\":false,\"type\":\"updateSoft\"}}"
                          ,tmp.fileSize,array.toBase64().data(),tmp.md5.toStdString().c_str(),tmp.token);
            //qDebug()<<tmp.payload.size()<<len<<buf;
            //send_frame(COM_ETH,0x80,CMD_READ,len,(unsigned char *)buf);

            send_frame(COM_ETH,0xD5,CMD_W_SUCCESS,len,(unsigned char *)buf);

            usleep(1000*200);
        }
        break;
    }
}

void updatetask::read_file()
{
    FILE * fp = NULL;
    int n=0,sum=0;
    char buf[MAXLEN];
    //char *file0 = "/ll/tps4";
    char *file0 = "/ll/tps4.zip";
    char *file1 = "/ll/tps4.tar.gz.tmp";

    fp = fopen(file0,"r+");
    if(fp ==NULL)
        return ;

    sum = make_filesize(QString(file0));

    QString md5 = make_md5(QString(file0));
    //read

    FRAME tmp0;
    tmp0.len=0;
    tmp0.md5 = "";
    tmp0.payload.resize(0);
    tmp0.token = 0;
    tmp0.fileSize = sum;
    frame_list.append(tmp0);

    while((n = fread( buf,1, MAXLEN,fp))>0)
    {
        FRAME tmp1;
        tmp1.token =1;
        memset(tmp1.payload2,0x00,MAXLEN);
        memcpy(tmp1.payload2,buf,n);
        tmp1.len = n;
        tmp1.fileSize = sum;
        tmp1.payload = buf;
        tmp1.payload.resize(n);

        frame_list.append(tmp1);


        qDebug()<<n<<sum<<tmp1.token;
    }

    FRAME tmp2;
    tmp2.len=0;
    tmp2.md5 = md5;
    tmp2.payload.resize(0);
    tmp2.token = 2;
    tmp2.fileSize = sum;
    frame_list.append(tmp2);


    qDebug()<<" md5:"<<md5 <<frame_list.size();
    fclose(fp);

//write
//    fp = fopen(file1,"w+");
//    if(fp ==NULL)
//        return ;
//    for(int i=0;i<frame_list.size();i++)
//    {
//        FRAME tmp;
//        tmp = frame_list.at(i);

//        fwrite(tmp.payload2,1,tmp.len,fp);
//    }
//    fclose(fp);

//    QString md55  = make_md5(file1);
//    qDebug()<<md55;

}

QString updatetask::make_md5(QString file)
{
    QFile qFile(file);

    if (!qFile.open(QIODevice::ReadOnly))
        return NULL;

    char data[MAXLEN]={0};
    int size = qFile.size();
    QByteArray ba = QCryptographicHash::hash(qFile.readAll(), QCryptographicHash::Md5);
    QString md5string  = QString(ba.toHex());
    qFile.close();
    return md5string;
}

long updatetask::make_filesize(QString file)
{
    QFile qFile(file);

    if (!qFile.open(QIODevice::ReadOnly))
        return 0;

    qFile.close();
    return qFile.size();
}

void updatetask::test_frame()
{
    char buf[1024]={0};
    int len;

    len = sprintf(buf,"{\"meta\":{\"type\":\"softVer\"}}");
    //send_frame(COM_ETH,0Xd5,0x88,len,(unsigned char *)buf);


    len = sprintf(buf,"{\"data\":{\"colorName\":\"蓝\",\"paInfo\":[{\"seqName\":\"三号\"}],\"customerCode\":\"ceshi1117\",\"orderNum\":\"30\",\"styleName\":\"ceshi1117\",\"hangerCode\":\"3919345\",\"styleCode\":\"ceshi1117\",\"customerName\":\"ceshi1117\",\"requireTime\":\"2020-11-23\",\"planNum\":\"30\",\"hgType\":\"返修衣架\",\"sizeName\":\"L\",\"orderCode\":\"201117-0001\",\"colorCode\":\"03\",\"sizeCode\":\"01\"},\"meta\":{\"type\":\"mq.realTime\"}}");
            //send_frame(COM_ETH,0x80,CMD_READ,len,(unsigned char *)buf);
            usleep(1000*200);

       //{"data":{"faultNumber":0,"planNum":30,"lastCostTime":"16","hgFinishNumber":2,"totalTime":18,"hangerCode":"3881768","totalStdAvgTime":0,"nextStation":"1101-03","finishNumber":2},"meta":{"type":"mq.lastCostTime"}}

    len = sprintf(buf,"{\"meta\":{\"type\":\"mq.lastCostTime\"},\"data\":{\"hangerCode\":\"800021\",\"lastCostTime\":\"10\",\"hgFinishNumber\":100,\"nextStation\":\"1101-02\",\"planNum\":\"200\",\"faultNumber\":5,\"totalTime\":222,\"totalStdAvgTime\":555}}");
    //send_frame(COM_ETH,0x80,CMD_READ,len,(unsigned char *)buf);
    usleep(1000*200);

    len = sprintf(buf,"{\"data\":{\"code\":\"W1004\",\"codeType\":\"E\",\"message\":\"重复出站\"},\"meta\":{\"type\":\"mq.stateCode\"}}");

    send_frame(COM_ETH,0x80,CMD_READ,len,(unsigned char *)buf);
    usleep(1000*2000);


    len = sprintf(buf,"{\"data\":{\"code\":\"W1004\",\"codeType\":\"W\",\"message\":\"重复出站\"},\"meta\":{\"type\":\"mq.stateCode\"}}");

    send_frame(COM_ETH,0x80,CMD_READ,len,(unsigned char *)buf);
    usleep(1000*200);
}

void updatetask::parse_file()
{
    QFile qFile("/ll/tps4");

    if (!qFile.open(QIODevice::ReadOnly))
        return;

    char data[MAXLEN]={0};
    int size = qFile.size();
    QByteArray ba = QCryptographicHash::hash(qFile.readAll(), QCryptographicHash::Md5);
    QString md5string  = QString(ba.toHex());
    //qDebug()<<size<<" md5:"<<md5string;
    qFile.seek(0);
    int count = size/MAXLEN;
    if(size%MAXLEN>0)
        count = count+1;

    FRAME tmp0;
    if(count<0)
    {
        tmp0.payload = qFile.read(MAXLEN);
        tmp0.md5 = md5string;
        tmp0.token = 2;
        return ;
    }

   // QByteArray tt =qFile.readAll();
   // qFile.seek(0);
    qDebug()<<count;
    int len = 0,i;
    for( i=0;i<count;i++)
    {

        len = qFile.read(data,MAXLEN);
        qDebug()<<len<<i;

//        FRAME tmp;
//        tmp.payload.append(qFile.read(MAXLEN));
//        len = tmp.payload.size();
//        if(len==0)
//            break;

//        tmp.md5 = md5string;
//        if(i==0)
//            tmp.token = 0;
//        else if(i!=0&&len<MAXLEN)
//            tmp.token = 2;
//        else
//            tmp.token = 1;
//        frame_list.append(tmp);
//        qDebug()<<tmp.token<<" "<<tmp.payload.size()<<" "<<i;
    }
    qDebug()<<count <<" "<<size<<" md5:"<<md5string <<frame_list.size();

    qFile.close();

   // return;
    QFile qFileTmp("/ll/tps4tmp");


    if (!qFileTmp.open(QIODevice::ReadWrite))
        return;

    for(i=0;i<frame_list.size();i++)
    {
        qFileTmp.write(frame_list.at(i).payload);
    }

    QByteArray baa = QCryptographicHash::hash(qFileTmp.readAll(), QCryptographicHash::Md5);
    QString md55  = QString(baa.toHex());
    qDebug()<<md55;

    qFileTmp.close();



}


void updatetask::data_received()
{

    QByteArray datagram;
    int size =0;
    memset(&st_Temp,0x00,sizeof(AC_FRAME));
    while(udp_socket->hasPendingDatagrams())
    {
        datagram.resize(udp_socket->pendingDatagramSize());
        int len = udp_socket->pendingDatagramSize();
        size = udp_socket->readDatagram(datagram.data(),len);
        qDebug()<<"\nsize:"<<size<<datagram;


       //  qDebug()<<datagram.toHex().toLower();
//         for (int i = 0;i<size ;i++)
//         {
//            printf("%02x ",datagram.at(i));
//         }
       // parse_frame(datagram.data(),size);
    }

}

void updatetask::send_frame(int type, int cmd,int execmd,int datalen,unsigned char * buf)
{

    AC_FRAME st_master;
    memset(&st_master,0x00,sizeof(AC_FRAME));

    st_master.uch_comtype = type;
    st_master.uch_cmd     = cmd;
    st_master.uch_excmd   = execmd;
    st_master.uin_datlen  = datalen;
    memcpy(st_master.uch_data+11,buf,st_master.uin_datlen);
    Make_ComPack(&st_master);
    u8 *pst;
    u16 i,len;
    pst = st_master.uch_data;
    len = st_master.uin_len;

    QByteArray tmp;
    //加码
    for(i=0;i<len;i++)
    {
        //7B 7C 7D
        char c;
        if(i > 0 && i < (len-1) && *pst >= 0x7B && *pst <= 0x7D)
        {
            c = 0x7C ;
            tmp.append(c);
            c = *pst^0x7C;
            *pst++;
            tmp.append(c);
        }
        else
        {
            c = *pst;
            *pst++;
            tmp.append(c);
        }
       // printf("%02x ",tmp.at(i));
    }
    fflush(stdout);
    //将信息发送给指定的ip
    QString ip = "192.168.100.103";
    quint16 port;

    port =5000;

    udp_socket->writeDatagram(tmp, (QHostAddress)ip, port);
}
