# Ejercicio opcional 1

## Datos de latencia para mutex y semáforos:

En este ejercicio, analicé 55.315 muestras para mutex y 55.557 muestras para semáforos. Para el mutex, encontré que la latencia más baja fue de 46 nanosegundos, mientras que la más alta llegó a 25.516 nanosegundos. El promedio se situó en 375,83 nanosegundos y la desviación estándar fue de 386,98 nanosegundos. En el caso del semáforo, los resultados mostraron diferencias más marcadas: la latencia mínima fue de 77 nanosegundos, la máxima de 21.534 nanosegundos, el promedio de 427,23 nanosegundos y la desviación estándar de 439,79 nanosegundos.

## Análisis de resultados:

Al analizar estos datos, me di cuenta de que el mutex es significativamente más rápido que el semáforo, con una diferencia de aproximadamente 51 nanosegundos en el promedio. Esta diferencia es más notable que en mediciones anteriores, lo que confirma que el mutex tiene un mejor rendimiento. Creo que esta ventaja se debe a que, según lo que hemos estudiado en clase, pthread_mutex utiliza FUTEX, que está diseñado específicamente para reducir el número de llamadas al sistema y los cambios de contexto. Solo realiza una llamada al sistema cuando el recurso está bloqueado y es necesario esperar, lo que lo hace más eficiente.

Cuando miré el histograma, pude ver que la mayoría de las mediciones para ambos se concentran en valores bajos de latencia, entre 0 y 1000 nanosegundos. Noté que el semáforo tiene una distribución ligeramente más dispersa, lo que explica por qué su desviación estándar es mayor. Además, observé que el mutex mantiene una ventaja clara en el rango de latencias más frecuentes.

Cabe destacar que ambos sistemas siguen un patrón similar, ya que casi todas las operaciones son muy rápidas, pero hay unas pocas que toman mucho más tiempo. Esto concuerda con lo que hemos visto sobre cómo FUTEX utiliza wait queues para realizar espera pasiva en el kernel, optimizando así el uso de CPU.

En conclusión, después de analizar todos estos datos, creo que la ventaja del mutex en velocidad se debe principalmente al uso de FUTEX, que minimiza las operaciones costosas. Sin embargo, el semáforo ofrece una mayor consistencia.

## Histograma
<img width="1200" height="800" alt="Comparacion" src="https://github.com/user-attachments/assets/b070cef8-8d16-4ba7-8bbd-74a4b4429ae8" />


Repositorio de código:
https://github.com/Ruben249/sistemas_distribuidos_ejercicios_opcionales/tree/main/ejercicio_opcional_1