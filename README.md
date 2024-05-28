# Electronic Speed Controller para motores Brushless DC:
Sistema de control de velocidad electrónico para motores Brushless Dc con sensores Hall integrados, basado en Arduino y de código abierto. 
Para entender el funcionamiento de un motor BLDC, así como el de un ESC, se recomienda ver los siguientes videos: 

Electronoobs: https://www.youtube.com/watch?v=G9PHqN9vmVI&t=712s

Great Scott: https://www.youtube.com/watch?v=YV-ee8wA5lI&t=356s

MOSFETs y Bootstrap (Electronoobs): https://www.youtube.com/watch?v=xUwp3TOe4m0&t=637s

Drivers (Lista de reproducción con toda la información necesaria para entender el funcionamiento de estos componentes, creada por Foolish Engineer): https://www.youtube.com/watch?v=DXyTHhUjxjk&list=PLbGlpmZLQWJceSkQv96j-L4YyuxS-E0Wm

Se debe entender un motor BLDC como un conjunto se rotor y estator, en el que el estator constará de un conjunto de imanes fijos, y el rotor con tres bobinados, y la rotación del mismo dependerá de las bobinas por las que ingrese y salga la corriente del sistema, es decir, del orden en que se conecten las bobinas al voltaje positivo y a tierra, respectivamente. La secuencia que se debe seguir para activar y desactivar las bobinas, dependiendo del estado de los sensores Hall, se muestra a continuación: 

| ![Diagrama secuencia de encendido](https://github.com/SamuelMenco/ESC-para-motores-BLDC/assets/160543787/bce77477-9b67-424b-9b5e-ec7bd024a8d1) |
|:--:|

| ![Tabla secuencia de encendido](https://github.com/SamuelMenco/ESC-para-motores-BLDC/assets/160543787/820370f4-b20f-4601-9fcf-4ebc33d034e9) |
|:--:|

En caso de no lograr leer los sensores Hall con el Arduino, seguramente será necesario acondicionar las señales con una resistencia Pull-up, la cual se ubicará entre la salida del sensor y la lectura del ardunino, conectándola directamente a 5V. 

Es asimismo importante entender la combinación de señales necesaria para que los drivers seleccionados activen los MOSFETs correspondientes. 

# Especificaciones:

*Voltaje de operación de 24V

*Probado con corrientes de hasta 30A 

*Diseñado para motores con sensores Hall, se identifican de manera automatica en el código 

*Código abierto, con posibilidad de mejoras e implementaciones de controles más sofisticados, como lo puede ser un PI

*Drivers L6387E

*MOSFETs IRFS3107

*Bajo costo

*Basado en Arduino


# Explicación del código:

El código que se encuentra en la sección de archivos, llamado control PWM 3, cuenta con todos los elementos necesarios para poder realizar la conmutación y controlar la velocidad de un motor BLDC. 

El código se encuentra comentado, pero a continuación se explicarán las funciones principales: 

**ISR(ADC_vect)**: Función que se activa cada vez que se realiza una conversión del ADC, según lo establecido en el setup. Dada la frecuencia con la que se realizan estan interrupciones, se decide leer el estado de los sensores Hall y activar las fases correspondientes. Como es lógico, en esta función se realizan las conversiones del ADC al que se conecta el potenciometro de aceleración, en un rango de valores de 0 a 1023, se divide entre 4 para llegar a valores entre 0 y 255, que serán los valores de Duty Cycle del PDW a escribir. 

**getHalls()**: En esta función se leen los pines del puerto D de Arduino, especificamente 5, 6 y 7, dependiendo de la combinación de estos sensores se decide el step, que corresponde a la etapa del ciclo electrico en la que nos encontramos, de acuerdo a la imagen de arriba. 

**WritePhases()**: Función para activar los drivers conectados a cada mosfet de cada uno de los tres puentes trifásicos (ver videos para entender correctamente) 

**DecideState()**: Dependiendo del Step decidido en getHalls. se activarán las fases correspondientes. 

**ISR(PCINT2_vect)**: Este vector de interrupción se activará cada vez que un sensor Hall cambie de estado. En este caso no se está utilizando, pero se puede utilizar, por ejemplo, para imprimir el estado de los sensores y la fase actual

Una recomendación para evitar cortocircuitos, es buscar la forma de implementar un DEADTIME entre la activación de fases, de modo que no se activen dos MOSFETs al mismo tiempo. 


# Hardware:

![image](https://github.com/SamuelMenco/ESC-para-motores-BLDC/assets/160543787/41b9e3c2-db8e-41bf-88c3-974d853d1231)

La combinación de señales para el driver seleccionado se presenta a continuación: 

![image](https://github.com/SamuelMenco/ESC-para-motores-BLDC/assets/160543787/53a7581c-e362-4c49-a667-941e200db08c)

Esto debe corresponder con la combinación programada en el arduino, especificamente en la función **DecideState()** 

Las ventajas que ofrecen estos componentes se relacionan con su confiabilidad. Los Mosfets utilizados (IRFS3107) soporta una corriente pico de 900A y 250A en estado estable. Por su parte, toda la lógica de control se encuentra programada en el arduino, que se puede reemplazar por cualquier otro microcontrolador con mejores prestaciones. En su defecto, se puede reemplazar el micro con un ciruito integrado dedicado como el MC33035 


# Telemetría
Telemetría con Arduino y LoRa
Este proyecto permite medir y transmitir varios parámetros de telemetría como RPM del motor, RPM de la llanta, corriente, voltaje, y datos GPS utilizando un Arduino, sensores de corriente y voltaje, y un módulo de comunicación LoRa. Los datos se envían a través de LoRa a una estación base para su monitoreo y análisis. En la carpeta de “Esquemáticos” podrás encontrar las conexiones correspondientes.

Se implementaron una serie de sensores para poder registrar parámetros importantes a la hora de hacer seguimiento al vehículo eléctrico. Para esto, se utilizó un Arduino Nano con el cual se recopilaron las señales correspondientes.

**Componentes**:
-	**Arduino Nano/Uno (x2)**: Microcontrolador para poder recopilar todas las señales provenientes de los sensores, para posteriormente ser enviadas al segundo Arduino mediante LoRa. Se utilizarán los puertos digitales y analógicos, para los últimos, se provee un tutorial para la tranformación de la señal del ADC.
Video Tutorial: https://youtu.be/gAhQxGrUz8o?si=lVkVQKXAq5nB05s_ 

-	**Sensor de Corriente ACS712 30A**: Para la medición de la corriente consumida por el vehículo.
Link de Compra Amazon: https://www.amazon.com/Current-Voltage-DC0-25V-Terminal-Arduino/dp/B096XW328X Nota: Elegir el de amperaje adecuado para su aplicación.
Video Tutorial: https://www.youtube.com/watch?v=hArTsILiBR8&ab_channel=MarioAlomoto

-	**Divisor de Voltaje:** Para la medición del voltaje de la batería/fuente de poder. Este, debe estar calculado de tal manera en la que el voltaje que llegue a la lectura analógica del Arduino sea menor a 5V, pero no tan pequeña como para que se pierda precisión en la medida.
Video Tutorial: https://youtu.be/DVWSOGCu_iA?si=zih63USFHZE2d8jD 

-	**LoRa Ra-01 SX1278 433 MHz (x2):** Modulo transceptor que permitirá transmitir y recibir las señales de radio mediante las cuales, se transmitirán datos en formato csv. Este, irá conectado al Arduino, y se necesitan 2, para poder enviar y recibir datos. Las frecuencias de funcionamiento y el módulo podrían variar de acuerdo con el país y las frecuencias libres.
Video Tutorial: https://youtu.be/uR0NHOCthcs?si=PzJw8fnk-Q3JDHxH 
Link de Compra Amazon: https://www.amazon.com/-/es/Comimark-SX1278-alcance-inal%C3%A1mbrico-Arduino/dp/B07W6ZPH7D

-	**GPS Neo6m:** Modulo GPS para poder obtener las coordenadas del dispositivo. Además, puede proveer distintos datos interesantes como la velocidad instantánea, cantidad de satélites conectados, entre otros.
Video Tutorial: https://youtu.be/0rnZKiEsZnk?si=7-4brc8d4JG8HcgL 

**Descripción del Código:**
-	Código de Transmisión de Telemetría con LoRa: Mediante el cual, se enviarán los datos en formato csv, para ser recibidos, guardados y procesados.

**Bibliotecas Utilizadas**
1.	<util/atomic.h>: Proporciona macros para operaciones atómicas que permiten desactivar y restaurar interrupciones.
2.	<SPI.h>: Biblioteca para comunicación SPI.
3.	<LoRa.h>: Biblioteca para comunicación LoRa.
4.	<SoftwareSerial.h>: Permite la comunicación serial en otros pines además de los pines seriales por defecto.
5.	<TinyGPS.h>: Biblioteca para manejar datos GPS.

Variables Globales
•	Variables Volátiles:
  •	mseg, t_stamp, pulsos, t_0, periodo, RPM_motor: Para medir las RPM del motor.
  •	delta_t_llanta, t0_llanta, rpm_llanta, pulsos_llanta: Para medir las RPM de la llanta.
  
•	Flags:newdata_motor, newdata_llanta: Indican si hay nuevos datos de RPM disponibles.
  
•	GPS:

TinyGPS gps: Objeto para manejar datos GPS.
  
SoftwareSerial ss: Comunicación serial con el módulo GPS.

latitude, longitude, speed_kmh: Datos del GPS.
  
•	Sensores:

I, V, voltajeSensor, Sensibilidad: Variables para medir corriente y voltaje.

•	LoRa:

BAND: Frecuencia de operación de LoRa.

message: Mensaje a enviar.
  
**Configuración Inicial (setup)**
1.	Serial: Inicializa la comunicación serial a 9600 bps para depuración.
2.	GPS: Inicializa la comunicación serial con el módulo GPS.
3.	Pines: Configura los pines para los sensores Hall y los sensores de corriente y voltaje.
4.	Interrupciones:
Timer1: Configura una interrupción que se activa cada milisegundo para contar el tiempo.

Sensor Hall Motor: Configura una interrupción en el pin 2 para contar los pulsos del sensor Hall del motor.

Sensor Hall Llanta: Configura una interrupción en el pin 3 para contar los pulsos del sensor Hall de la llanta.

6.	LoRa: Inicializa el módulo LoRa y verifica su correcto funcionamiento.
   
**Bucle Principal (loop)**
1.	Medición de Corriente y Voltaje:
Llama a las funciones get_corriente() y get_voltaje() para obtener las lecturas promedio de corriente y voltaje.

2.	Datos GPS:
Actualiza los datos del GPS si hay datos disponibles.

3.	Transmisión de Datos:
Si hay nuevos datos de RPM del motor (newdata_motor), crea un mensaje con todos los datos y lo envía a través de LoRa.

Si no hay nuevos datos, envía un mensaje con las RPM del motor y la llanta como 0.

**Interrupciones**
  •	ISR(TIMER1_COMPA_vect): Incrementa el contador mseg cada milisegundo.
  •	interrupcion0(): Maneja la interrupción del sensor Hall del motor para calcular las RPM del motor.
  •	interrupcion1(): Maneja la interrupción del sensor Hall de la llanta para calcular las RPM de la llanta.
  
**Funciones de Medición**
  •	get_corriente(n_muestras): Mide la corriente promedio tomando n_muestras del sensor de corriente.
  •	get_voltaje(n_muestras): Mide el voltaje promedio tomando n_muestras del divisor de voltaje.

-	**Codigo del Receptor de Telemetría con LoRa:** Este permite recibir datos de telemetría enviados a través de LoRa utilizando el Arduino de Transmisión. Los datos recibidos pueden incluir información sobre la velocidad del motor, corriente, voltaje y datos GPS, y se muestran en el monitor serial para su monitoreo y análisis.
  
**Bibliotecas Utilizadas**
1.	<SPI.h>: Biblioteca para comunicación SPI.
2.	<LoRa.h>: Biblioteca para comunicación LoRa.
   
**Variables Globales**
  •	LED: Pin del LED que se puede utilizar para indicar la recepción de un mensaje.
  •	inString: Cadena temporal para almacenar los caracteres entrantes.
  •	message: Mensaje completo recibido.
  •	val, rssi: Valores auxiliares para el procesamiento de datos y la intensidad de la señal recibida.
  
**Configuración Inicial (setup)**
1.	Serial: Inicializa la comunicación serial a 9600 bps para depuración.
2.	LED: Configura el pin del LED como salida.
3.	LoRa: Inicializa el módulo LoRa y verifica su correcto funcionamiento a una frecuencia de 433 MHz (o 915 MHz, según la región).
   
**Bucle Principal (loop)**
**1.	Recepción de Paquetes:**
  •	Verifica si hay un paquete disponible utilizando LoRa.parsePacket().
  •	Si se recibe un paquete, lee los caracteres del paquete mientras estén disponibles utilizando LoRa.available() y los almacena en inString, omitiendo los espacios.
  
**2.	Procesamiento del Mensaje:**
  •	Asigna la cadena recibida a message y limpia inString para la siguiente recepción.
  •	Obtiene la intensidad de la señal recibida (RSSI) utilizando LoRa.packetRssi().  
  
**3.	Salida Serial:**
  •	Imprime el mensaje recibido en el monitor serial para su visualización.
  
**Ejemplo de Flujo**
**1.	Inicialización:**
  •	El Arduino inicializa la comunicación serial y el módulo LoRa.
  •	Verifica si el módulo LoRa se ha inicializado correctamente.
  
**2.	Recepción de Datos:**
  •	Espera paquetes de datos entrantes.
  •	Cuando se recibe un paquete, procesa los caracteres y los almacena en una cadena.
  
**3.	Visualización de Datos:**
  •	Muestra el mensaje recibido y el valor de RSSI en el monitor serial.


**Visualización en Tiempo Real:** Para la visualización en tiempo real, se utilizó un software de código abierto realizado por farrefel, compartido en GitHub. Este, recibe los datos en formato csv, por lo cual no se debe hacer ninguna modificación al código anterior. Posteriormente, se organizan las estructuras de los datos, y permite verlos y guardarlos en tiempo real, para su posterior exportación.
Video Tutorial: https://youtu.be/C1m6CvgjcSc?si=9Fx-rMbT3mTT6_ZV 

Repositorio de GitHub TelemetryViewer: https://github.com/farrellf/TelemetryViewer/tree/master/Telemetry%20Viewer 









