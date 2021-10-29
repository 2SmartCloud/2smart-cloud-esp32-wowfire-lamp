# PROPERTY_TYPE

Перечисление для разделения свойств на типы.

## values

- OPTION - свойство для опций, настроек и т.д.
- TELEMETRY - свойство которое только публикует значения и не считывает их.
- SENSOR - свойства для сенсоров.

***

# Property 

## API

Класс для работы со свойствами девайса и нод.

***

- Property(const char* name, const char* id, Node* node, PROPERTY_TYPE type, bool settable, bool retained, const char* data_type)
- Property(const char* name, const char* id, Device* device,PROPERTY_TYPE type, bool settable, bool retained, const char* data_type)
- virtual bool Init(Homie* homie)
- String GetValue() const
- String GetId() const
- PROPERTY_TYPE GetType() const
- Node* GetNode() const
- Device* GetDevice() const
- bool HasNewValue()
- void SetValue(String value)
- void SetHasNewValue(bool has_new_value)
- virtual void HandleCurrentState()
- virtual void HandleSettingNewValue()

***

**Property(const char\* name, const char\* id, Node\* node, PROPERTY_TYPE type, bool settable, bool retained, const char\* data_type)**

Создает объект свойства.

- name: имя свойства.
- id: идентификатор свойства.
- node: указатель на объект [ноды](../node/README.md).
- type: тип свойства.
- settable: флаг отвечающий за возможность изменить значение.
- retained: флаг отвечающий за сохранение сообщения в топкие.
- data_type: тип данных сообщения согласно конвенции homie.

***

**Property(const char\* name, const char\* id, Device\* device, PROPERTY_TYPE type, bool settable, bool retained, const char\* data_type)**

Создает объект свойства.

- name: имя свойства.
- id: идентификатор свойства.
- device: указатель на объект [устройства](../device/README.md).
- type: тип свойства.
- settable: флаг отвечающий за возможность изменить значение.
- retained: флаг отвечающий за сохранение сообщения в топкие.
- data_type: тип данных сообщения согласно конвенции homie.

***

**virtual bool Init(Homie\* homie)**

Инициализирует свойство опубликовав необходимые аттрибуты. При необходимости подписывается на свой топик. Сохраняет указатель на объект [homie](../README.md).

- homie: указатель на объект [homie](../README.md).

Возвращает статус инициализации.

***

**String GetValue() const**

Возвращает значение свойства.

***

**String GetId() const**

Возвращает идентификатор свойства.

***

**PROPERTY_TYPE GetType()**

Возвращает тип свойства.

***

**Node\* GetNode() const**

Возвращает указатель на объект [ноды](../node/README.md).

***

**Device\* GetDevice() const**

Возвращает указатель на объект [устройства](../device/README.md).

***

**bool HasNewValue()**

Возвращает флаг о наявности нового значения у свойства.

***

**void SetValue(String value)**

Устанавливает в свойство новое значение, и публикует его в брокер.

- value: новое знаяение.

***

**void SetHasNewValue(bool has_new_value)**

Устанавливает флаг наявности нового(необработанного) значения свойства.

- has_new_value: новое состояние флага. 

***

**virtual void HandleCurrentState()**

Метод-обработчик текущего значения свойства.

***

**virtual void HandleSettingNewValue()**

Метод-обработчик нового значения свойства. Если необходима специфическая обработка нового значения свойства его необходимо переопределить в классе наследнике.
