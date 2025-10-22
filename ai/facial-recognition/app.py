from datetime import datetime
import io
import time
import pickle
import hashlib
from typing import Optional, Tuple
import subprocess
import sys
import os
from collections import deque
import logging
import configparser

import cv2
import numpy as np
import streamlit as st
from PIL import Image
import pandas as pd
import plotly.express as px
import psycopg2 
from psycopg2 import sql
from psycopg2.extras import DictCursor, register_uuid

# --- Configuration Loading Function ---
def load_db_config(filename='database.ini', section='postgresql'):
    """Reads database configuration parameters from a file."""
    parser = configparser.ConfigParser()
    parser.read(filename)

    db = {}
    if parser.has_section(section):
        params = parser.items(section)
        for param in params:
            db[param[0]] = param[1]
    else:
        raise Exception(f'Section {section} not found in the {filename} file')
    return db

# Set up logging 
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)

# Reuse core logic from local camera script
# NOTE: camera_local.py MUST ALSO BE UPDATED TO USE CONFIGPARSER or ENV VARS
from camera_local import (
    extract_embedding,
    analyze_emotion,
    analyze_emotion_full,
    recognize,
    load_person_embeddings,
)

# initial page config
st.set_page_config(
    page_title="Facial Recognition System",
    layout="wide",
)

class FacialRecognitionSystem():
    def __init__(self):
        self.db_config = load_db_config() # Load config here
        self.conn = None
        self.init_database()
        # Cache cascade to avoid reloading
        self._cascade = None

    # initialize database
    def init_database(self):
        try:
            # Connect to PostgreSQL using loaded config
            self.conn = psycopg2.connect(**self.db_config)
            
            # Ensure transactions are committed immediately for single-user web app
            self.conn.autocommit = True
            
            # Cursor that returns column names
            cursor = self.conn.cursor(cursor_factory=DictCursor)
            
            logger.info("Successful connection to PostgreSQL database")

            # tables - Using SERIAL for auto-increment and TEXT/REAL/BYTEA for data types

            # Person table
            cursor.execute("""
                CREATE TABLE IF NOT EXISTS Persona (
                    id SERIAL PRIMARY KEY,
                    nombre TEXT NOT NULL,
                    apellido TEXT NOT NULL,
                    email TEXT UNIQUE NOT NULL,
                    fecha_registro_timestamp TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
                )
            """)
            logger.info("Persona table created")

            # Face table
            cursor.execute("""
                CREATE TABLE IF NOT EXISTS Rostro (
                    id SERIAL PRIMARY KEY,
                    id_persona INTEGER NOT NULL REFERENCES Persona (id) ON DELETE CASCADE,
                    rostro BYTEA NOT NULL
                )
            """)
            logger.info("Rostro table created")

            # Detection table (v2 schema)
            cursor.execute("""
                CREATE TABLE IF NOT EXISTS Deteccion (
                    id SERIAL PRIMARY KEY,
                    id_persona INTEGER REFERENCES Persona (id) ON DELETE SET NULL,
                    recog_confianza REAL,
                    emocion TEXT NOT NULL,
                    emocion_confianza REAL NOT NULL,
                    timestamp TIMESTAMP WITH TIME ZONE DEFAULT CURRENT_TIMESTAMP
                )
            """)
            logger.info("Deteccion table created or updated")

            # Emotion detail table (full distribution per detection)
            cursor.execute(
                """
                CREATE TABLE IF NOT EXISTS DeteccionEmocionDetalle (
                    id SERIAL PRIMARY KEY,
                    id_deteccion INTEGER NOT NULL REFERENCES Deteccion (id) ON DELETE CASCADE,
                    emocion TEXT NOT NULL,
                    confianza REAL NOT NULL
                )
                """
            )
            logger.info("Emotion detail table created")

        except Exception as e:
            st.error(f"Error connecting or initializing PostgreSQL. Ensure 'database.ini' is configured correctly: {e}")
            logger.error(f"Database error: {e}")
            st.stop()
        finally:
            if 'cursor' in locals() and cursor:
                cursor.close()

    # --- Database interaction methods (remain largely the same using %s placeholders) ---
    
    def get_cascade(self):
        if self._cascade is None:
            from cv2 import data as cv2_data
            self._cascade = cv2.CascadeClassifier(
                cv2_data.haarcascades + 'haarcascade_frontalface_default.xml'
            )
        return self._cascade

    def get_all_persons(self):
        """Get all registered persons"""
        try:
            cursor = self.conn.cursor()
            cursor.execute("SELECT id, nombre, apellido, email FROM Persona ORDER BY nombre, apellido")
            return cursor.fetchall()
        except Exception as e:
            st.error(f"Error fetching persons: {e}")
            return []
        finally:
            if 'cursor' in locals() and cursor:
                cursor.close()

    def get_person_name(self, person_id):
        """Get person name by ID"""
        try:
            cursor = self.conn.cursor()
            cursor.execute("SELECT nombre, apellido FROM Persona WHERE id = %s", (person_id,))
            result = cursor.fetchone()
            return f"{result[0]} {result[1]}" if result else "Unknown"
        except Exception as e:
            st.error(f"Error fetching name: {e}")
            return "Unknown"
        finally:
            if 'cursor' in locals() and cursor:
                cursor.close()

# initialize system (Streamlit web page)
# ... (session state initializations remain the same)
if 'system' not in st.session_state:
    st.session_state.system = FacialRecognitionSystem()
if 'reg_captures' not in st.session_state:
    st.session_state.reg_captures = []
if 'reg_hashes' not in st.session_state:
    st.session_state.reg_hashes = set()
if 'reg_form_ok' not in st.session_state:
    st.session_state.reg_form_ok = False
if 'reg_form_data' not in st.session_state:
    st.session_state.reg_form_data = {"nombre":"", "apellido":"", "email":""}
if 'reg_embs' not in st.session_state:
    st.session_state.reg_embs = {}
if 'native_proc_pid' not in st.session_state:
    st.session_state.native_proc_pid = None


def main():
    st.title("Facial Recognition System")
    
    # --- New Layout: Tabs instead of Sidebar Radio ---
    tab1, tab2, tab3 = st.tabs(["Registration", "Detection", "Reports"])

    with tab1:
        show_registration_page()
    with tab2:
        show_detection_page()
    with tab3:
        show_reports_page()

# --- PAGE FUNCTIONS (UNCHANGED SQL LOGIC) ---

# emotions chart
def show_emotion_charts():
    st.subheader("Emotion Distribution")

    try: 
        cursor = st.session_state.system.conn.cursor(cursor_factory=DictCursor) 

        cursor.execute("""
            SELECT COALESCE(P.nombre || ' ' || P.apellido, 'Unknown') as persona,
                   D.emocion,
                   COUNT(*) as conteo
            FROM Deteccion D
            LEFT JOIN Persona P ON D.id_persona = P.id
            GROUP BY persona, D.emocion
        """)

        data = cursor.fetchall()
        cursor.close()

        if data:
            df = pd.DataFrame([dict(row) for row in data])
            df.rename(columns={
                'persona': 'Person',
                'emocion': 'Emotion',
                'conteo': 'Count'
            }, inplace=True)
            
            fig = px.bar(df, x='Person', y='Count', color='Emotion',
                         title='Emotion Distribution per Person')
            st.plotly_chart(fig, use_container_width=True)
        else:
            st.info("No detection data to display")
    except Exception as e:
        st.error(f"Error generating emotion chart: {e}")

# general stats
def show_general_stats():
    st.subheader("General Statistics")

    try:
        cursor = st.session_state.system.conn.cursor()

        col1, col2, col3, col4 = st.columns(4)

        # registered persons
        cursor.execute("SELECT COUNT(*) FROM Persona")
        total_personas = cursor.fetchone()[0]

        # detections today (using DATE_TRUNC)
        cursor.execute(
            """
            SELECT COUNT(*) FROM Deteccion
            WHERE timestamp >= DATE_TRUNC('day', NOW() AT TIME ZONE 'UTC')
            """
        )
        detecciones_hoy = cursor.fetchone()[0]

        # most common emotion
        cursor.execute(
            """
            SELECT emocion, COUNT(*) as count
            FROM Deteccion
            GROUP BY emocion
            ORDER BY count DESC
            LIMIT 1
            """
        )
        emocion_data = cursor.fetchone()
        emocion_predominante = emocion_data[0] if emocion_data else "N/A"

        # recognition rate
        cursor.execute("SELECT COUNT(*) FROM Deteccion WHERE recog_confianza IS NOT NULL AND recog_confianza > 0.7")
        reconocimientos_confiables = cursor.fetchone()[0]
        cursor.execute("SELECT COUNT(*) FROM Deteccion WHERE recog_confianza IS NOT NULL")
        total_reconocimientos = cursor.fetchone()[0]
        tasa_reconocimiento = (reconocimientos_confiables / total_reconocimientos * 100) if total_reconocimientos > 0 else 0

        # metrics
        with col1:
            st.metric("Total Registered Persons", total_personas)
        with col2:
            st.metric("Detections Today", detecciones_hoy)
        with col3:
            st.metric("Predominant Emotion", emocion_predominante)
        with col4:
            st.metric("Reliable Recognition Rate", f"{tasa_reconocimiento:.1f}%")

        # query for detections per hour (using EXTRACT)
        st.subheader("Hourly Detection Pattern")
        cursor.execute(
            """
            SELECT EXTRACT(HOUR FROM timestamp AT TIME ZONE 'UTC') as hora, COUNT(*) as count
            FROM Deteccion
            GROUP BY hora
            ORDER BY hora
            """
        )

        hora_data = cursor.fetchall()
        cursor.close()
        
        if hora_data:
            horas = [f"{int(h[0]):02d}:00" for h in hora_data]
            counts = [h[1] for h in hora_data]
            fig = px.line(
                x=horas,
                y=counts,
                title='Hourly Detection Pattern',
                labels={'x': 'Hour of the day', 'y': 'Number of detections'}
            )
            st.plotly_chart(fig, use_container_width=True)
    except Exception as e:
        st.error(f"Error displaying general statistics: {e}")
    finally:
        if 'cursor' in locals() and cursor:
            cursor.close()


# PAGE: Registration
def show_registration_page():
    # ... (Form and capture logic remains the same)
    st.header("Person Registration")
    with st.form("form-registro"):
        c1, c2, c3 = st.columns(3)
        with c1:
            nombre = st.text_input("First Name", value=st.session_state.reg_form_data.get("nombre", ""))
        with c2:
            apellido = st.text_input("Last Name", value=st.session_state.reg_form_data.get("apellido", ""))
        with c3:
            email = st.text_input("Email", value=st.session_state.reg_form_data.get("email", ""))
        submitted = st.form_submit_button("Confirm Data")

    if submitted:
        if not (nombre and apellido and email):
            st.error("Please complete first name, last name, and email.")
        else:
            st.success("Data validated. Capture images below.")
            st.session_state.reg_form_ok = True
            st.session_state.reg_form_data = {"nombre": nombre, "apellido": apellido, "email": email}

    st.markdown("---")
    st.subheader("Face Captures")
    st.caption("Take at least 5 sharp, well-lit, and centered face photos.")

    c1, c2 = st.columns([2,1])
    with c1:
        img_upload = st.camera_input("Take photo", disabled=not st.session_state.reg_form_ok)
        col_btn = st.columns(1)[0]
        with col_btn:
            if st.button("Clear Captures"):
                st.session_state.reg_captures = []
                st.session_state.reg_hashes = set()
                st.session_state.reg_embs = {}
        if img_upload is not None:
            raw = img_upload.getvalue()
            h = hashlib.sha256(raw).hexdigest()
            if h not in st.session_state.reg_hashes:
                st.session_state.reg_captures.append(raw)
                st.session_state.reg_hashes.add(h)
                st.success("Capture added")
    with c2:
        st.write(f"Accumulated captures: {len(st.session_state.reg_captures)}")
        save_now = st.button("Save to Database", disabled=(not st.session_state.reg_form_ok or len(st.session_state.reg_captures) == 0))

    # Preview de thumbnails
    if st.session_state.reg_captures:
        cols = st.columns(3)
        for i, raw in enumerate(st.session_state.reg_captures[-6:]):
            with cols[i % 3]:
                st.image(Image.open(io.BytesIO(raw)), caption=f"Capture #{i+1}", use_container_width=True)

    # Save logic
    if save_now:
        try:
            conn = st.session_state.system.conn
            cur = conn.cursor()
            person_id = None
            
            # Check if person exists
            cur.execute("SELECT id FROM Persona WHERE email = %s", (st.session_state.reg_form_data["email"],))
            row = cur.fetchone()
            
            if row:
                person_id = row[0]
            else:
                # INSERT and retrieve the new ID using RETURNING
                cur.execute(
                    "INSERT INTO Persona(nombre, apellido, email) VALUES(%s, %s, %s) RETURNING id",
                    (st.session_state.reg_form_data["nombre"], st.session_state.reg_form_data["apellido"], st.session_state.reg_form_data["email"])
                )
                person_id = cur.fetchone()[0] 

            saved = 0
            cascade = st.session_state.system.get_cascade()
            for raw in st.session_state.reg_captures:
                try:
                    img = Image.open(io.BytesIO(raw)).convert("RGB")
                    frame_rgb = np.array(img)
                    
                    # Detection and cropping logic remains the same
                    gray = cv2.cvtColor(frame_rgb, cv2.COLOR_RGB2GRAY)
                    faces = cascade.detectMultiScale(gray, 1.1, 5, minSize=(60, 60))
                    
                    if len(faces) > 0:
                        (x, y, w, h) = max(faces, key=lambda b: b[2]*b[3])
                        x0 = max(0, x); y0 = max(0, y)
                        x1 = min(frame_rgb.shape[1], x + w)
                        y1 = min(frame_rgb.shape[0], y + h)
                        crop_rgb = frame_rgb[y0:y1, x0:x1]
                    else:
                        h_f, w_f = frame_rgb.shape[:2]
                        cx0 = int(w_f*0.2); cy0 = int(h_f*0.2)
                        cx1 = int(w_f*0.8); cy1 = int(h_f*0.8)
                        crop_rgb = frame_rgb[cy0:cy1, cx0:cx1]

                    emb = extract_embedding(crop_rgb)
                    if emb is None:
                        continue
                        
                    blob = pickle.dumps(emb) 
                    cur.execute("INSERT INTO Rostro(id_persona, rostro) VALUES(%s, %s)", (person_id, blob))
                    saved += 1
                except Exception as e:
                    logger.error(f"Error saving face capture: {e}")
                    continue
            
            cur.close()
            st.success(f"Saved: {saved} captures for {st.session_state.reg_form_data['nombre']} {st.session_state.reg_form_data['apellido']}")
            
            # clean state
            st.session_state.reg_captures = []
            st.session_state.reg_hashes = set()
            st.session_state.reg_embs = {}
            st.session_state.reg_form_ok = False
        except Exception as e:
            st.error(f"Could not save: {e}")


# PAGE: Detection
def show_detection_page():
    st.header("Real-Time Detection (Native OpenCV)")
    st.caption("A native window with video and overlay will open. Close with 'q'.")

    # --- Reorganizing UI with columns for better use of wide layout ---
    
    st.subheader("Detection & Recognition Settings")
    c_rec1, c_rec2, c_rec3 = st.columns(3)
    with c_rec1:
        threshold = st.slider("Recognition Threshold (cosine)", 0.5, 0.95, 0.6, 0.01)
        infer_ms = st.slider("Inference Interval (ms)", 100, 2000, 500, 50)
    with c_rec2:
        detect_scale = st.slider("Face Detection Scale", 0.2, 1.0, 0.5, 0.05)
        infer_scale = st.slider("Inference Scale (Face)", 0.3, 1.0, 0.5, 0.05)
    with c_rec3:
        detector_backend = st.selectbox("Face Detector Backend (Green Box)", options=['opencv','opencv-dnn','retinaface','mediapipe'], index=0)
        embed_model = st.selectbox("Embeddings Model", options=['ArcFace','Facenet','VGG-Face'], index=1)

    st.subheader("Emotion Analysis Settings")
    c_emo1, c_emo2, c_emo3 = st.columns(3)
    with c_emo1:
        emo_backend = st.selectbox("Emotion Backend (DeepFace)", options=['opencv','retinaface','mediapipe','skip'], index=0)
        emo_ms = st.slider("Emotion Interval (ms)", 500, 10000, 3000, 50)
        disable_emotion = st.checkbox("Disable Emotions (Max FPS)", value=False)
    with c_emo2:
        crop_padding = st.slider("Emotion Crop Padding", 0.0, 0.3, 0.15, 0.01)
        emo_scale = st.slider("Emotion Scale (Upscale)", 1.0, 2.0, 1.2, 0.1)
        emo_smooth = st.slider("Emotion Smoothing (Frames)", 1, 15, 5, 1)
    with c_emo3:
        emo_disgust_gain = st.slider("Disgust Gain (Calibration)", 0.5, 3.0, 1.6, 0.1, help="Multiplies 'disgust' probability before normalizing. Useful if it never appears.")
        emo_balance = st.checkbox("Balance Emotions (Uniform 7 classes)", value=True, help="Re-weights by prior (EMA) so no emotion is underrepresented")
        emo_balance_strength = st.slider("Balance Strength (Beta)", 0.0, 2.0, 1.0, 0.1, help="0 = no effect; 1 = total balance; >1 overcompensates")
        emo_balance_alpha = st.slider("Balance Alpha (EMA)", 0.05, 0.5, 0.15, 0.01, help="How quickly the emotion prior updates")
        
    st.subheader("System & Logging")
    c_sys1, c_sys2, c_sys3 = st.columns(3)
    with c_sys1:
        min_log_ms = st.slider("Min Save Interval (ms)", 1000, 20000, 5000, 100, help="Limits the frequency of database writes. A new emotion will be saved even if the interval isn't met.")
    with c_sys2:
        force_mjpg = st.checkbox('Force MJPG (Reduces CPU latency)', value=True, key='mjpg')
        target_fps = st.slider('Target FPS (Attempt)', 0, 60, 30, 1, key='fps')
    with c_sys3:
        box_alpha = st.slider('Box Smoothing (Alpha)', 0.0, 1.0, 0.5, 0.05, key='boxalpha')

    st.subheader("Camera Settings")
    colr1, colr2, colr3 = st.columns(3)
    with colr1:
        res_option = st.selectbox(
            "Resolution",
            ["640x480 (SD)", "1280x720 (HD)", "1920x1080 (FHD)", "Custom"],
            index=1,
            help="Increasing resolution improves quality but may reduce FPS"
        )
    with colr2:
        cam_idx = st.number_input("Camera Index", min_value=0, max_value=10, value=0, step=1)

    cam_w, cam_h = 1280, 720 # Default HD
    if res_option == "640x480 (SD)":
        cam_w, cam_h = 640, 480
    elif res_option == "1280x720 (HD)":
        cam_w, cam_h = 1280, 720
    elif res_option == "1920x1080 (FHD)":
        cam_w, cam_h = 1920, 1080
    else:
        with colr3:
            cwx, cwy = st.columns(2)
            with cwx:
                cam_w = st.number_input("Width (px)", min_value=320, max_value=3840, value=1280, step=16)
            with cwy:
                cam_h = st.number_input("Height (px)", min_value=240, max_value=2160, value=720, step=16)
    
    st.markdown("---")
    
    colb1, colb2, colb3 = st.columns([1,1,2])
    can_start = st.session_state.native_proc_pid is None
    with colb1:
        if st.button("Start", disabled=not can_start):
            try:
                # Build command as before
                cmd = [
                    sys.executable,
                    os.path.join(os.getcwd(), 'camera_local.py'),
                    'detect',
                    '--camera', str(int(cam_idx)),
                    '--threshold', str(float(threshold)),
                    '--infer-interval-ms', str(int(infer_ms)),
                    '--scale', str(float(infer_scale)),
                    '--detect-scale', str(float(detect_scale)),
                    '--frame-width', str(int(cam_w)),
                    '--frame-height', str(int(cam_h)),
                    '--detector-backend', str(detector_backend),
                    '--embed-model', str(embed_model),
                ]
                # Add emotion flags
                if not disable_emotion and emo_backend != 'skip':
                    cmd += [
                        '--emotion-interval-ms', str(int(emo_ms)),
                        '--emotion-backend', str(emo_backend),
                        '--emo-smooth-frames', str(int(emo_smooth)),
                        '--emotion-scale', str(float(emo_scale)),
                        '--crop-padding', str(float(crop_padding)),
                        '--emo-disgust-gain', str(float(emo_disgust_gain)),
                        '--emo-balance-strength', str(float(emo_balance_strength)),
                        '--emo-balance-alpha', str(float(emo_balance_alpha)),
                        '--min-log-interval-ms', str(int(min_log_ms)),
                    ]
                    if emo_balance:
                        cmd += ['--emo-balance']
                else:
                    cmd += ['--no-emotion']
                # Add tuning flags
                if force_mjpg:
                    cmd += ['--force-mjpg']
                if target_fps > 0:
                    cmd += ['--target-fps', str(int(target_fps))]
                cmd += ['--box-smooth-alpha', str(float(box_alpha))]
                
                # --- Pass config file path to subprocess via ENV VAR ---
                env = os.environ.copy()
                env['DB_CONFIG_FILE'] = 'database.ini' 
                
                creationflags = 0
                if os.name == 'nt' and hasattr(subprocess, 'CREATE_NEW_CONSOLE'):
                    creationflags = subprocess.CREATE_NEW_CONSOLE
                proc = subprocess.Popen(cmd, creationflags=creationflags, env=env)
                st.session_state.native_proc_pid = proc.pid
                st.success(f"Native detection started (PID {proc.pid}). Close with 'q' in the window or use Stop.")
            except Exception as e:
                st.error(f"Could not start native detection. Is 'camera_local.py' present? Error: {e}")
    with colb2:
        stop_enabled = st.session_state.native_proc_pid is not None
        if st.button("Stop", disabled=not stop_enabled):
            try:
                pid = st.session_state.native_proc_pid
                if pid is not None:
                    if os.name == 'nt':
                        subprocess.call(['taskkill', '/PID', str(pid), '/F'], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
                    else:
                        os.kill(pid, 9)
                    st.session_state.native_proc_pid = None
                    st.info("Native detection stopped")
            except Exception as e:
                st.error(f"Could not stop: {e}")
    with colb3:
        pid = st.session_state.native_proc_pid
        st.write(f"Status: {'Running (PID '+str(pid)+')' if pid else 'Stopped'}")


# PAGE: Reports
def show_reports_page():
    st.header("Reports")
    show_general_stats()
    st.markdown("---")
    show_emotion_charts()
    st.markdown("---")
    st.subheader("Latest Detections")
    # Latest detections
    try:
        cur = st.session_state.system.conn.cursor()
        cur.execute(
            """
            SELECT D.timestamp, COALESCE(P.nombre || ' ' || P.apellido, 'Unknown') as persona,
                   D.recog_confianza, D.emocion, D.emocion_confianza
            FROM Deteccion D
            LEFT JOIN Persona P ON D.id_persona = P.id
            ORDER BY D.timestamp DESC
            LIMIT 100
            """
        )
        rows = cur.fetchall()
        cur.close()
        if rows:
            df = pd.DataFrame(rows, columns=["Timestamp", "Person", "Recog. Confidence", "Emotion", "Emotion Confidence"])
            st.dataframe(df, use_container_width=True)
        else:
            st.info("No detections registered")
    except Exception as e:
        st.error(f"Error loading detections: {e}")


if __name__ == "__main__":
    main()