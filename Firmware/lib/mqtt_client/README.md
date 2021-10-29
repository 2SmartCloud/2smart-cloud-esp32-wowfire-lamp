# MqttClient

Класс для работы с MQTT протоколом.

***

## API

- MqttClient()
- bool Init(String username, String host, String port, String password, std::function<void(char*, uint8_t*, unsigned int)> callback)
- void MqttLoop()
- bool Subscribe(String topic)
- bool Publish(String topic, String value, bool retained)

***

**MqttClient()**

Создает объект MQTT клиента.

***

**bool Init(String username, String host, String port, String password, std::function<void(char\*, uint8_t\*, unsigned int)> callback)**

Сохраянет принятые параметры и инициализирует MQTT клиент.

- username: идентификатор пользователя.
- host: адрес где установлен MQTT брокер.
- port: порт на котором находится брокер.
- password: пароль для подключения к брокеру.
- callback: функция которая будет вызываться  на принятие новых сообщений из брокера. 

Возвращает статус инициализации.

***

**void MqttLoop()**

Проверяет подключение к брокеру и наличиен новых сообщений.

***

**bool Subscribe(String topic)**

- topic: топик на который надо подписаться.

Возвращает статус подписки.

***

**bool Publish(String topic, String value, bool retained)**

Проверяет подключен ли клиент к брокеру и публикует сообщение в топик.

- topic: топик на который надо подписаться.
- value: сообщение.
- retained: флаг отвечающий за сохранение сообщения в топкие.

Возвращает статус публикации.

***
