# Philosophers

<div align="center">

![42 School Badge](https://img.shields.io/badge/42-School-000000?style=flat&logo=42&logoColor=white)
![C](https://img.shields.io/badge/C-00599C?style=flat&logo=c&logoColor=white)
![Threading](https://img.shields.io/badge/Threading-Multithreading-red?style=flat)
![Status](https://img.shields.io/badge/Status-Completed-success?style=flat)

</div>

## 📖 Descrizione

Il progetto **Philosophers** è un problema di sincronizzazione classico che simula il famoso "Dining Philosophers Problem" proposto da Edsger Dijkstra. 

Il problema coinvolge un certo numero di filosofi seduti attorno a un tavolo rotondo. Tra ogni coppia di filosofi c'è una forchetta. Ogni filosofo alterna tra tre stati: **pensare**, **mangiare** e **dormire**. Per mangiare, un filosofo ha bisogno di due forchette (quella alla sua sinistra e quella alla sua destra).

### Obiettivi del Progetto

- Imparare i concetti di threading e sincronizzazione
- Gestire correttamente i mutex per evitare data races
- Prevenire deadlock tra i thread
- Implementare un sistema di monitoraggio efficiente

---

## 🚀 Features

✅ Gestione di N filosofi tramite threads  
✅ Sincronizzazione con mutex per le forchette  
✅ Monitoraggio dello stato di ogni filosofo  
✅ Rilevamento della morte di un filosofo  
✅ Gestione del numero minimo di pasti (opzionale)  
✅ Nessun data race o deadlock  
✅ Performance ottimizzate con minimal delay  

---

## 🛠️ Struttura del Progetto

```
philo_horror/
├── include/
│   └── philo.h          # Header file con strutture e prototipi
├── src/
│   ├── main.c           # Entry point e gestione thread
│   ├── init_philo.c     # Inizializzazione filosofi e dati
│   ├── routine.c        # Routine principale dei filosofi
│   ├── monitor.c        # Thread di monitoraggio morti
│   ├── print_state.c    # Stampa degli stati
│   └── utils.c          # Utility functions (atoi, get_time, etc.)
├── obj/                 # Object files (generati durante la compilazione)
├── Makefile             # Build automation
├── .gitignore          # Files da ignorare in git
└── README.md           # Questo file
```

---

## 📦 Compilazione ed Esecuzione

### Requisiti

- **GCC** o **Clang**
- **Make**
- Sistema operativo **Linux** o **macOS**

### Compilazione

```bash
make
```

Questo comando genera l'eseguibile `philo`.

### Esecuzione

```bash
./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [number_of_times_each_philosopher_must_eat]
```

**Parametri:**

1. `number_of_philosophers`: Il numero di filosofi (e di forchette)
2. `time_to_die`: Tempo (in ms) prima che un filosofo muoia senza mangiare
3. `time_to_eat`: Tempo (in ms) che impiega un filosofo a mangiare
4. `time_to_sleep`: Tempo (in ms) che un filosofo passa dormendo
5. `number_of_times_each_philosopher_must_eat` (opzionale): Numero minimo di volte che ogni filosofo deve mangiare

**Esempi:**

```bash
# Nessun filosofo dovrebbe morire
./philo 5 800 200 200

# I filosofi si fermano dopo aver mangiato 7 volte ciascuno
./philo 5 800 200 200 7

# Un filosofo dovrebbe morire
./philo 4 310 200 100
```

### Comandi Make

| Comando | Descrizione |
|---------|-------------|
| `make` | Compila il progetto |
| `make clean` | Rimuove i file oggetto |
| `make fclean` | Rimuove i file oggetto e l'eseguibile |
| `make re` | Ricompila tutto da zero |
| `make val` | Esegue Valgrind per controllare memory leak |
| `make hel` | Esegue Helgrind per rilevare race conditions |

---

## 🧵 Implementazione

### Strutture Dati Principali

#### t_data
Contiene i parametri globali della simulazione:
- Tempi di vita, mangiare e dormire
- Numero di filosofi
- Mutex per la scrittura e sincronizzazione
- Array di mutex per le forchette

#### t_philo
Rappresenta ogni singolo filosofo:
- ID univoco
- Contatore pasti consumati
- Timestamp dell'ultimo pasto
- Thread associato
- Mutex per proteggere i dati dei pasti
- Puntatori alle forchette (sinistra e destra)

### Thread di Monitoraggio

Un thread dedicato (`ft_death_star`) monitora continuamente tutti i filosofi per:
- Rilevare se un filosofo è morto (tempo dall'ultimo pasto > time_to_die)
- Verificare se tutti i filosofi hanno mangiato abbastanza (se specificato)

### Prevenzione Deadlock

Per evitare deadlock, i filosofi con ID pari e dispari prendono le forchette in ordine diverso:
- **Pari**: prima sinistra, poi destra
- **Dispari**: prima destra, poi sinistra

---

## 📝 Note Tecniche

- **Precisione timing**: Utilizzo di `gettimeofday()` per precisione al millisecondo
- **Data race free**: Ogni accesso a dati condivisi è protetto da mutex
- **No busy waiting**: Uso di `usleep()` per evitare consumo eccessivo di CPU
- **Thread-safe printing**: Tutti i messaggi sono sincronizzati tramite write_mutex

---

## 🐛 Testing

Per testare il programma con diversi scenari:

```bash
# Test base - nessuno muore
./philo 5 800 200 200

# Test con 1 filosofo - dovrebbe morire
./philo 1 800 200 200

# Test limite - filosofo muore appena
./philo 4 310 200 100

# Test con molti filosofi
./philo 200 800 200 200

# Test con numero di pasti
./philo 4 410 200 200 10
```

---

## 🎓 Concetti Appresi

- **Threading** con pthread library
- **Mutex** e sincronizzazione
- **Race conditions** e come evitarle
- **Deadlock** detection e prevenzione
- **Memory management** in programmi multithread
- **Time management** ad alta precisione

---

## 📄 Licenza

Questo progetto è parte del curriculum di **42 Ecole**.

---

## 👤 Autore

**gnicolo** - [42 Firenze]

---

## 🙏 Ringraziamenti

Grazie a 42 Ecole per questo progetto stimolante che insegna concetti fondamentali di programmazione concorrente!

