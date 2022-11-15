参考自：[qinguoyi/TinyWebServer](https://github.com/qinguoyi/TinyWebServer)

## 接口：

```shell
# 从数据库中读取内容 && 向数据库中添加内容
curl  127.0.0.1:9999/message_board/all
curl  127.0.0.1:9999/message_board -X POST -d '{"name": "大草", "message":"嘿嘿"}'
```

## 压力测试

略。
