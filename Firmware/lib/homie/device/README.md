# DeviceData

Структура которая содержит необходимые данные для устройства

## Properties

- const char* name - имя устройства
- const char* fw_ver - версия прошивки
- const char* fw_name - название прошивки
- const char* ip - IP адрес устройства
- const char* impl - Идентификатор для реализации Homie (например, "esp-32")
- const char* mac - MAC адресс устройства
- const char* state - состояние устройства
- const char* id - идентификатор устройства

***

# Device

Класс для работы с устройством.

***
## API

- Device(Homie* homie)
- void SetCredentials(DeviceData device_data)
- bool Init()
- void AddNode(Node* node)
- void AddProperty(Property* property)
- String GetId() const
- String GetFirmwareVersion() const
- Node* GetNode(String id)
- void SetNotifier(Notifier* notifier)
- void HandleCurrentState()
- bool SendNotification(String text)
- bool IsSysNotifyEnabled()
- bool IsFwNotifyEnabled()

***

**Device(Homie\* homie)**

Создает объект устройства.

- homie: указатель на объект [homie](../README.md).

***

**void SetCredentials(DeviceData device_data)**

Присваивает необходимые данные устройству.

- device_data: структура с данными для устройства.

***

**bool Init()**

Инициализирует устройство опубликовав необходимые аттрибуты. Вызывает инициализацию у [нод](../node/README.md) и [свойств](../property/README.md).

Возвращает статус инициализации.

***

**void AddNode(Node\* node)**

Добавляет указатель на ноду в список нод для дальнейшего обращения.

- node: указатель на ноду.

***

**void AddProperty(Property\* property)**

Добавляет указатель на свойство в список свойств для дальнейшего обращения.

- property: указатель на [свойство](../property/README.md).

***

**String GetId() const**

Возвращает идентификатор устройства.

***

**String GetFirmwareVersion() const**

Возвращает версию прошивки.

***

**Node\* GetNode(String id)**

Возвращает указатель на ноду по идентификатору.

- id: идентификатор [ноды](../node/README.md).

***

**void SetNotifier(Notifier\* notifier)**

Сохраняет указатель на модуль уведомлений.

- notifier: указатель на объект [модуля уведомлений](../notifier/README.md).

***

**void HandleCurrentState()**

Вызывает обработку текущих состояний у [нод](../node/README.md) и [свойств](../property/README.md). Осуществялет отправку heartbeat.

***

**bool SendNotification(String text)**

Отправляет уведомление с принятым текстом.

- text: текст сообщения.

Возвращает статус отправки уведомления.

***

**bool IsSysNotifyEnabled()**

Возвращает состояние системных уведомлений.

***

**bool IsFwNotifyEnabled()**

Возвращает состояние уведомлений об обновлениях.
