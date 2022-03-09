# Red de monitorización de CO<sub>2</sub>

Propuesta de proyecto Capstone 2021

Ideado por:

* Dr. Adán Geovanni Medrano Chávez
* Dra. Areli Rojo Hernández

## Resumen

Este documentos esboza las ideas generales que conllevarían al desarrollo de un sistema de monitorización de CO<sub>2</sub> basado en una red de sensores. Los sensores estarían conectados mediante radios WiFi a una internet privada. El objetivo de este proyecto es proveer a la industria y al sector público un sistema que permita medir la calidad del aire dentro de espacios cerrados. Las mediciones que el sistema arroje permitirían generar alertas que informen que es necesario ventilar un área. El desarrollo de este proyecto es necesario porque actualmente se requieren sistemas que coadyuven con las medidas de prevención de infecciones respiratorias. El sistema que tenemos en mente permitiría detectar indirectamente posibles riesgos de contagio de la COVID-19 o la influenza. Esto debido a que una concentración de 600 PPM de CO<sub>2</sub> indicarían que el área de monitorización no está suficientemente ventilada.

## Introducción

De acuerdo con estudios recientes (Wang, 2021), la COVID-19 es una enfermedad cuyo patógeno se transmite via aerea a través de los aerosoles que expulsamos al hablar, estornudar o toser.

Con el fin de reducir el número de contagios de esta enfermedad, se han ideado diversas medidas de prevención. En la Ciudad de México, por ejemplo, se fomenta el uso de cubrebocas y el control del aforo en lugares cerrados (Gobierno de la Ciudad de México, 2021). Actualmente, se sugiere que monitorizar la calidad del aire (Money, 2021) ayudaría en la reducción de contagios.

En el mercado existen diversos monitores de CO<sub>2</sub> que permiten obtener indirectamente una medición de la calidad del aire, siendo el valor de 600 PPM un indicador que sugiere la ventilación del área de observación del dispositivo. 

Los de monitores son apropiados para su uso en espacios pequeños, como los cuartos de una casa o las oficinas de una microempresa, porque los usuarios pueden leer las mediciones que el aparato muestra en su pantalla. No obstante, la monitorización de un área grande, como un campus universitario, un centro comercial o las oficinas de un corporativo, mediante monitores de CO<sub>2</sub> comerciales no sería suficiente debido a que se requiere ir al lugar de colocación del monitor para conocer se lectura.

Una red de sensores de CO<sub>2</sub> resulta más apropiada para monitorizar grandes espacios porque este sistema permitiría que sus usuarios obtengan información sobre la calidad del aire de manera remota mediante una aplicación web; así, los visitante podrían conocer la calidad del aire, previo a sus llegada. Además, los datos colectados por el sistema permitiría a los administradores del área tomar medidas más acertadas sobre el aforo, también podrían dirigir el flujo de personas a áreas ventiladas o impedir el paso en zonas donde la calidad del aire no es la apropiada. Inclusive, los datos capturados por la red de sensores podrían utilizarse para correlacionar los reportes de contagios con las áreas con ventilación inapropiada.

La situación de emergencia sanitaria que se experimenta actualmente requiere de sistemas que coadyuven con las medidas de prevención de infecciones respiratorias: la red de sensores que describiremos en esta propuesta de proyecto justo busca ser parte de los complementos que ayuden a mitigar los problemas relacionados con la pandemia.

## Problema

*A partir de una red de sensores de CO<sub>2</sub>, monitorizar remotamente la calidad del aire*.

## Objetivos

Habremos logrado solucionar el problema anterior una vez que el siguiente objetivo sea alcanzado.

### Objetivo general

Diseñar e implementar sensores que capture mediciones de CO<sub>2</sub>. Las mediciones serán enviadas a un bróker MQTT. Los sensores estarían energizados mediante la red eléctrica, asimismo, estos dispositivos estarán conectados a una internet privada via WiFi, al igual que el bróker. La realización del proyecto debe producir 2 sensores.

### Objetivos específicos

El objetivo general se alcanzará gradualmente, mediante el cumplimiento de los siguientes objetivos específicos.

1. Diseñar e implementar un sensor remoto de CO<sub>2</sub> configurable.
2. Configurar una internet privada según la arquitectura de red del objetivo cinco.
3. Conectar los sensores al bróker.
4. Conectar un cliente MQTT al bróker.
5. Verificar que el sistema de monitorización opera correctamente.

## Justificación

La realización de este proyecto requiere del uso de las tecnologías de la Internet de las Cosas porque su principal función es la monitorización de los niveles de CO<sub>2</sub> mediante un dispositivo configurable.

Los datos que el sistema capture serán procesados y presentados de tal manera que sea sencillo tomar decisiones sobre qué áreas ventilar y cuándo hacerlo.

El sistema de monitorización sería diseñado para el bróker, la aplicación cliente MQTT y los sensores remotos puedan ser actualizados. Asimismo, sería necesario dar mantenimiento preventivo y correctivo a los sensores remotos para asegurar que estos están operando correctamente. Estas dos necesidades permitirían seguir obteniendo capital después de que la venta de la red de sensores.

Finalmente, la realización de este proyecto contribuiría al alcance del objetivo tres de la agenda 2030 de los Objetivos de Desarrollo Sostenible de las Naciones Unidas (Naciones Unidas, 2021): «*Garantizar una vida sana y promover el bienestar para todos en todas las edades*». Particularmente, el aporte se haría en el objetivo 3.d «*Reforzar la capacidad de todos los países, en particular los países en desarrollo, en materia de alerta temprana, reducción de riesgos y gestión de los riesgos para la salud nacional y mundial*».

## Descripción del escenario de aplicación

![Edificio](./fig/building_wsn_CO2.svg "Red de sensores de CO2 desplegada en un edificio.")

## Materiales requeridos

### Software

1. Editor de código fuente VS Code.
2. Terminal de comandos.
3. Extensiones de VS Code para programar en C/C++, Arduino C, Python y Markdown.
4. Compiladores GCC o CLANG.
5. Interpreté Python 3.9.

### Hardware

1. Placas de desarrollo ESP32.
2. Sensores de CO<sub>2</sub> CM1107-N.
3. Monitor de CO<sub>2</sub> https://www.amazon.com.mx/interiores-Temperatura-exteriores-recargable-incorporada/dp/B098Q5XZTK/ref=sr_1_1
4. Elementos electrónicos (alambres, resistores, capacitores, ledes, tabletas de desarrollo, etc.).
5. Raspberry Pi.
6. PC.

## Referencias

1. Wang, C. C., Prather, K. A., Sznitman, J., Jimenez, J. L., Lakdawala, S. S., Tufekci, Z., & Marr, L. C. (2021). *Airborne transmission of respiratory viruses*. Science (New York, N.Y.), 373(6558), eabd9149. https://doi.org/10.1126/science.abd9149
2. Gobierno de la Ciudad de México. (2021, julio). *COVID-19 CDMX*. https://www.covid19.cdmx.gob.mx/
3. Money, C. (2021, septiembre). *The coronavirus is airborne. Here’s how to know if you’re breathing other people’s breath.*. The Washington Post. https://www.washingtonpost.com/health/2021/02/10/carbon-dioxide-device-coronavirus/
4. Naciones Unidas. (2021, julio). *Salud - Desarrollo Sostenible*. https://www.un.org/sustainabledevelopment/es/health/

