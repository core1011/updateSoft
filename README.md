# tps4
tps4

上位机下发
{"type":"softVer"}
回复上位机
{"data":{"ver":"1.0","maketime":"20200917"},"meta":{"isMessage":false,"type":"softVer"}}

上位机下发

{"data":{"fileSize":90000,"payload":"512字节","md5":"md5值11111111","token":0},"meta":{"isMessage":false,"type":"updateSoft"}}


{
        "data":
        {
            "fileSize":90000,
            "payload": "max 512字节",
            "md5":"md5值11111111",
            "token":0/1/2  //0 开始帧，1中间帧，2结束帧
        },
        "meta":
        {
            "isMessage": false, //字段data 带节点的话为false，为单纯的字符串时候为true
            "type": "updateSoft"
        }
}

回复上位机 成功

{"data":"update success","meta":{"isMessage":true,"type":"updateSoft"}}
失败
{"error":{"message":"msd5 err"},"type":"updateSoft"}
