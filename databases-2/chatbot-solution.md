# Tercer Examen - Elaboración de solución

## Modelo de Datos

### 1. Tabla `clients`
* **Descripcion**: Datos de los clientes.
* **Campos**:
    * `id` (UUID): Identificador unico del cliente.
    * `nombre` (VARCHAR): Nombre del usuario de soporte.
    * `email` (VARCHAR, UNIQUE): Correo electronico del cliente.

### 2. Tabla `support`
* **Descripcion**: Datos de los usuarios de soporte.
* **Campos**:
    * `id` (UUID): Identificador unico del usuario de soporte.
    * `nombre` (VARCHAR): Nombre del usuario de soporte.
    * `email` (VARCHAR, UNIQUE): Correo electronico del usuario de soporte.

### 3. Tabla `chats`
* **Descripcion**: Representa cada sesión de chat.
* **Campos**:
    * `id` (UUID): Identificador unico del chat.
    * `client_id` (UUID): Referencia a `id` en la tabla `clients`.
    * `started_by` (UUID, NULL): Referencia a `id` en la tabla `support`. Es NULL en caso de haber sido iniciado por un chatbot.
    * `delegated_to`: (UUID, NULL): Referencia a `id` en la tabla `support`. Es NULL en caso de no haber sido delegado a soporte aun.
    * `previous_chat_id`: (UUID, NULL): Referencia al ID del chat anterior (si fue delegado).
    * `date_start` (TIMESTAMP): Marca de tiempo de inicio.
    * `date_end` (TIMESTAMP, NULL): Marca de tiempo de finalizacion. Es NULL en caso de que el chat no haya finalizado aun.

### 4. Tabla `messages`
* **Descripcion**: Representa los mensajes dentro de un chat.
* **Campos**:
    * `id` (UUID): Identificador unico del mensaje.
    * `chat_id` (UUID): Referencia a `id` en la tabla `chats`.
    * `sent_by` (ENUM): Posibles valores: `client`, `support`, `chatbot`.
    * `content` (TEXT): Contenido del mensaje.
    * `date_sent` (TIMESTAMP): Marca de tiempo del mensaje.

## Logica del flujo de datos
### 1. Inicio de un Chat:
* Si el cliente inicia el chat:

    * Crea un registro en `chats` con:
        * `client_id` asociado al cliente.
        * `started_by` en NULL (iniciado por el chatbot).
        * `date_start` con la fecha y hora actual.
        * `delegated_to` en `NULL`.

* Si soporte inicia el chat:
    * Crea un registro en `chats` con:
        * `client_id` asociado al cliente.
        * `started_by` con el ID del soporte que inició el chat.
        * `date_start` con la fecha y hora actual.
        * `delegated_to` en `NULL`.

* **Consulta ejemplo**:
```
INSERT INTO chats (client_id, started_by, date_start, delegated_to)
VALUES (`client_id`, `started_by`, CURRENT_TIMESTAMP, NULL);
```

### 2. Delegación del Chat
Cuando el chatbot no puede responder y el chat se transfiere a soporte:
* Actualiza el campo `delegated_to` con el ID del usuario de soporte asignado.

* **Consulta ejemplo**:
```
UPDATE chats
SET delegated_to = `support_id`
WHERE id = `chat_id`;
```

### 3. Envio y Registro de Mensajes
Cada vez que un mensaje es enviado:
* Crea un nuevo registro en `messages` con:
    * `chat_id` del chat correspondiente.
    * `sent_by` con el tipo de emisor (`client`, `support` o `chatbot`).
    * `content` con el contenido del mensaje.
    * `date_sent` con la fecha y hora actual.

* **Consulta ejemplo**:
```
INSERT INTO messages (chat_id, sent_by, content, date_sent)
VALUES (`chat_id`, `sent_by`, `content`, CURRENT_TIMESTAMP);
```

### 4. Recuperacion del Historial del Chat
* Para el Soporte (al ser transferido de un chatbot):
    * Recupera todos los mensajes de un chat ordenados cronologicamente (`date_sent`) para preparar el contexto del chat antes de continuar la conversacion.

* **Consulta ejemplo**:
    * Si se tiene el `id` de chat anterior:
    ```
    SELECT sent_by, content, date_sent
    FROM messages
    WHERE chat_id = `chat_id`
    ORDER BY date_sent ASC;
    ```

    * Si no se tiene el `id` del chat anterior, sino solo el `id` del chat actual:
    ```
    SELECT sent_by, content, date_sent
    FROM messages
    WHERE chat_id = (
        SELECT previous_chat_id
        FROM chats
        WHERE id = `current_chat_id`
    );
    ```

### 5. Finalizacion del Chat
Cuando el cliente finaliza la conversacion o el soporte lo cierra, actualiza el campo `date_end` en `chats` con la hora actual.

* **Consulta ejemplo**:
```
UPDATE chats
SET date_end = CURRENT_TIMESTAMP
WHERE id = `chat_id`;
```

## Logica del backend de la aplicacion
### **1. Inicio de Sesión y Autenticación**
1. **Cliente o soporte accede al sistema**:
    * El usuario se autentica enviando sus credenciales.
    * El backend valida las credenciales mediante la base de datos.

2. **Generación de token de sesión**:
    * Si las credenciales son válidas:
        * Se genera un token con los datos del usuario (ID, rol, y nombre).
        * Este token se utiliza para autenticar solicitudes posteriores.

## **2. Inicio de Chat por Cliente**
1. **Cliente inicia un chat con el chatbot**:
    * El cliente envía una solicitud para iniciar un chat.
    * El backend:
    * Crea un registro en la tabla `chats`:
        * `client_id`: ID del cliente autenticado.
        * `started_by`: `NULL` (indica que comenzó con el chatbot).
        * `delegated_to`: `NULL`.
        * `date_start`: Fecha y hora actuales.
    * Responde al cliente con el `id` del chat recién creado.
    * El chatbot se conecta al cliente mediante WebSockets.

2. **El cliente envía mensajes al chatbot**:
    * Cada mensaje enviado por el cliente:
        * Es procesado por el backend.
        * Se registra en la tabla `messages`:
        * `chat_id`: ID del chat.
        * `sent_by`: `client`.
        * `content`: Contenido del mensaje.
        * `date_sent`: Fecha y hora actuales.
    * El mensaje es enviado al chatbot para generar una respuesta.
    * Los mensajes enviados por el chatbot se registran en la base de datos con `sent_by`: `chatbot`.

## **3. Delegación del Chat a Soporte**
1. **El chatbot decide delegar**:
    * Si el chatbot detecta que no puede resolver el problema del cliente:
    * Solicita al backend delegar el chat a un usuario de soporte.

2. **Asignación a soporte**:
    * El backend selecciona un usuario de soporte disponible (por ejemplo, basado en carga de trabajo).
    * Actualiza el registro en la tabla `chats`:
        * `delegated_to`: ID del soporte asignado.
    * Notifica al soporte asignado mediante WebSockets.

3. **Historial del chat**:
    * Antes de que el soporte tome el control:
        * El backend recupera todos los mensajes del chat desde la tabla `messages`.
        * El historial se envía al soporte para proporcionar contexto.

4. **El soporte toma el control**:
    * El soporte acepta el chat y se conecta al cliente mediante WebSockets.
    * Los mensajes subsiguientes se registran con `sent_by` como `support` o `client`.

## **4. Envío y Registro de Mensajes**
1. **Registro de mensajes enviados por cliente o soporte**:
    * Cada mensaje enviado:
        * Se valida mediante el token de sesion.
        * Se guarda en la tabla `messages`:
            * `chat_id`: ID del chat.
            * `sent_by`: `client`, `support` o `chatbot`.
            * `content`: Contenido del mensaje.
            * `date_sent`: Fecha y hora actuales.
        * Se envía a través de WebSockets al destinatario.

## **5. Cierre del Chat**
1. **Finalización del chat**:
   * El cliente o el soporte pueden solicitar cerrar el chat.
   * El backend actualiza el campo `date_end` en la tabla `chats` con la fecha y hora actuales.

2. **Liberación de recursos**:
   * Si el soporte estaba asignado, el backend lo elimina de la lista de chats activos.

## Consideraciones adicionales
* Se recomienda crear **Indexes** en las foreign keys de las tablas de la base de datos para mejorar el rendimiento en las consultas realizadas.