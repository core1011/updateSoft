
```

# tps4
tps4

上位机下发
{"type":"softVer"}
回复上位机
{"data":{"ver":"1.0","maketime":"20200917"},"meta":{"isMessage":false,"type":"softVer"}}

上位机下发


{
        "data":
        {
            "fileSize":90000,
            "frameSize":100,
            "state":"start"
        },
        "meta":
        {
            "isMessage": false, //字段data 带节点的话为false，为单纯的字符串时候为true
            "type": "updateSoft"
        }
}
{
        "data":
        {
            "payload": "max 512字节",
            "state":"transfer",
            "transferSize":512/1024/1536/2048
            "transferNo":0/1/2/3/sum

        },
        "meta":
        {
            "isMessage": false, //字段data 带节点的话为false，为单纯的字符串时候为true
            "type": "updateSoft"
        }
}

#when lost frame node will retun this json data
{"error":{"message":"frame id lost","lostTransferNo":333},"type":"updateSoft"}



{
        "data":
        {
            "md5":"md5值11111111",
            "state":"end"
        },
        "meta":
        {
            "isMessage": false, //字段data 带节点的话为false，为单纯的字符串时候为true
            "type": "updateSoft"
        }
}

/*
{
        "data":
        {
            "fileSize":90000,
            "payload": "max 512字节",
            "md5":"md5值11111111",
            "frameSize":100,
            "state":"start/run/end"
            "frameNo":0/1/2/3/sum /0 开始帧，1中间帧，frameSize结束帧
        },
        "meta":
        {
            "isMessage": false, //字段data 带节点的话为false，为单纯的字符串时候为true
            "type": "updateSoft"
        }
}*/

回复上位机 成功

{"data":"update success","meta":{"isMessage":true,"type":"updateSoft"}}
失败
{"error":{"message":"msd5 err"},"type":"updateSoft"}





{"data":{"colorName":"蓝","paInfo":[{"seqName":"二号"}],"customerCode":"ceshi1117","orderNum":"30","styleName":"ceshi1117","hangerCode":"3881768","styleCode":"ceshi1117","customerName":"ceshi1117","requireTime":"2020-11-23","planNum":"30","hgType":"缝制衣架","sizeName":"L","orderCode":"201117-0001","colorCode":"03","sizeCode":"01"},"meta":{"type":"mq.realTime"}}
{"data":{"faultNumber":0,"planNum":30,"lastCostTime":"16","hgFinishNumber":2,"totalTime":18,"hangerCode":"3881768","totalStdAvgTime":0,"nextStation":"1101-03","finishNumber":2},"meta":{"type":"mq.lastCostTime"}}
{"data":{"allowReceive":"1","empCode":"1107test10","empName":"1107员工10","sysTime":"2020-11-30 14:37:23"},"meta":{"type":"mq.stateInTime"}}
{"data":{"code":"W1004","codeType":"W","message":"重复出站"},"meta":{"type":"mq.stateCode"}}
{"data":{"allowReceive":"1","empCode":"1107test10","empName":"1107员工10","sysTime":"2020-11-30 15:08:34"},"meta":{"type":"mq.stateInTime"}}







```
