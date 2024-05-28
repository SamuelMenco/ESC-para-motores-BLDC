# Electronic Speed Controller para motores Brushless DC:
Sistema de control de velocidad electrónico para motores Brushless Dc con sensores Hall integrados, basado en Arduino y de código abierto. 
Para entender el funcionamiento de un motor BLDC, así como el de un ESC, se recomienda ver los siguientes videos: 

Electronoobs: https://www.youtube.com/watch?v=G9PHqN9vmVI&t=712s

Great Scott: https://www.youtube.com/watch?v=YV-ee8wA5lI&t=356s

MOSFETs y Bootstrap (Electronoobs): https://www.youtube.com/watch?v=xUwp3TOe4m0&t=637s

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







