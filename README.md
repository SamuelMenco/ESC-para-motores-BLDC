# Electronic Speed Controller para motores Brushless DC
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




