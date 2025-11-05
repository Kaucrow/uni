from datetime import datetime
import os
import sys
from utils.data_loader import load_and_validate_data    
from utils.analyzer import DataAnalyzer, analyze_data
from utils.visualizer import generate_visualizations
from utils.whatsapp_sender import WhatsAppSender, send_whatsapp_report

# Códigos de color ANSI
class Colors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

# create directories if not exist

def setup_directories():
    print(f"{Colors.OKBLUE}{Colors.BOLD}--- Configuración de Directorios ---{Colors.ENDC}")
    directories = ['data', 'outputs/graphs', 'utils']
    for directory in directories:
        os.makedirs(directory, exist_ok=True)
        print(f"[{Colors.OKGREEN}ÉXITO{Colors.ENDC}] Carpeta '{directory}' verificada.")
    print(f"{Colors.OKBLUE}-----------------------------------{Colors.ENDC}")


# load enviroment variables

def load_env_variables():
    try: 
        from dotenv import load_dotenv
        # load default .env first
        load_dotenv()
        # if a project-specific env file exists, load it (whatsapp_config.env)
        env_file = 'whatsapp_config.env'
        if os.path.exists(env_file):
            load_dotenv(env_file, override=True)
            print(f"[{Colors.OKGREEN}ÉXITO{Colors.ENDC}] Variables de entorno cargadas desde {Colors.UNDERLINE}{env_file}{Colors.ENDC}.")
        else:
            print(f"[{Colors.WARNING}INFO{Colors.ENDC}] Variables de entorno cargadas (desde .env si existe).")
    except ImportError:
        print(f"[{Colors.FAIL}ERROR{Colors.ENDC}] {Colors.BOLD}python-dotenv no está instalado.{Colors.ENDC} Asegúrese de que las variables de entorno estén configuradas manualmente.")

def main():
    # Mensaje de inicio en Azul y Negrita
    print(f"{Colors.OKBLUE}{Colors.BOLD}============================================================{Colors.ENDC}")
    print(f"{Colors.OKBLUE}{Colors.BOLD} Iniciando Proceso RPA de Análisis de Ventas {Colors.ENDC}")
    print(f"{Colors.OKBLUE}{Colors.BOLD}Hora de Inicio: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}{Colors.ENDC}")
    print(f"{Colors.OKBLUE}{Colors.BOLD}============================================================{Colors.ENDC}")

    # setup directories and load env variables
    setup_directories()
    load_env_variables()

    # verify if file exists
    data_file = 'data/Ventas_Fundamentos.xlsx'

    if not os.path.exists(data_file):
        # Error en Rojo
        print(f"\n{Colors.FAIL}{Colors.BOLD}Archivo de datos no encontrado:{Colors.ENDC} {data_file}")
        print(f"{Colors.FAIL}Por favor, ejecute 'create_sample_data.py' para generar el archivo de datos de muestra.{Colors.ENDC}")
        sys.exit(1)

    print(f"{Colors.HEADER}\n============================================================{Colors.ENDC}")
    # load and validate data
    print(f"{Colors.HEADER} Cargando y validando datos...{Colors.ENDC}")
    print(f"{Colors.HEADER}============================================================{Colors.ENDC}")

    df, validation = load_and_validate_data(data_file)

    if df is not None and validation['is_valid']:
        # Éxito en Verde
        print(f"[{Colors.OKGREEN}ÉXITO{Colors.ENDC}] Datos cargados y validados exitosamente.")
        print(f"{Colors.WARNING}--- Resumen de Datos ---{Colors.ENDC}")
        print(f"  Total registros: {Colors.BOLD}{len(df):,}{Colors.ENDC}")
        print(f"  Sedes Únicas: {Colors.BOLD}{df['Headquarter'].nunique():,}{Colors.ENDC}")
        print(f"  Modelos Únicos: {Colors.BOLD}{df['Model'].nunique():,}{Colors.ENDC}")
        print(f"  Clientes Únicos: {Colors.BOLD}{df['Client_ID'].nunique():,}{Colors.ENDC}")
        print(f"{Colors.WARNING}------------------------{Colors.ENDC}")
    else:
        # Error en Rojo
        print(f"[{Colors.FAIL}ERROR{Colors.ENDC}] {Colors.FAIL}{Colors.BOLD}Error en la carga de datos.{Colors.ENDC}")
        if 'error' in validation:
            print(f"{Colors.FAIL}Detalle: {validation['error']}{Colors.ENDC}")
        sys.exit(1)

    # analyze data
    print(f"\n{Colors.HEADER}============================================================{Colors.ENDC}")
    print(f"{Colors.HEADER} Iniciando análisis de datos...{Colors.ENDC}")
    print(f"{Colors.HEADER}============================================================{Colors.ENDC}")
    try:
        analyzer = DataAnalyzer(df)
        results = analyzer.full_analysis()
        
        # show summary

        print(f"\n{Colors.OKBLUE}{Colors.BOLD}--- Resumen del Análisis ---{Colors.ENDC}")
        metrics = results['summary_metrics']
        print(f"  Clientes Únicos: {Colors.BOLD}{metrics['unique_clients']:,}{Colors.ENDC}")
        print(f"  Total de Ventas (Registros): {Colors.BOLD}{metrics['total_sales']:,}{Colors.ENDC}")
        print(f"  Ventas Totales sin IGV: {Colors.OKGREEN}${metrics['total_sales_without_igv']:,.2f}{Colors.ENDC}")
        print(f"  Ventas Totales con IGV: {Colors.OKGREEN}${metrics['total_sales_with_igv']:,.2f}{Colors.ENDC}")
        print(f"  Ventas Promedio: {Colors.WARNING}${metrics['average_sales_without_igv']:,.2f}{Colors.ENDC}")
        print(f"{Colors.OKBLUE}------------------------------------------{Colors.ENDC}")
        
        # Top ítems en color para resaltarlos
        print(f"Modelo Más Vendido: {Colors.BOLD}{results['top_models'].index[0]}{Colors.ENDC}")
        print(f"Sede con Más Ventas: {Colors.BOLD}{results['sales_by_headquarter'].index[0]}{Colors.ENDC}")
        print(f"Canal con Más Ventas: {Colors.BOLD}{results['sales_by_channel'].index[0]}{Colors.ENDC}")

    except Exception as e:
        # Error en Rojo
        print(f"\n[{Colors.FAIL}ERROR{Colors.ENDC}] {Colors.FAIL}Error durante el análisis de datos: {str(e)}{Colors.ENDC}")
        sys.exit(1)

    # generate graphs
    print(f"\n{Colors.HEADER}Generando visualizaciones...{Colors.ENDC}")
    try:
        generate_visualizations(results)
        # Éxito en Verde
        print(f"[{Colors.OKGREEN}ÉXITO{Colors.ENDC}] Visualizaciones generadas exitosamente en '{Colors.UNDERLINE}outputs/graphs{Colors.ENDC}'.")
    except Exception as e:
        # Error en Rojo
        print(f"[{Colors.FAIL}ERROR{Colors.ENDC}] {Colors.FAIL}Error durante la generación de visualizaciones: {str(e)}{Colors.ENDC}")
        sys.exit(1)


    # send whatsapp report
    print(f"\n{Colors.HEADER}📱 Enviando reporte por WhatsApp (Twilio)...{Colors.ENDC}")
    try:
        # get whatsapp destiny
        destiny = os.getenv("WHATSAPP_DESTINY")
        
        print(f"  Método de envío: {Colors.BOLD}TWILIO{Colors.ENDC}")
        if destiny:
            print(f"  Número de destino: {Colors.OKBLUE}{destiny}{Colors.ENDC}")
        else:
            print(f"  [{Colors.WARNING}ALERTA{Colors.ENDC}] Número de destino no encontrado en variables de entorno.")

        # normalize destiny (strip whitespace) before using
        if destiny:
            destiny = destiny.strip()

        # send report
        if destiny and send_whatsapp_report(results, destiny):
            # Éxito en Verde
            print(f"[{Colors.OKGREEN}ÉXITO{Colors.ENDC}] Reporte enviado exitosamente por WhatsApp.")
        
        else:
            # Error/Alerta en Rojo/Amarillo
            if destiny:
                 print(f"[{Colors.FAIL}ERROR{Colors.ENDC}] Error al enviar el reporte por WhatsApp (revisar logs/configuración de Twilio).")
            else:
                 print(f"[{Colors.WARNING}ALERTA{Colors.ENDC}] Envío omitido. No se encontró número de destino.")
    except Exception as e:
        # Error en Rojo
        print(f"[{Colors.FAIL}ERROR{Colors.ENDC}] Error durante el envío del reporte por WhatsApp: {e}")
        # exit program

    # Mensaje final en Verde y Negrita
    print(f"\n{Colors.OKGREEN}{Colors.BOLD}============================================================{Colors.ENDC}")
    print(f"{Colors.OKGREEN}{Colors.BOLD} PROCESO RPA COMPLETADO EXITOSAMENTE{Colors.ENDC}")
    print(f"{Colors.OKGREEN}{Colors.BOLD} Hora de Finalización: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}{Colors.ENDC}")
    print(f"{Colors.OKGREEN}{Colors.BOLD}============================================================{Colors.ENDC}")

if __name__ == "__main__":
    main()