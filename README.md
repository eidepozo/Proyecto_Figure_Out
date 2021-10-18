# Proyecto_Figure_Out

Etapa 2
Ver. 1.2.3

Autores: Rodrigo Miranda, Adolfo Cañoles, Elliot Ide
Ultima actualizacion: 14/12/2016 01:06:21

Librerias:
- libglew-dev
- libglfw3-dev
- libassimp-dev
- 
/* -libasound-dev
   -libopenal-dev 
   -libalut-dev*/

Keys:
+---------------------------------+	     
|     Accion     |     Tecla      |
+----------------+----------------+
|    <Saltar>    |   Backspace    |
|    <Inicio>    |     Enter      |
|    <Pausa>     |       P        |  
|    < <-- >     |       A	  |	
|    < --> >     |       D        |  
|   <updt-cam>   |       F        |
+----------------+----------------+ 

Modalidad de juego: Para iniciar aprete enter, y muevase hacia la derecha e izquierda con A y D respectivamente intentando evadir los obstaculos (cubos) y conseguir la mayor cantidad de monedas y vidas.

Camara:
Existen 2 "tipos" de camara en el juego:
  -Camara 1: La camara que se utiliza en conjunto con la interaccion de teclado (source 11-texture-uv-mesh).
  -Camara 2: La camara para desplazarse en el entorno, que utiliza parametros sen y cos (source 11-texture-uv-mesh).
  	     Se controla con i,j,k,l (arriba, izda, dere, abajo) y el resto de las teclas siguen de la misma forma.

**
Mallas: Todas las mallas excepto la del corazon son originales.

Texturas: Existen 6 archivos .jpg y png, para el personaje, los obstaculos, el piso, el mundo, las monedas y los corazones.

Mejoras:
-Incorporacion de texturas en todas las mallas.
-Se eliminaron las clases corazon, moneda, piso y obstaculo.
-Renovacion de la clase malla.
-Ahora se estan utilizando las funciones que existian en personaje, y se creo ademas una nueva funcion para mostrar el estado
actual del personaje.
-Modularizacion de la creacion de objetos y rendering.
-Cada objeto tiene su propio arreglo de matrices que son utilizadas en el rendering =>(mejora en el rendimiento, se pueden "cargar" mas mallas).
-Incorporacion de colisiones (para un solo obstaculo momentaneamente, hay mas detalles en tools.cpp)

**
-Nueva disposicion de los obstaculos y las monedas.
-Colisiones para obstaculos, monedas y corazones.
-Especificaciones a nivel general de las funciones y caracteristicas del juego en main.cpp y tools.cpp.

ref: http://jsfiddle.net/LyM87 //source del salto
http://tf3dm.com/3d-model/valentine39s-day-heart-69073.html //source de la malla del corazon
