# WifiClient

Класс для работы с Wi-Fi клиентом.

***

## API

- WifiClient()
- void SetCredentials(String ssid_name, String ssid_password)
- bool Connect()

***

**MqttClient()**

Создает объект Wi-Fi клиента.

***

**void SetCredentials(String ssid_name, String ssid_password)**

Метод для установки имени и пароля к Wi-Fi.

- ssid_name: назваение сети.
- ssid_password: пароль к сети.

***

**Connect()**

Подключается к сети, в случае если подключение ещё не установленно устанавливает его.

Возвращает статус подключения.
