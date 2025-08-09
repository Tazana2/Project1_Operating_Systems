# procplanner – Process Scheduling Simulation

Simulador educativo de cambio de contexto y planificación Round-Robin para la clase de Sistemas Operativos (Universidad EAFIT).

## Objetivos
- Representar el estado interno de un proceso (PC, registros, quantum, estado, instrucciones).
- Simular planificación Round-Robin con quantums configurables.
- Mostrar claramente los cambios de contexto y la evolución de registros.
- Detectar bucles con saltos repetidos excesivos (heurística anti-bucle infinito).
- Registrar la ejecución en un archivo de log (`logs.txt`).

## Estructura principal
```
include/        Headers (process, loader, planner, logger)
src/            Código fuente
processes.txt   Archivo de definición de procesos
<pid>.txt       Instrucciones por proceso (1.txt, 2.txt, ...)
build/makefile  Script de compilación
```

## Formato processes.txt
Cada línea define un proceso. Campos separados por coma, se permiten `:` o `=`.
Campos soportados: `PID`, `AX`, `BX`, `CX`, `Quantum`.
Ejemplo:
```
PID:1,AX=4,BX=2,Quantum=3
PID:2,Quantum=5
```

Para cada `PID n` se intenta cargar `n.txt` con instrucciones (máx. 20). Líneas vacías se ignoran.

## Instrucciones soportadas
```
ADD R1,R2|VAL    R1 = R1 + (R2 o inmediato)
SUB R1,R2|VAL    R1 = R1 - (R2 o inmediato)
MUL R1,R2|VAL    R1 = R1 * (R2 o inmediato)
INC R            R = R + 1
JMP idx          Salta a instrucción idx (0-based). Se controla salto repetido.
NOP              No hace nada
```
Errores de sintaxis se reportan en consola.

## Compilación
```sh
make -f build/makefile
```
Genera el ejecutable `procplanner`.

## Ejecución
```sh
./procplanner -f processes.txt
```
Esto leerá el archivo processes.txt cargando las instrucciones de los n procesos 1.txt, 2.txt, ... , n.txt. **Estos archivos deben estar en el mismo directorio donde se ejecuta procplanner.** 

Salida típica (fragmento):
```
[Context switch] -> Next PID 1
Saving state: PID=1 PC=0 AX=4 BX=2 CX=0
	[0] ADD AX,BX
	[1] SUB BX,1
	[2] INC AX
Updated state: PID=1 PC=3 AX=7 BX=1 CX=0
```

## Detección de bucles
Si un proceso ejecuta el mismo destino de `JMP` más de n veces consecutivas se considera bloqueo y se termina (`MAX_REPEATED_JUMPS`).

## Log
Se crea `logs.log` con eventos (inicio, carga de procesos, cambios de contexto, finalización de procesos, errores).

## Versión del compilador
Proyecto probado con `gcc (GCC) 13.x` (flag `-std=c11`).
