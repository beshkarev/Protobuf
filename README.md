## What is required?
- Protobuf v3.5 at least
- Boost v1.4 at least

## What is required for launch?
ProtobufConfig.json from config directory have to be placed near bin file.

## How to work with application?
You need launch a first instance of the application with default config file. After that you have to change *client_port* and *server_port* to another values, e.g just swipe between them and start a second instance after that.

*First instance*
```json
{
    "client_port": 7777,
    "server_hostname": "localhost",
    "client_hostname": "localhost",
    "server_port": 8888
}
```

*Second instance*
```json
{
    "client_port": 8888,
    "server_hostname": "localhost",
    "client_hostname": "localhost",
    "server_port": 7777
}
```
