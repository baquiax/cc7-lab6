# Xell v1.0

A continuacion se describen las carácterísticas principales que este Shell posee:

  - Ejecución de procesos en background
  - History Stack 
  - SIGINT Handler

Esta es una aplicación que muestra el uso de subprocesos en Linux Shells.

### Version
1.0

### Modo de uso

Primero debe compilar los archivos necesarios. Puede notar que se usa un segundo script `background.c` que puede usar para los procesos en segundo plano.

```sh
$ gcc shell.cpp -o shell && gcc background.c -o background
```

```sh
$ ./shell
baquiax@baquiax$: 
ls
baquiax@baquiax$: 
./background &

```

License
----

MIT
