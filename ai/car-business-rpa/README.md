# Automatización de Reportes de Ventas vía WhatsApp

Este proyecto de Python implementa un proceso automatizado para el análisis de datos de ventas. El sistema extrae información de ventas, consolida métricas, crea visualizaciones de datos y distribuye un informe resumido utilizando la API de Twilio para WhatsApp.

* Procesamiento y validación de archivos Excel (`data/Ventas_Fundamentos.xlsx`).
* Cálculo de métricas clave (top modelos, sedes, canales).
* Creación de gráficos guardados en la carpeta `outputs/graphs/`.
* Distribución de reportes por WhatsApp usando Twilio.
* Soporte opcional para alojamiento de imágenes en imgbb (para URLs públicas).
* Mecanismo de contingencia (fallback) a modo de simulación si se alcanza el límite diario de Twilio (error `63038`).

---

## Requisitos del sistema

* Python 3.10 o una versión más reciente.
* Se recomienda Windows (probado en PowerShell), aunque debería funcionar en otros sistemas operativos con Python.
* Una cuenta de Twilio activa con capacidad de envío de WhatsApp (número de envío).
* Una cuenta de imgbb (opcional) para el alojamiento de gráficos.

Las dependencias de Python se gestionan mediante el archivo `requirements.txt`.

---

## Instalación y Configuración Inicial

Siga estos pasos en una terminal (por ejemplo, PowerShell en Windows):

```powershell
# 1) Clonar o descargar el repositorio
# cd <directorio-del-proyecto>

# 2) Crear y activar un entorno virtual (recomendado)
python -m venv .venv
.\.venv\Scripts\Activate.ps1

# 3) Instalar los paquetes necesarios
pip install -r requirements.txt

# Opcionalmente, puede usar el script auxiliar:
# python install_dependencies.py

# Variables de Entorno para la Automatización de Ventas
```
La configuración de esta aplicación se gestiona a través de un archivo `.env` (generalmente `whatsapp_config.env`, basado en la plantilla `whatsapp_config.env.sample`). Estas variables son esenciales para la conectividad de Twilio, el envío de reportes vía WhatsApp y las opciones de simulación.

# Configuración del envío de WhatsApp y API de Twilio
```
WHATSAPP_DESTINY = +1234567890             # Número E.164 del destinatario
TWILIO_ACCOUNT_SID = ACxxxxxxxxxxxxxxxxxxxx
TWILIO_AUTH_TOKEN = xxxxxxxxxxxxxxxxxxxxxxxx
TWILIO_WHATSAPP_FROM = +19876543210        # Número Twilio (E.164)
```
# Configuración opcional para alojamiento de imágenes
```
IMGBB_API_KEY = xxxxxxxxxxxxxxxxxxxxxxxx   # Clave API de imgbb (si es requerida)
```
# Opciones de reintento y simulación
```
WHATSAPP_MAX_RETRIES = 3                   # Intentos máximos en fallas transitorias
WHATSAPP_WAIT_TIME = 5                     # Espera entre reintentos (segundos)
WHATSAPP_SIMULATE = false                  # true/false para forzar el modo de simulación
```

# Ejecución del Proceso Automatizado

Siga estos pasos para iniciar la carga de datos, el análisis y la distribución del reporte de ventas:

1. **Generar Datos de Muestra (Opcional):** Si no posee el archivo de datos de ventas (`data/Ventas_Fundamentos.xlsx`), puede crear uno de ejemplo:

    ```powershell
    python create_sample_data.py
    ```

2. **Iniciar el proceso RPA**

# Notas de Seguridad y Control de Versiones

Se han implementado medidas específicas para proteger credenciales y datos sensibles de ser expuestos en el repositorio:

* **Variables de Entorno:** El archivo `whatsapp_config.env` que almacena las claves sensibles (SID, tokens) de Twilio está incluido en el archivo `.gitignore` y **no debe ser subido** al repositorio. Utilice la plantilla `whatsapp_config.env.sample` como referencia.
* **Archivos Generados:** Los directorios `outputs/` (gráficos, logs, mensajes simulados) y `data/` (el archivo de ventas Excel) también están listados en `.gitignore` para evitar la subida accidental de datos o informes confidenciales.
* **Proceso RPA**: Ejecute el script principal para que el flujo de trabajo se complete (carga, análisis, gráficos y envío/simulación):

    ```powershell
    python main.py
    ```

El sistema cargará los datos, generará los análisis y gráficos necesarios, y procederá al envío del reporte por WhatsApp vía Twilio o, si es el caso, a la simulación del envío.

# Arquitectura del Flujo de Trabajo

El proceso de automatización sigue una secuencia de pasos lógica y modular para garantizar el correcto análisis y distribución del reporte:

1. **Carga y Validación de Datos:** El sistema lee y verifica la estructura de los datos (detallado en `utils/data_loader.py`).
2. **Análisis de Datos:** Se calculan métricas y agregados de ventas clave (módulo `utils/analyzer.py`).
3. **Generación de Gráficas:** Se crean las visualizaciones de datos, las cuales se guardan en la ruta `outputs/graphs/` (módulo `utils/visualizer.py`).
4. **Envío del Reporte:** Se procede a la distribución del reporte por WhatsApp, incluyendo el texto y las URLs de las imágenes, utilizando `utils/whatsapp_sender.py`.

# Estructura del Proyecto

Esta es la jerarquía de archivos y módulos que componen la aplicación RPA de análisis de ventas:

main.py                   # Script principal (orquestador del flujo completo)
create_sample_data.py     # Utilidad para generar datos de ventas de prueba (Excel)
requirements.txt          # Listado de paquetes y dependencias de Python
whatsapp_config.env.sample # Plantilla de variables de entorno para la configuración

utils/
  data_loader.py          # Módulo responsable de la carga y validación de datos
  analyzer.py             # Módulo para el cálculo de métricas y agregaciones
  visualizer.py           # Módulo para la creación y guardado de gráficos
  whatsapp_sender.py      # Módulo que maneja el envío por Twilio y el fallback a simulación
  image_uploader.py       # Módulo para subir las imágenes a imgbb

experimental/
  ...                     # Implementaciones de prueba o archivadas (no productivas)

outputs/
  graphs/                 # Directorio donde se almacenan las visualizaciones generadas
  simulation_log.txt      # Archivo de registro del historial de ejecuciones simuladas
  simulation_message.txt  # Contenido del mensaje de WhatsApp cuando se ejecuta en modo simulación

# Archivos de Salida del Proceso

El proceso de automatización genera varios artefactos importantes en el directorio `outputs/`. Estos archivos contienen las visualizaciones y la documentación de la ejecución:

* **Gráficos:** Se almacenan todas las imágenes generadas por el análisis en la subcarpeta `outputs/graphs/` (formatos como `.png` o `.jpg`).
* **Mensaje Simulado:** Cuando el proceso se ejecuta en modo simulación (o como *fallback*), el contenido del cuerpo del mensaje de WhatsApp se guarda en `outputs/simulation_message.txt`.
* **Registro de Simulación:** El historial de las ejecuciones simuladas se anota, incluyendo la hora, en el archivo `outputs/simulation_log.txt`.

# Manejo de Límites de Twilio

El sistema está diseñado para gestionar automáticamente la limitación de la cuota diaria de mensajes impuesta por Twilio.

**Contingencia por Límite Diario:**

Si la API de Twilio retorna el código de error `63038` (que indica que se ha superado el límite de mensajes diarios permitido):

1.  **Suspensión de Reintentos:** El proceso detiene inmediatamente los intentos de envío inútiles.
2.  **Alojamiento de Imágenes:** Las gráficas generadas se suben a imgbb (si la variable `IMGBB_API_KEY` está configurada), y el sistema recolecta las URLs públicas.
3.  **Simulación:** Se activa el modo de simulación, y el reporte completo (incluyendo el texto y las URLs de las imágenes) se guarda en `outputs/simulation_message.txt` y se registra en `outputs/simulation_log.txt`.

**Resolución del Límite:**

Para reanudar los envíos reales, debe esperar el reinicio automático del ciclo de 24 horas de Twilio o contactar a su equipo de Soporte para solicitar un incremento en la cuota diaria asignada a su cuenta.

# Diagnóstico y Solución de Problemas

Aquí se presentan las soluciones a los inconvenientes más comunes durante la configuración y ejecución del proyecto:

* **Problemas de Envío de WhatsApp (Twilio):**
    * Verifique que las credenciales (SID y Token) en `whatsapp_config.env` sean correctas.
    * Asegúrese de que el número de Twilio (`TWILIO_WHATSAPP_FROM`) esté correctamente configurado con la capacidad de WhatsApp y no esté operando en modo *sandbox* sin verificar.
    * Revise los límites y el estado general de su cuenta de Twilio.
* **Imágenes Ausentes en el Mensaje:**
    * Confirme que se generaron archivos de imagen en la ruta `outputs/graphs/`.
    * Verifique que la clave `IMGBB_API_KEY` esté correctamente configurada en el archivo de entorno, ya que es necesaria para alojar imágenes y adjuntarlas al mensaje de WhatsApp.
* **Error al Procesar Archivos Excel:**
    * Asegure la compatibilidad manteniendo el paquete `openpyxl` en la versión `>= 3.1.0`.
    * Si el archivo de datos no cumple con la estructura esperada, puede usar `python create_sample_data.py` para generar una plantilla válida.

# Notas de Seguridad y Control de Versiones

Se han implementado medidas específicas para proteger credenciales y datos sensibles de ser expuestos en el repositorio:

* **Variables de Entorno:** El archivo `whatsapp_config.env` que almacena las claves sensibles (SID, tokens) de Twilio está incluido en el archivo `.gitignore` y **no debe ser subido** al repositorio. Utilice la plantilla `whatsapp_config.env.sample` como referencia.
* **Archivos Generados:** Los directorios `outputs/` (gráficos, logs, mensajes simulados) y `data/` (el archivo de ventas Excel) también están listados en `.gitignore` para evitar la subida accidental de datos o informes confidenciales.