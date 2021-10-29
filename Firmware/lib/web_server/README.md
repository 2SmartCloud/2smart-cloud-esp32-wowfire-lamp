# WebServer

Класс для работы с вебсервером.

***

## API

- WebServer(Device \*device)
- void Init()


***

**WebServer(Device \*device)**

Создает объект веб сервера клиента.

- device: указатель на объект [устройства](device/README.md)

***

**bool Init()**

Инициализирует веб-сервер. Метод устанавливает оброботчики следующих роутов:

**GET** /  
Октрывает админ страницу index.html.

**GET** /index.html  
Октрывает админ страницу index.html.

**GET** /header.html  
Возвращает файл header.html.

**GET** /wifi.html  
Октрывает страницу подключения wifi сети wifi.html

**GET** /settings.html  
Октрывает страницу с модулями управления девайсом settings.html

**GET** /static/favicon.png  
Возвращает файл favicon.png.

**GET** /static/logo.svg  
Возвращает файл logo.svg.

**GET** /styles.css  
Возвращает файл styles.css.

**GET** /healthcheck  
Возвращает код 200.

**GET** /reboot  
Перезагружает ESP.

**GET** /resetdefault  
Стирает параметры пользователя из памяти.

**GET** /newauthpass  
Устанавливает новый пароль и перезагружает ESP для приминения изменений.
Query params:
  - newpass - новый пароль, который необходимо установить

**GET** /setwifi  
Сохраняет логин и пароль к wifi сети. Перезагружает ESP для приминения изменений.
Query params:
  - ssid - имя wifi сети;
  - pass - пароль.

**GET** /scan  
Сканирует и возвращает список wifi сетей.

**GET** /connectedwifi  
Возвращает имя wifi сети к которой подключено устройство.

**GET** /setcredentials  
Метод сохраняющий новые параметры из админ панели. Перезагружает ESP для приминения изменений.
Query params:
  - mail - email адрес пользователя.
  - token - пароль пользователя.
  - brokerPort - порт MQTT брокера.
  - productId - идентификатор продукта.
  - hostname - адрес где установлен MQTT брокер.
  - deviceId - идентификатор устройства.

**GET** /pair  
Метод сохраняющий новые параметры из мобильного приложения. Перезагружает ESP для приминения изменений.
Query params:
  - ssid - email адрес пользователя.
  - psk - пароль пользователя.
  - wsp - порт MQTT брокера.
  - token - пароль пользователя.
  - host - адрес где установлен MQTT брокер.
  - brport - порт MQTT брокера.


**GET** /update  
Отправляет новое состояние устройства.
Query params:
  - output - параметр;
  - state - значение.

**GET** /settings  
Возвращает состояние устройства.

**POST** /firmware/upload  
Принимает файл с новой прошивкой и обновляет прошивку, после чего перезапускает ESP.
