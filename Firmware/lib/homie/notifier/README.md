# Notifier

Класс для работы с уведомлениями.

***
## API

- Notifier(MqttClient* mqtt_client)
- void SetUserHash(String user_hash)
- bool CreateNotification(String text)

***

**Notifier(MqttClient\* mqtt_client)**

Создает объект модуля для уведомлений.

- mqtt_client: указатель на объект [MQTT клиента](../../mqtt_client/src/README.md).

***

**void SetUserHash(String user_hash)**

Запоминает идентификатор пользователя.

- user_hash: идентификатор пользователя.

***

**bool CreateNotification(String text)**

Публикует уведомление в топик `<user_hash>/notifications/create` 

- text: JSON строка с параметрами уведомления.

Возвращает статус публикации уведомлений.
