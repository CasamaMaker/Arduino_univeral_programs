# import sys
# import serial.tools.list_ports
# import serial
# import numpy as np
# import matplotlib
# matplotlib.use('QtAgg')  # Usa el backend de Qt para Matplotlib
# import matplotlib.pyplot as plt
# from matplotlib.backends.backend_qtagg import FigureCanvasQTAgg as FigureCanvas
# from matplotlib.figure import Figure

# from PySide6.QtWidgets import (
#     QApplication, QMainWindow, QVBoxLayout, QHBoxLayout, 
#     QWidget, QComboBox, QLabel, QPushButton, QSpinBox
# )
# from PySide6.QtCore import QTimer, Signal, QObject

# class SerialDataThread(QObject):
#     """Gestiona la lectura de datos del puerto serie en un hilo seguro"""
#     data_received = Signal(list)
#     error_occurred = Signal(str)

#     def __init__(self, port='/dev/ttyUSB0', baudrate=9600):
#         super().__init__()
#         self.port = port
#         self.baudrate = baudrate
#         self.ser = None
#         self.running = False

#     def start_reading(self):
#         try:
#             self.ser = serial.Serial(self.port, self.baudrate)
#             self.running = True
#         except Exception as e:
#             self.error_occurred.emit(f"Error al abrir puerto: {e}")

#     def read_data(self):
#         if not self.running or not self.ser:
#             return

#         try:
#             if self.ser.in_waiting > 0:
#                 line = self.ser.readline().decode('utf-8').strip()
#                 values = [float(val) for val in line.split()]
#                 self.data_received.emit(values)
#         except Exception as e:
#             self.error_occurred.emit(f"Error leyendo datos: {e}")

#     def stop_reading(self):
#         if self.ser:
#             self.ser.close()
#         self.running = False

# class PercentageSpinBox(QSpinBox):
#     """Custom QSpinBox that increments/decrements by 10%"""
#     def __init__(self, *args, **kwargs):
#         super().__init__(*args, **kwargs)
#         self.setMinimum(10)
#         self.setMaximum(10000)
#         self.setValue(100)

#     def stepBy(self, steps):
#         """Override step method to increment/decrement by 10%"""
#         current_value = self.value()
#         # Calculate 10% of current value, round to nearest int
#         # step_size = max(1, int(current_value * 0.1))
#         step_size = max(1, int(current_value * 0.1 / pow(10, len(str(int(current_value * 0.1))) - 1)) * pow(10, len(str(int(current_value * 0.1))) - 1))
#         new_value = current_value + steps * step_size
        
#         # Ensure new value is within min and max
#         new_value = max(self.minimum(), min(new_value, self.maximum()))
#         self.setValue(new_value)

# class ArduinoSerialPlotterGUI(QMainWindow):
#     def __init__(self):
#         super().__init__()
#         self.setWindowTitle("Arduino Serial Plotter")
#         self.setGeometry(100, 100, 1000, 600)

#         # Layout principal
#         main_layout = QVBoxLayout()
        
#         # Layout superior para controles
#         control_layout = QHBoxLayout()
        
#         # Puerto COM
#         self.port_combo = QComboBox()
#         self.refresh_ports()
#         control_layout.addWidget(QLabel("Port:"))
#         control_layout.addWidget(self.port_combo)

        
        
#         # Baudrate
#         self.baudrate_combo = QComboBox()
#         baudrates = [9600, 115200, 57600, 38400, 19200]
#         self.baudrate_combo.addItems(map(str, baudrates))
#         self.baudrate_combo.setCurrentText('115200')
#         control_layout.addWidget(QLabel("Baudrate:"))
#         control_layout.addWidget(self.baudrate_combo)
        
#         # # Max Samples
#         # self.max_samples_spin = QSpinBox()
#         # self.max_samples_spin.setRange(10, 10000)
#         # self.max_samples_spin.setValue(100)
#         # control_layout.addWidget(QLabel("Max Samples:"))
#         # control_layout.addWidget(self.max_samples_spin)

#         # Max Samples (Custom Spin Box)
#         self.max_samples_spin = PercentageSpinBox()
#         control_layout.addWidget(QLabel("Max Samples:"))
#         control_layout.addWidget(self.max_samples_spin)
        
        
        
#         # Botón Iniciar/Detener
#         self.start_stop_btn = QPushButton("Start")
#         self.start_stop_btn.clicked.connect(self.toggle_plotting)
#         control_layout.addWidget(self.start_stop_btn)

#         main_layout.addLayout(control_layout)

#         # Configuración de la gráfica
#         self.figure = Figure(figsize=(10, 6))
#         self.canvas = FigureCanvas(self.figure)
#         self.ax = self.figure.add_subplot(111)
#         main_layout.addWidget(self.canvas)

#         # Widget contenedor
#         container = QWidget()
#         container.setLayout(main_layout)
#         self.setCentralWidget(container)

#         # Variables de estado
#         self.is_plotting = False
#         self.lines = []
#         self.y_data = []
#         self.x_data = None
#         self.num_variables = 0

#         # Inicialización del hilo de datos serie
#         self.serial_thread = SerialDataThread()
#         self.serial_thread.data_received.connect(self.update_plot)
#         self.serial_thread.error_occurred.connect(self.handle_error)

#         # Timer para lectura de datos
#         self.data_timer = QTimer()
#         self.data_timer.timeout.connect(self.serial_thread.read_data)
#         self.data_timer.setInterval(50)  # 50ms entre lecturas

#     def refresh_ports(self):
#         """Actualiza la lista de puertos disponibles"""
#         self.port_combo.clear()
#         ports = [port.device for port in serial.tools.list_ports.comports()]
#         self.port_combo.addItems(ports)

#     def toggle_plotting(self):
#         """Inicia o detiene el ploteo"""
#         if not self.is_plotting:
#             try:
#                 # Configuración inicial
#                 self.serial_thread.port = self.port_combo.currentText()
#                 self.serial_thread.baudrate = int(self.baudrate_combo.currentText())
                
#                 # Resetea la gráfica
#                 self.ax.clear()
#                 self.lines = []
#                 self.y_data = []
                
#                 # Configura eje X
#                 max_samples = self.max_samples_spin.value()
#                 self.x_data = np.arange(max_samples)
                
#                 # Inicia lectura
#                 self.serial_thread.start_reading()
#                 self.data_timer.start()
                
#                 # Cambia estado
#                 self.start_stop_btn.setText("Stop")
#                 self.is_plotting = True
            
#             except Exception as e:
#                 self.handle_error(str(e))
        
#         else:
#             # Detiene ploteo
#             self.serial_thread.stop_reading()
#             self.data_timer.stop()
#             self.start_stop_btn.setText("Start")
#             self.is_plotting = False

#     def update_plot(self, values):
#         """Actualiza la gráfica con nuevos datos"""
#         # Inicializa líneas si es necesario
#         if not self.lines and values:
#             self.num_variables = len(values)
#             for i in range(self.num_variables):
#                 line, = self.ax.plot([], [], linewidth=2, label=f'Variable {i+1}')
#                 self.lines.append(line)
            
#             self.ax.set_xlim(0, self.max_samples_spin.value())
#             self.ax.set_xlabel('Sample')
#             self.ax.set_ylabel('Values')
#             self.ax.grid(True)
#             self.ax.legend()
        
#         # Añade datos
#         self.y_data.append(values)
#         if len(self.y_data) > self.max_samples_spin.value():
#             self.y_data.pop(0)
        
#         # Actualiza líneas
#         for i, line in enumerate(self.lines):
#             column_data = [row[i] for row in self.y_data]
#             line.set_data(self.x_data[:len(column_data)], column_data)
        
#         # Ajusta ejes
#         self.ax.relim()
#         self.ax.autoscale_view()
        
#         # Redibuja
#         self.canvas.draw()

#     def handle_error(self, error_msg):
#         """Maneja errores de conexión o lectura"""
#         print(f"Error: {error_msg}")
#         self.start_stop_btn.setText("Start")
#         self.is_plotting = False
#         self.serial_thread.stop_reading()
#         self.data_timer.stop()

# def main():
#     app = QApplication(sys.argv)
#     plotter = ArduinoSerialPlotterGUI()
#     plotter.show()
#     sys.exit(app.exec())

# if __name__ == '__main__':
#     main()



import sys
import serial.tools.list_ports
import serial
import numpy as np
import matplotlib
matplotlib.use('QtAgg')  # Usa el backend de Qt para Matplotlib
import matplotlib.pyplot as plt
from matplotlib.backends.backend_qtagg import FigureCanvasQTAgg as FigureCanvas
from matplotlib.figure import Figure

from PySide6.QtWidgets import (
    QApplication, QMainWindow, QVBoxLayout, QHBoxLayout, 
    QWidget, QComboBox, QLabel, QPushButton, QSpinBox, QTextEdit
)
from PySide6.QtCore import QTimer, Signal, QObject, Qt

class SerialDataThread(QObject):
    """Gestiona la lectura de datos del puerto serie en un hilo seguro"""
    data_received = Signal(list)
    error_occurred = Signal(str)

    def __init__(self, port='/dev/ttyUSB0', baudrate=9600):
        super().__init__()
        self.port = port
        self.baudrate = baudrate
        self.ser = None
        self.running = False

    def start_reading(self):
        try:
            self.ser = serial.Serial(self.port, self.baudrate)
            self.running = True
        except Exception as e:
            self.error_occurred.emit(f"Error al abrir puerto: {e}")

    def read_data(self):
        if not self.running or not self.ser:
            return

        try:
            if self.ser.in_waiting > 0:
                line = self.ser.readline().decode('utf-8').strip()
                values = [float(val) for val in line.split()]
                self.data_received.emit(values)
        except Exception as e:
            self.error_occurred.emit(f"Error leyendo datos: {e}")

    def stop_reading(self):
        if self.ser:
            self.ser.close()
        self.running = False

class PercentageSpinBox(QSpinBox):
    """Custom QSpinBox that increments/decrements by 10%"""
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        self.setMinimum(10)
        self.setMaximum(10000)
        self.setValue(100)

    def stepBy(self, steps):
        """Override step method to increment/decrement by 10%"""
        current_value = self.value()
        # Calcula el tamaño del paso redondeando al primer dígito significativo
        step_size = max(1, int(current_value * 0.1 / pow(10, len(str(int(current_value * 0.1))) - 1)) * pow(10, len(str(int(current_value * 0.1))) - 1))
        new_value = current_value + steps * step_size
        
        # Ensure new value is within min and max
        new_value = max(self.minimum(), min(new_value, self.maximum()))
        self.setValue(new_value)

class ArduinoSerialPlotterGUI(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Arduino Serial Plotter")
        self.setGeometry(100, 100, 1200, 600)  # Aumentado el ancho para estadísticas

        # Layout principal
        main_layout = QHBoxLayout()  # Cambiado a horizontal para gráfica y estadísticas
        
        # Layout para controles y gráfica
        left_layout = QVBoxLayout()
        
        # Layout superior para controles
        control_layout = QHBoxLayout()
        
        # Puerto COM
        self.port_combo = QComboBox()
        self.refresh_ports()
        control_layout.addWidget(QLabel("Puerto:"))
        control_layout.addWidget(self.port_combo)
        
        # Baudrate
        self.baudrate_combo = QComboBox()
        baudrates = [9600, 115200, 57600, 38400, 19200]
        self.baudrate_combo.addItems(map(str, baudrates))
        self.baudrate_combo.setCurrentText('115200')
        control_layout.addWidget(QLabel("Baudrate:"))
        control_layout.addWidget(self.baudrate_combo)
        
        # Max Samples (Custom Spin Box)
        self.max_samples_spin = PercentageSpinBox()
        control_layout.addWidget(QLabel("Max Samples:"))
        control_layout.addWidget(self.max_samples_spin)
        
        # Botón Iniciar/Detener
        self.start_stop_btn = QPushButton("Start")
        self.start_stop_btn.clicked.connect(self.toggle_plotting)
        control_layout.addWidget(self.start_stop_btn)

        left_layout.addLayout(control_layout)

        # Configuración de la gráfica
        self.figure = Figure(figsize=(10, 6))
        self.canvas = FigureCanvas(self.figure)
        self.ax = self.figure.add_subplot(111)
        left_layout.addWidget(self.canvas)

        # Estadísticas
        self.stats_text = QTextEdit()
        self.stats_text.setReadOnly(True)
        self.stats_text.setMaximumWidth(300)  # Limitar ancho de las estadísticas

        # Añadir layouts al layout principal
        main_layout.addLayout(left_layout)
        main_layout.addWidget(self.stats_text)

        # Widget contenedor
        container = QWidget()
        container.setLayout(main_layout)
        self.setCentralWidget(container)

        # Variables de estado
        self.is_plotting = False
        self.lines = []
        self.y_data = []
        self.x_data = None
        self.num_variables = 0

        # Inicialización del hilo de datos serie
        self.serial_thread = SerialDataThread()
        self.serial_thread.data_received.connect(self.update_plot)
        self.serial_thread.error_occurred.connect(self.handle_error)

        # Timer para lectura de datos
        self.data_timer = QTimer()
        self.data_timer.timeout.connect(self.serial_thread.read_data)
        self.data_timer.setInterval(50)  # 50ms entre lecturas

    def refresh_ports(self):
        """Actualiza la lista de puertos disponibles"""
        self.port_combo.clear()
        ports = [port.device for port in serial.tools.list_ports.comports()]
        self.port_combo.addItems(ports)

    def toggle_plotting(self):
        """Inicia o detiene el ploteo"""
        if not self.is_plotting:
            try:
                # Configuración inicial
                self.serial_thread.port = self.port_combo.currentText()
                self.serial_thread.baudrate = int(self.baudrate_combo.currentText())
                
                # Resetea la gráfica
                self.ax.clear()
                self.lines = []
                self.y_data = []
                
                # Configura eje X
                max_samples = self.max_samples_spin.value()
                self.x_data = np.arange(max_samples)
                
                # Inicia lectura
                self.serial_thread.start_reading()
                self.data_timer.start()
                
                # Cambia estado
                self.start_stop_btn.setText("Stop")
                self.is_plotting = True
            
            except Exception as e:
                self.handle_error(str(e))
        
        else:
            # Detiene ploteo
            self.serial_thread.stop_reading()
            self.data_timer.stop()
            self.start_stop_btn.setText("Start")
            self.is_plotting = False

    def update_plot(self, values):
        """Actualiza la gráfica con nuevos datos"""
        # Inicializa líneas si es necesario
        if not self.lines and values:
            self.num_variables = len(values)
            for i in range(self.num_variables):
                line, = self.ax.plot([], [], linewidth=2, label=f'Variable {i+1}')
                self.lines.append(line)
            
            self.ax.set_xlim(0, self.max_samples_spin.value())
            self.ax.set_xlabel('Muestras')
            self.ax.set_ylabel('Valores')
            self.ax.grid(True)
            self.ax.legend()
        
        # Añade datos
        self.y_data.append(values)
        if len(self.y_data) > self.max_samples_spin.value():
            self.y_data.pop(0)
        
        # Actualiza líneas
        for i, line in enumerate(self.lines):
            column_data = [row[i] for row in self.y_data]
            line.set_data(self.x_data[:len(column_data)], column_data)
        
        # Ajusta ejes
        self.ax.relim()
        self.ax.autoscale_view()
        
        # Redibuja
        self.canvas.draw()

        # Actualiza estadísticas
        self.update_statistics(values)

    def update_statistics(self, current_values):
        """Actualiza el texto de estadísticas"""
        stats_text = ""
        for i, line in enumerate(self.lines):
            # Obtener datos de esta variable
            column_data = [row[i] for row in self.y_data]
            
            # Calcular estadísticas
            min_val = min(column_data) if column_data else current_values[i]
            max_val = max(column_data) if column_data else current_values[i]
            mean_val = np.mean(column_data) if column_data else current_values[i]
            current_val = current_values[i]

            # Formatear texto de estadísticas
            stats_text += f"Variable {i+1}:\n"
            stats_text += f"  Actual: {current_val:.2f}\n"
            stats_text += f"  Min:    {min_val:.2f}\n"
            stats_text += f"  Max:    {max_val:.2f}\n"
            stats_text += f"  Mean:  {mean_val:.2f}\n\n"

        # Actualizar widget de texto
        self.stats_text.setText(stats_text)

    def handle_error(self, error_msg):
        """Maneja errores de conexión o lectura"""
        print(f"Error: {error_msg}")
        self.start_stop_btn.setText("Start")
        self.is_plotting = False
        self.serial_thread.stop_reading()
        self.data_timer.stop()

def main():
    app = QApplication(sys.argv)
    plotter = ArduinoSerialPlotterGUI()
    plotter.show()
    sys.exit(app.exec())

if __name__ == '__main__':
    main()