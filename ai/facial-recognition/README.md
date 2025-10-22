# Real-Time Face Recognition and Emotion Analysis

## Project Overview

This project, implemented in Python using **OpenCV**, **DeepFace**, and **PostgreSQL**, provides a robust system for real-time face detection, recognition, and emotion analysis from a local webcam stream. It is designed to run in two distinct modes: **Registration** (to enroll new people) and **Detection** (to perform real-time analysis and logging).

The system uses advanced face recognition models (like **Facenet**) for identity and a separate pipeline for in-depth emotion analysis and smoothing, with all detection data persisted to a PostgreSQL database.

## Features

- **Dual Mode Operation**: Separate modes for `register` (enrollment) and `detection` (real-time analysis).
- **DeepFace Integration**: Leverages powerful models for face embedding (e.g., VGG-Face, Facenet, ArcFace) and emotion analysis.
- **Multiple Detector Backends**: Supports various face detection methods (e.g., MediaPipe, RetinaFace, OpenCV-DNN) for flexibility and performance.
- **PostgreSQL Persistence**: Logs all recognition and emotion events, including full emotion distributions, to a database.
- **Performance Optimized**: Uses multi-threading for separate camera capture and inference loops to maximize frame rate and responsiveness.
- **Emotion Smoothing**: Implements frame-based averaging and advanced balancing techniques to stabilize and refine real-time emotion results.

## Prerequisites

Before running the project, ensure you have the following installed:

1. **Python 3.8+**
2. **PostgreSQL Database**: A running instance is required for storing user data and detection logs.
3. **Required Libraries**: Install the Python dependencies.

### Installation

1. **Clone the repository (assuming a similar structure):**
   ```bash
   git clone [your-repo-link]
   cd [your-project-directory]
   ```

2. **Install dependencies:**
   ```bash
   pip install -r requirements.txt
   # If a requirements.txt is not available, install the key libraries:
   pip install opencv-python numpy deepface scikit-learn psycopg2-binary
   ```

3. **Database Configuration:**

   Create a configuration file (e.g., `db_config.ini`) to hold your PostgreSQL connection details.

   **`db_config.ini` Example:**
   ```ini
   [postgresql]
   host=localhost
   database=face_analysis_db
   user=your_db_user
   password=your_db_password
   port=5432
   ```

   You must set an environment variable pointing to this file. The script expects the variable **`DB_CONFIG_FILE`**.

   **Linux/macOS:**
   ```bash
   export DB_CONFIG_FILE=/path/to/db_config.ini
   ```

   **Windows (Command Prompt):**
   ```bash
   set DB_CONFIG_FILE=C:\path\to\db_config.ini
   ```

## Usage

The script is run from the command line using the `mode` argument: `register` or `detection`.

### 1. Registration Mode (`register`)

Use this mode to create new users and enroll their faces into the database. A minimum of **5 successful captures** is recommended for each person.

#### **Creating a New Person and Registering Faces**

```bash
python camera_local.py register \
    --nombre "John" \
    --apellido "Doe" \
    --email "john.doe@example.com"
```

- The script will open your camera.
- Align your face with the bounding box.
- Press `c` to capture an embedding.
- Press `q` to quit.

#### **Adding More Faces to an Existing Person**

If the person already exists, use the `--person-id` flag instead of the name and email arguments:

```bash
python camera_local.py register \
    --person-id 1 \
    --force  # Use --force to bypass duplicate checks if necessary
```

---

### 2. Detection Mode (`detection`)

Use this mode for real-time analysis and logging of recognized individuals and their dominant emotions.

```bash
python camera_local.py detection \
    --camera 0 \
    --threshold 0.55 \
    --embed-model "Facenet" \
    --detector-backend "mediapipe" \
    --infer-interval-ms 150 \
    --emo-smooth-frames 5
```

#### **Key Detection Arguments**

| Argument             | Description                                                  | Default     |
|----------------------|--------------------------------------------------------------|-------------|
| `--camera`           | Index of the webcam to use (e.g., 0, 1).                      | `0`         |
| `--threshold`        | Minimum similarity score (0.0 to 1.0) for recognition.        | `0.55`      |
| `--embed-model`      | DeepFace model for recognition (e.g., Facenet).              | `Facenet`   |
| `--detector-backend` | Face detection backend (e.g., retinaface, mediapipe).        | `mediapipe` |
| `--infer-interval-ms`| Minimum delay between full inference cycles.                 | `100`       |
| `--emo-smooth-frames`| Frames to average emotion results over for stability.        | `3`         |
| `--no-emotion`       | Flag to disable emotion analysis for performance.            | (disabled)  |

---

## Database Schema

The script automatically initializes four tables in your configured PostgreSQL database:

### **Persona**

Stores basic personal information.

- `id` (SERIAL, PK)  
- `nombre` (TEXT)  
- `apellido` (TEXT)  
- `email` (TEXT, UNIQUE)  
- `fecha_registro_timestamp` (TIMESTAMP WITH TIME ZONE)

### **Rostro**

Stores face embeddings for recognition.

- `id` (SERIAL, PK)  
- `id_persona` (INTEGER, FK to Persona)  
- `rostro` (BYTEA, stores the pickled embedding vector)

### **Deteccion**

Stores the log of each significant detection event.

- `id` (SERIAL, PK)  
- `id_persona` (INTEGER, FK to Persona or NULL if unknown)  
- `recog_confianza` (REAL, recognition confidence)  
- `emocion` (TEXT, dominant emotion)  
- `emocion_confianza` (REAL, dominant emotion confidence)  
- `timestamp` (TIMESTAMP WITH TIME ZONE)

### **DeteccionEmocionDetalle**

Stores the full 7-class emotion distribution for each detection.

- `id` (SERIAL, PK)  
- `id_deteccion` (INTEGER, FK to Deteccion)  
- `emocion` (TEXT)  
- `confianza` (REAL)

## Configuration (`database.ini`)

The script relies on the `database.ini` configuration file for connecting to the PostgreSQL database. This file is **mandatory** for both registration and detection modes.