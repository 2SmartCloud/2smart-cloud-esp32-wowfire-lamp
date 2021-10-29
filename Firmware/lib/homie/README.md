# Homie 


Класс для управления сущностями Homie и обеспечения их взаимодействия с MQTT брокером.

***
## API

- Homie(MqttClient* mqtt_client)
- void SetDevice(Device* device);
- bool Init(String user_hash, String host, String broker_port, String token, std::function<void(char*, uint8_t*, unsigned int)> callback)
- bool Publish(const Device& device, String attribute, String value, bool retained)
- bool Publish(const Node& node, String attribute, String value, bool retained)
- bool Publish(const Property& property, String attribute, String value, bool retained)
- bool SubscribeTopic(const Property& property)
- void HandleMessage(String topic, byte* payload, unsigned int length)
- void HandleCurrentState()

***

**Homie(MqttClient\* mqtt_client)**

Создает объект homie, принимает объект MQTT клиента 

- mqtt_client: объект [MQTT клиента](../mqtt_client/src/README.md) который присваивается.

***

**void SetDevice(Device\* device)**

Присваивает объект устройства к текущему объекту homie

- device: объект устройства который присваивается.

***

**bool Init(String user_hash, String host, String broker_port, String token, std::function<void(char\*, uint8_t\*, unsigned int)> callback)**

Подключается к MQTT брокеру и вызывает инициализацию устройства.

- user_hash: идентификатор пользователя, генерируется на основе почтового ящика.
- host: адрес где установлен MQTT брокер.
- broker_port: порт на котором находится брокер.
- token: пароль для подключения к брокеру, генерируется при создании пользователя.
- callback: функция которая будет вызываться  на принятие новых сообщений из брокера. 

Возвращает статус инициализации.

***

**bool Publish(const Device& device, String attribute, String value, bool retained)**

Публикует значение аттрибута устройства в брокер.

- device: указатель на объект [устройства](device/README.md).
- attribute: аттрибут который публикуется.
- value: значение параметра.
- retained: флаг отвечающий за сохранение сообщений в топиках.

Возвращает статус публикации сообщения в топик.

***

**bool Publish(const Node& node, String attribute, String value, bool retained)**

Публикует значение аттрибута ноды в брокер.

- node: указатель на объект [ноды](node/README.md).
- attribute: аттрибут который публикуется.
- value: значение параметра.
- retained: флаг отвечающий за сохранение сообщений в топиках.

Возвращает статус публикации сообщения в топик.

***

**bool Publish(const Property& property, String attribute, String value, bool retained)**

Публикует значение аттрибута свойства в брокер.

- property: указатель на объект [свойства](property/README.md).
- attribute: аттрибут который публикуется.
- value: значение параметра.
- retained: флаг отвечающий за сохранение сообщений в топиках.

Возвращает статус публикации сообщения в топик.

***

**bool SubscribeTopic(const Property& property)**

Подписывается на /set топик значения свойства.

Пример: 
```
Для сенсора, который имеет топик значения 73062d872926c2a556f17b36f50e328ddf9bff9d403939bd14b6c3b7f5a33fc2/sweet-home/device-id/node-id/sensor-id
при вызове SubscribeTopic homie подпишется на топик 73062d872926c2a556f17b36f50e328ddf9bff9d403939bd14b6c3b7f5a33fc2/sweet-home/device-id/node-id/sensor-id/set
```

- property: указатель на объект [свойства](property/README.md).

Возвращает статус подписки.

***

**void HandleMessage(String topic, byte\* payload, unsigned int length)**

Принимает сообщение и вызывает обработчик у необходимого [свойства](../property/README.md)

- topic: топик откуда пришло сообщение.
- payload: текст сообщения.
- length: длинна сообщения.

***

**void HandleCurrentState()**

Вызывает цикл у MQTT клиента, и обрабатывает текущее состояние устройства
