#include "updatetask.h"

updatetask::updatetask()
{
    udp_socket = new QUdpSocket(this);
    udp_socket->bind(8001, QUdpSocket::ShareAddress | QUdpSocket::ReuseAddressHint);
    connect(udp_socket, SIGNAL(readyRead()), this, SLOT(data_received()));
}

void updatetask::run()
{
    read_file2();
    lost_frame_no  = 0;
    lost_frame = false;
    int size = frame_transfer_list.size();
    int len = 0;
    char buf[1024]={0};
   // size =10;

    len = sprintf(buf,"{\"data\":{\"fileSize\":%d,\"state\":\"%s\",\"frameSize\":%d},\"meta\":{\"isMessage\":false,\"type\":\"updateSoft\"}}"
                  ,frame_start.fileSize,frame_start.state.toStdString().c_str(),frame_start.frameSize);
    send_frame(COM_ETH,0xD5,CMD_W_SUCCESS,len,(unsigned char *)buf);
    usleep(1000*2000);//need make start frame not lost

    while (1)
    {
        FRAMETRANSFER tmp ;
        for(int i= 0;i<size;i++)
        {
            if(lost_frame)
            {
                lost_frame = false;
                i = lost_frame_no;
            }

            tmp = frame_transfer_list.at(i);
            QByteArray array =QByteArray(tmp.payload,tmp.len);
            len = sprintf(buf,"{\"data\":{\"transferSize\":%d,\"payload\":\"%s\",\"state\":\"%s\",\"transferNo\":%d},\"meta\":{\"isMessage\":false,\"type\":\"updateSoft\"}}"
                          ,tmp.transferSize,array.toBase64().data()
                          ,tmp.state.toStdString().c_str()
                          ,tmp.transferNo);
            //qDebug()<<tmp.payload.size()<<len<<buf;

            send_frame(COM_ETH,0xD5,CMD_W_SUCCESS,len,(unsigned char *)buf);

            usleep(1000*180);
        }
        break;
    }
    usleep(1000*2000);//need make last frame not lost
    len = sprintf(buf,"{\"data\":{\"md5\":\"%s\",\"state\":\"%s\"},\"meta\":{\"isMessage\":false,\"type\":\"updateSoft\"}}"
                  ,frame_end.md5.toStdString().c_str(),frame_end.state.toStdString().c_str());
    send_frame(COM_ETH,0xD5,CMD_W_SUCCESS,len,(unsigned char *)buf);



}


//void updatetask::run()
//{

//    //read_file();
//    //test_frame();

//    read_file2();

//    int size = frame_list.size();
//    int len = 0;
//    char buf[1024]={0};
//   // size =10;

//    while (1)
//    {
//        FRAME tmp ;
//        for(int i= 0;i<size;i++)
//        {
//            tmp = frame_list.at(i);
//            QByteArray array =QByteArray(tmp.payload2,tmp.len);
//            len = sprintf(buf,"{\"data\":{\"fileSize\":%d,\"payload\":\"%s\",\"md5\":\"%s\",\"token\":%d},\"meta\":{\"isMessage\":false,\"type\":\"updateSoft\"}}"
//                          ,tmp.fileSize,array.toBase64().data(),tmp.md5.toStdString().c_str(),tmp.token);
//            //qDebug()<<tmp.payload.size()<<len<<buf;
//            //send_frame(COM_ETH,0x80,CMD_READ,len,(unsigned char *)buf);

//            send_frame(COM_ETH,0xD5,CMD_W_SUCCESS,len,(unsigned char *)buf);

//            usleep(1000*350);
//        }
//        break;
//    }
//}


void updatetask::read_file2()
{
    FILE * fp = NULL;
    long filesize=0,transferSize=0;
    int n=0,no=0;
    char buf[MAXLEN];
    char *file0 = "/ll/tps4.tar.gz";
    fp = fopen(file0,"r+");
    if(fp ==NULL)
        return ;
    filesize = make_filesize(QString(file0));
    QString md5 = make_md5(QString(file0));
    while((n = fread( buf,1, MAXLEN,fp))>0)
    {
        FRAMETRANSFER tmp1;
        tmp1.transferNo =no;
        no++;
        transferSize = transferSize + n;
        tmp1.transferSize = transferSize;
        memset(tmp1.payload,0x00,MAXLEN);
        memcpy(tmp1.payload,buf,n);
        tmp1.len = n;
        tmp1.state="transfer";
        frame_transfer_list.append(tmp1);
        qDebug()<<n<<tmp1.transferSize<<tmp1.transferNo;
    }

    frame_start.fileSize = filesize;
    frame_start.frameSize = frame_transfer_list.size();
    frame_start.state = "start";

    frame_end.md5 = md5;
    frame_end.state="end";

    qDebug()<<" md5:"<<md5 <<frame_start.fileSize <<frame_transfer_list.size();
    fclose(fp);



}




void updatetask::read_file()
{
    FILE * fp = NULL;
    int n=0,sum=0,no=0;
    char buf[MAXLEN];
    //char *file0 = "/ll/tps4";
    //char *file0 = "/ll/tps4.zip";
     char *file0 = "/ll/tps4.tar.gz";
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
    tmp0.frameNo = no;
    tmp0.fileSize = sum;
    tmp0.frameSize = 0;
    tmp0.token = 0;
    frame_list.append(tmp0);

    while((n = fread( buf,1, MAXLEN,fp))>0)
    {
        FRAME tmp1;
        no++;
        tmp1.frameNo =no;
        memset(tmp1.payload2,0x00,MAXLEN);
        memcpy(tmp1.payload2,buf,n);
        tmp1.len = n;
        tmp1.token = 1;
        tmp1.fileSize = sum;
        tmp1.payload = buf;
        tmp1.payload.resize(n);

        frame_list.append(tmp1);


        qDebug()<<n<<sum<<tmp1.token;
    }

    FRAME tmp2;
    tmp2.len=0;
    tmp2.md5 = md5;
    tmp2.token = 2;
    tmp2.payload.resize(0);
    no++;
    tmp2.frameNo = no;
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
            send_frame(COM_ETH,0x80,CMD_READ,len,(unsigned char *)buf);
            usleep(1000*200);

       //{"data":{"faultNumber":0,"planNum":30,"lastCostTime":"16","hgFinishNumber":2,"totalTime":18,"hangerCode":"3881768","totalStdAvgTime":0,"nextStation":"1101-03","finishNumber":2},"meta":{"type":"mq.lastCostTime"}}

    len = sprintf(buf,"{\"meta\":{\"type\":\"mq.lastCostTime\"},\"data\":{\"hangerCode\":\"800021\",\"lastCostTime\":\"10\",\"hgFinishNumber\":100,\"nextStation\":\"1101-02\",\"planNum\":\"200\",\"faultNumber\":5,\"totalTime\":222,\"totalStdAvgTime\":555}}");
    send_frame(COM_ETH,0x80,CMD_READ,len,(unsigned char *)buf);
    usleep(1000*200);

    len = sprintf(buf,"{\"data\":{\"code\":\"W1004\",\"codeType\":\"E\",\"message\":\"重复出站\"},\"meta\":{\"type\":\"mq.stateCode\"}}");

    //send_frame(COM_ETH,0x80,CMD_READ,len,(unsigned char *)buf);
    usleep(1000*2000);


    len = sprintf(buf,"{\"data\":{\"code\":\"W1004\",\"codeType\":\"W\",\"message\":\"重复出站\"},\"meta\":{\"type\":\"mq.stateCode\"}}");

    //send_frame(COM_ETH,0x80,CMD_READ,len,(unsigned char *)buf);
    usleep(1000*200);


    len =sprintf(buf,"{\"data\":{\"allowReceive\":\"0\",\"empCode\":\"1107\",\"empName\":\"员工10\",\"sysTime\":\"2020-11-30 15:08:34\"},\"meta\":{\"type\":\"mq.stateInTime\"}} ");
    send_frame(COM_ETH,0x80,CMD_READ,len,(unsigned char *)buf);

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
        qDebug()<<"size:"<<size<<QString(datagram.data());


       //qDebug()<<datagram.toHex().toLower();
//     for (int i = 0;i<size ;i++)
//     {
//        printf("%02x ",datagram.at(i));
//     }
        parse_frame(datagram.data(),size);
    }

}

void updatetask::parse_frame(char *data,int size)
{
    char *p;
    char uch_temp;
    u16 i;
    p = data;

    //循环处理数据
    for( i=0; i<size; i++ )
    {
        uch_temp = *p++;
        st_Temp.uch_data[st_Temp.uin_len++] = uch_temp;
       // printf("%02x ",uch_temp);
        fflush(stdout);

        if(st_Temp.uin_len == 1)
        {
            if(uch_temp != 0x7B)
            {
               qDebug()<<"====0";
               st_Temp.uin_len = 0;
               memset(&st_Temp,0x00,sizeof(AC_FRAME));
               break;
            }
            st_Temp.uch_last = 0x00;
        }
        else if(st_Temp.uin_len <= BUF_MAX)
        {
            if(st_Temp.uch_last == 0x7C)
            {
                st_Temp.uch_last = 0x00;
                st_Temp.uin_len --;
                st_Temp.uch_data[st_Temp.uin_len-1] = (uch_temp^0x7c);
            }
            else if(uch_temp == 0x7D)
            {
                if(st_Temp.uin_len < 10)
                {
                    st_Temp.uin_len = 0;
                    memset(&st_Temp,0x00,sizeof(AC_FRAME));
                    qDebug()<<"====1";
                    break;
                }
                else
                {
                    //接收完一帧
                    st_Temp.uch_comtype = COM_ETH;
                    st_Temp.uch_type    = PACKTYPE_AC;
                    st_Temp.state       = STATE_REV;
                    //qDebug()<<"====OK";
                    //return st_Temp;
                    //解通信包
                    if(Dec_ComPack(&st_Temp))
                        parse_data(&st_Temp);
                }
            }
            st_Temp.uch_last = uch_temp;
        }
        else
        {
            qDebug()<<"===3";
            st_Temp.uin_len = 0;
            memset(&st_Temp,0x00,sizeof(AC_FRAME));
            return ;
        }
}

}

void updatetask::parse_data(AC_FRAME *pr)
{
    qDebug()<<"cmd:"<<pr->uch_cmd;
    switch (pr->uch_cmd)
    {
        case 0xD5:
        case 0x80:
        {
            cJSON *json=NULL;
            json=cJSON_Parse((const char *)pr->puch_data);
            if (!json)
            {
              qDebug("parse json Error" );
            }
            else
            {
                qDebug()<<"action_cure:"<<cJSON_PrintUnformatted(json);
                update_soft(json);
                //int ret = action_cure(json);
                if(json)
                {
                    cJSON_Delete(json);
                    json = NULL;
                }
            }
           fflush(stdout);
        }
        break;
    default:
    break;
    }
}

void updatetask::update_soft(cJSON *json)
{
    //{"error":{"message":"frame id lost","lostTransferNo":333},"type":"updateSoft"}
    cJSON *error =NULL,*lost=NULL;
    error = cJSON_GetObjectItem(json, "error");
    if(error==NULL)
    {
        return ;
    }
    else
    {
        lost = cJSON_GetObjectItem(error, "lostTransferNo");
       if(lost!=NULL)
       {
           lost_frame_no  = lost->valueint;
           lost_frame = true;
       }
       return ;
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
