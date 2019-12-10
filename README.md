# esp-443-mqtt
All the script does is broadcasting all 443Mhz signals into a MQTT server using an ESP8266.

![443Mhz signals](https://i.poespas.me/valuable-flavin.png)

## Possible MQTT Messages:
#### Connected
When the device is connected to the MQTT server, it will send a message with its macaddress:
```
Topic: /esp/connected
Message: 5C:CF:7F:4C:7B:1F
```

#### On Signal/Message
When the device detects a signal, it will get sent to the MQTT server:
```
Topic: /esp/message
Message: 1956369811/32/2
```

The message is composed of the **received value, bit length, and received protocol**
