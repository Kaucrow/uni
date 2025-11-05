import matplotlib.pyplot as plt
import pandas as pd
import numpy as np
import os 
import logging
from matplotlib import rcParams
from typing import Dict, Any, List

logger = logging.getLogger(__name__)

class DataVisualizer:
    
    # class to generate graphs from sales data

    def __init__(self, results: Dict[str, Any]):

        # initialize with analysis results

        self.results = results
        
        # styles

        self.configure_styles()

    # configure graph styles

    def configure_styles(self):

        plt.style.use('ggplot') 
        rcParams['figure.figsize'] = (12, 8)
        rcParams['font.size'] = 12
        rcParams['axes.titlesize'] = 16
        rcParams['axes.labelsize'] = 14

        # PALETA DE COLORES EVERFOREST (Dark) - Ampliada para más variedad
        # He añadido más tonos para tener suficientes colores únicos para los gráficos de pastel/dona.
        self.colors = [
            '#A7C080',  # green (Principal para Tendencia)
            '#7FBBB3',  # blue (Principal para Sede)
            '#DBBC7F',  # yellow (Principal para Promedio)
            '#E67E80',  # red (Principal para Segmento y Resumen)
            '#D699B6',  # purple (Principal para Modelos)
            '#83C092',  # aqua
            '#AF9665',  # light yellow-green
            '#64734C',  # dark green
            '#8EC07C',  # brighter green
            '#D3C6AA',  # lighter gray-beige
            '#B57614',  # orange
            '#427B58',  # darker blue-green
        ]
        
    # save graph to file

    def save_graph(self, filename: str, dpi: int = 300):

        try: 
            abs_path = os.path.join('outputs', 'graphs', filename)
            plt.savefig(abs_path, dpi=dpi, bbox_inches='tight', facecolor= 'white', edgecolor='white')
            plt.close()
            logger.info(f"Graph saved to {abs_path}")
        except Exception as e:
            logger.error(f"Error saving graph: {str(e)}")
            raise

    # create graph for sales by headquarter (Mantenemos Barras Verticales)

    def create_sales_by_headquarter_graph(self):

        try:
            
            data= self.results['sales_by_headquarter']

            fig, ax = plt.subplots(figsize=(14,8))
            # Usamos los primeros colores de Everforest para asegurar variedad si hay muchas sedes
            # Se seleccionan `len(data)` colores de la paleta.
            bars = ax.bar(data.index, data.values, color=self.colors[:len(data)], edgecolor='black', linewidth=0.5) 
        
            ax.set_title('VENTAS SIN IGV POR SEDE', fontsize=18, fontweight='bold', pad=20)
            ax.set_xlabel('Sede', fontweight='bold')
            ax.set_ylabel('Ventas Sin IGV ($)', fontweight='bold')

            for bar in bars:
                height = bar.get_height()
                ax.text(bar.get_x() + bar.get_width()/2., height + height*0.01,
                        f'S/ {height:,.0f}', ha='center', va='bottom', fontweight='bold', fontsize=10) 
                
            plt.xticks(rotation=45, ha='right')
            plt.tight_layout()

            self.save_graph('sales_by_headquarter.png')

        except Exception as e:
            logger.error(f"Error creando gráfico de ventas por sede: {str(e)}")
            raise

    # create graph for top n models (Mantenemos Barras Horizontales)

    def create_top_models_graph(self):

        try:

            data = self.results['top_models']

            fig, ax = plt.subplots(figsize=(14,8))
            # Usamos colores diferentes para cada barra, tomando del inicio de la paleta
            bars = ax.barh(range(len(data)), data.values, color=self.colors[:len(data)], edgecolor='black', linewidth=0.5) 

            ax.set_title('TOP MODELOS MÁS VENDIDOS (SIN IGV)', fontsize=18, fontweight='bold', pad=20)
            ax.set_xlabel('Ventas Sin IGV ($)', fontweight='bold')
            ax.set_ylabel('Modelo', fontweight='bold')
            ax.set_yticks(range(len(data)))
            ax.set_yticklabels(data.index)

            for i, bar in enumerate(bars):
                width = bar.get_width()
                ax.text(width + width*0.01, bar.get_y() + bar.get_height()/2.,
                        f'S/ {width:,.0f}', ha='left', va='center', fontweight='bold', fontsize=10)
                
            plt.tight_layout()
            self.save_graph('top_models.png')
        
        except Exception as e:
            logger.error(f"Error creando gráfico de los mejores modelos: {str(e)}")
            raise

    # create graph for sales by channel (Gráfico de Dona)

    def create_sales_by_channel_graph(self):

        try: 
            data = self.results['sales_by_channel'] 
            
            fig, ax = plt.subplots(figsize=(12, 10))

            # Aseguramos colores únicos para cada canal. 
            # Tomamos los primeros 'n' colores de la paleta.
            num_channels = len(data)
            pie_colors = self.colors[:num_channels] # Seleccionar los primeros N colores

            wedges, texts, autotexts = ax.pie(
                data.values, 
                autopct='%1.1f%%', 
                startangle=90, 
                colors=pie_colors, 
                wedgeprops={'edgecolor': 'black', 'linewidth': 1}
            )

            centre_circle = plt.Circle((0,0), 0.70, fc='white', edgecolor='black', linewidth=1)
            fig.gca().add_artist(centre_circle)

            for autotext in autotexts:
                autotext.set_color('white')
                autotext.set_fontweight('bold')
                autotext.set_fontsize(11)

            ax.set_title('DISTRIBUCIÓN DE VENTAS POR CANAL (Unidades)', fontsize=18, fontweight='bold', pad=20)
            ax.axis('equal') 

            legend_labels = [f'{label}: {value:,.0f} unidades' 
                             for label, value in zip(data.index, data.values)]
            
            ax.legend(wedges, legend_labels, title="Canales", 
                      loc="center left", bbox_to_anchor=(1, 0, 0.5, 1))

            plt.tight_layout()
            self.save_graph('sales_by_channel.png')

        except Exception as e:
            logger.error(f"Error creando gráfico de ventas por canal: {str(e)}")
            raise

    # create graph for sales by segment (Gráfico de Pastel)

    def create_sales_by_segment_graph(self):

        try: 
            data= self.results['sales_by_segment']
            fig, ax = plt.subplots(figsize=(12,12))

            # Aseguramos colores únicos para cada segmento. 
            # Tomamos los siguientes 'n' colores de la paleta para que sean diferentes a los de 'Ventas por Canal'.
            num_segments = len(data)
            # Empezamos a seleccionar colores desde un índice diferente para evitar repeticiones con el gráfico anterior
            pie_colors = self.colors[num_segments:num_segments*2] if len(self.colors) >= num_segments*2 else self.colors[:num_segments]
            # Una alternativa más simple y segura es simplemente usar los primeros N colores si la paleta es suficientemente larga
            # pie_colors = self.colors[:num_segments] # Si quieres que ambos empiecen con los mismos colores
            
            # Para garantizar que sean diferentes a 'sales_by_channel' pero aún únicos:
            # Seleccionamos un rango de colores que no se superponga o tomamos del final si la paleta es grande.
            start_index_segment = len(self.results['sales_by_channel']) # Empieza después de los colores usados por 'by_channel'
            available_colors = self.colors[start_index_segment:]
            if len(available_colors) < num_segments: # Si no hay suficientes, volvemos a usar la paleta desde el inicio
                pie_colors = self.colors[:num_segments]
            else:
                pie_colors = available_colors[:num_segments]


            wedges, texts, autotexts = ax.pie(data.values, labels=data.index, autopct='%1.1f%%', startangle=90, colors=pie_colors, textprops={'fontsize': 12}) 

            for autotext in autotexts:
                autotext.set_color('white')
                autotext.set_fontweight('bold')
                autotext.set_fontsize(11)

            for text in texts:
                text.set_fontsize(13)
                text.set_fontweight('bold')

            ax.set_title('SEGMENTACIÓN DE VENTAS POR CLIENTE (SIN IGV)', fontsize=18, fontweight='bold', pad=20)

            legend_labels = [f'{label}: S/ {value:,.0f}' 
                             for label, value in zip(data.index, data.values)]
            ax.legend(wedges, legend_labels, title="Montos Totales", 
                      loc="center left", bbox_to_anchor=(1, 0, 0.5, 1))
            
            plt.tight_layout()
            self.save_graph('sales_by_segment.png')

        except Exception as e:
            logger.error(f"Error creando gráfico de ventas por segmento: {str(e)}")
            raise
    
    # create monthly sales trend graph (Gráfico de Líneas con Promedio)

    def create_monthly_sales_trend_graph(self):

        try:

            data = self.results['monthly_sales_trend']
            
            if data is None or data.empty:
                logger.warning("No hay datos para crear la gráfica de tendencia de ventas mensuales.")
                return
            
            fig, ax = plt.subplots(figsize=(15,8))

            months = data.index.astype(str)

            ax.plot(months, data.values, marker='o', color=self.colors[0], linewidth=3, markersize=8, markerfacecolor='white', markeredgecolor='black', label='Ventas Mensuales')

            avg_sales = data.values.mean()
            ax.axhline(avg_sales, color=self.colors[2], linestyle='--', linewidth=2, label=f'Promedio: S/ {avg_sales:,.0f}')

            ax.set_title('TENDENCIA MENSUAL DE VENTAS SIN IGV', fontsize=18, fontweight='bold', pad=20)
            ax.set_xlabel('Mes', fontweight='bold')
            ax.set_ylabel('Ventas Sin IGV ($)', fontweight='bold')
            ax.grid(True, linestyle='--', alpha=0.5)

            plt.xticks(rotation=45, ha='right')

            for i, (months, value) in enumerate(zip(months, data.values)):
                ax.annotate(f'S/ {value:,.0f}', (months, value),
                            textcoords="offset points", xytext=(0,10),
                            ha='center', fontweight='bold')
            
            ax.legend(loc='best') 

            plt.tight_layout()
            self.save_graph('monthly_sales_trend.png')
        
        except Exception as e:
            logger.error(f"Error creando gráfico de tendencia mensual de ventas: {str(e)}")
            # dont do raise to avoid stopping the whole process    

    # create dashboard summary (Tarjeta de Resumen)

    def create_dashboard_summary(self):

        try:

            metrics = self.results['summary_metrics']

            fig, ax = plt.subplots(figsize=(10,6))
            ax.axis('off')

            ax.text(0.5, 0.9, 'RESUMEN DEL ANÁLISIS DE VENTAS', fontsize=24, fontweight='bold', ha='center', va='center',
                     transform=ax.transAxes, color=self.colors[0]) 

            ax.text(0.5, 0.8, 'Métricas Clave', fontsize=18, fontweight='bold', ha='center', va='center', transform=ax.transAxes)

            positions = [
                (0.25, 0.75), (0.75, 0.75),
                (0.25, 0.60), (0.75, 0.60),
                (0.25, 0.45), (0.75, 0.45),
                (0.25, 0.30), (0.75, 0.30)
            ]

            metrics_data = [
                ('Clientes Únicos', f"{metrics['unique_clients']}"),
                ('Total Ventas', f"S/ {metrics['total_sales_without_igv']:,.2f}"),
                ('Ventas sin IGV', f"S/ {metrics['total_sales_without_igv']:,.2f}"),
                ('Ventas con IGV', f"S/ {metrics['total_sales_with_igv']:,.2f}"),
                ('IGV Recaudado', f"S/ {metrics['total_igv_collected']:,.2f}"),
                ('Venta Promedio', f"S/ {metrics['average_sales_without_igv']:,.2f}"),
                ('Venta Maxima', f"S/ {metrics['max_sale_without_igv']:,.2f}"),
                ('Venta Minima', f"S/ {metrics['min_sale_without_igv']:,.2f}")
            ]

            for (x, y), (title, value) in zip(positions, metrics_data):

                rect = plt.Rectangle((x-0.15, y-0.08), 0.3, 0.12, 
                                     fill=True, color=self.colors[3], alpha=0.3, 
                                     transform=ax.transAxes)
                
                ax.add_patch(rect)

                ax.text(x, y+0.02, title, ha='center', va='center', 
                        fontsize=12, fontweight='bold', transform=ax.transAxes)
                
                ax.text(x, y-0.02, value, ha='center', va='center', 
                        fontsize=14, fontweight='bold', color=self.colors[3],
                        transform=ax.transAxes)
                
                ax.text(0.5, 0.15, f" Período Analizado: Últimos 12 meses", 
                   ha='center', va='center', fontsize=12, style='italic',
                   transform=ax.transAxes)
                
                ax.text(0.5, 0.10, f" Generado automáticamente por RPA Python", 
                   ha='center', va='center', fontsize=10, color='gray',
                   transform=ax.transAxes)
                
            plt.tight_layout()
            self.save_graph('dashboard_summary.png')

        except Exception as e:
            logger.error(f"Error creando resumen del dashboard: {str(e)}")
            raise

    # create all graphs

    def generate_all_graphs(self):

        try:
            logger.info("Iniciando generación de gráficos.")

            os.makedirs('outputs/graphs', exist_ok=True)

            self.create_sales_by_headquarter_graph()
            self.create_top_models_graph()
            self.create_sales_by_channel_graph()
            self.create_sales_by_segment_graph()
            self.create_monthly_sales_trend_graph()
            self.create_dashboard_summary()

            logger.info("Generación de gráficos finalizada.")

        except Exception as e:
            logger.error(f"Error generando todos los gráficos: {str(e)}")
            raise

# aux function for direct use

def generate_visualizations(results: Dict[str, Any]):

    visualizer = DataVisualizer(results)
    visualizer.generate_all_graphs()