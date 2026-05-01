*This project has been created as part of the 42 curriculum by dlaktaf.*

## Description

Codexion is a concurrency simulation written in C, inspired by the classic Dining
Philosophers problem. It models a group of coders sitting in a circular co-working hub,
competing for limited USB dongles needed to compile quantum code. Each coder is
represented by a POSIX thread and follows a repeating lifecycle: compile, debug, refactor.
Compiling requires two dongles simultaneously (left and right). The simulation ends when
all coders have compiled the required number of times, or when a coder burns out from
waiting too long without compiling.

The program implements two scheduling policies for fair dongle arbitration:
- **FIFO** (First In, First Out): dongles are granted in request arrival order.
- **EDF** (Earliest Deadline First): dongles are granted to the coder closest to burnout.

Both schedulers are backed by a custom min-heap priority queue.

## Instructions

### Compilation

```bash
make        # Build the codexion binary
make clean  # Remove object files
make fclean # Remove object files and binary
make re     # Full rebuild
```

The project compiles with `cc -Wall -Wextra -Werror -pthread`.

### Execution

```bash
./codexion <number_of_coders> <time_to_burnout> <time_to_compile> \
           <time_to_debug> <time_to_refactor> <number_of_compiles_required> \
           <dongle_cooldown> <scheduler>
```

**Arguments (all mandatory, all in milliseconds except counts and scheduler):**

| Argument | Description |
|---|---|
| `number_of_coders` | Number of coders (and dongles) |
| `time_to_burnout` | Max ms since last compile start before burnout |
| `time_to_compile` | Duration of compilation (holding 2 dongles) |
| `time_to_debug` | Duration of debugging phase |
| `time_to_refactor` | Duration of refactoring phase |
| `number_of_compiles_required` | Simulation ends when all reach this count |
| `dongle_cooldown` | Ms a dongle stays unavailable after release |
| `scheduler` | `fifo` or `edf` |

### Example

```bash
./codexion 5 800 200 100 100 3 50 fifo
```

## Blocking cases handled

### Deadlock prevention

Deadlock requires four Coffman conditions: mutual exclusion, hold-and-wait, no
preemption, and circular wait. This implementation prevents deadlock through two
mechanisms:

1. **No hold-and-wait**: Coders acquire both dongles atomically using `try_take_both()`.
   A coder either obtains both dongles simultaneously or holds none. This eliminates
   the scenario where a coder holds one dongle while waiting for another, which is the
   primary deadlock vector in dining philosopher problems.

2. **Ordered locking**: When locking two dongle mutexes, the lower-numbered dongle is
   always locked first. This prevents circular-wait conditions between mutex
   acquisitions.

### Starvation prevention

The priority queue (min-heap) per dongle ensures fair scheduling. The `should_yield()`
function checks whether a higher-priority coder could compile: if so, the lower-priority
coder backs off. A coder only yields to another coder who can actually acquire both
their dongles, preventing priority inversion where a high-priority coder blocks everyone
while unable to compile itself.

With FIFO, coders are served by request arrival order. With EDF, coders closest to
burnout get priority, preventing starvation under feasible parameters.

### Cooldown handling

Each dongle tracks its `release_time`. After release, the dongle remains unavailable for
`dongle_cooldown` milliseconds. The `dongle_available()` function checks both `in_use`
status and cooldown elapsed time before allowing acquisition. Coders poll with
`usleep(500)` between attempts, providing sub-millisecond response to cooldown
expiration.

### Precise burnout detection

A dedicated monitor thread polls every 1ms (`usleep(1000)`), checking each coder's
`last_compile_start` against `time_to_burnout`. This guarantees burnout detection
within 10ms of the actual burnout time. Coders who have already completed their
required compiles are excluded from burnout checks.

### Log serialization

All output is protected by a single `log_mutex`. The `print_status()` function locks the
mutex, checks the stop flag, prints the message, and unlocks. This guarantees no
interleaved log lines. The burnout message uses a separate `print_burnout()` function
that bypasses the stop-flag check to ensure the burnout event is always logged.

## Thread synchronization mechanisms

### pthread_mutex_t

- **Per-dongle mutex** (`t_dongle.mutex`): Protects each dongle's state (`in_use`,
  `release_time`, `wait_queue`). When acquiring both dongles, mutexes are locked in
  ascending dongle-ID order to prevent deadlock.

- **Log mutex** (`t_sim.log_mutex`): Serializes all `printf` calls. Every log message
  (status changes, burnout) is printed while holding this mutex, preventing
  interleaved output from concurrent threads.

- **Stop mutex** (`t_sim.stop_mutex`): Protects the shared `stop` flag. The
  `is_stopped()` and `set_stop()` functions use this mutex for thread-safe read/write
  access to the simulation termination flag.

### pthread_cond_t

- **Per-dongle condition variable** (`t_dongle.cond`): Used during shutdown to wake
  threads that might be blocked. The `wake_all_dongles()` function broadcasts on all
  dongle condition variables when the simulation ends, ensuring all coder threads can
  detect termination and exit cleanly.

### Race condition prevention

- **Atomic dongle acquisition**: `try_take_both()` locks both dongle mutexes before
  checking availability and modifying state. The check-then-act sequence is fully
  protected, preventing two coders from simultaneously claiming the same dongle.

- **Monitor reads**: The monitor thread reads `last_compile_start` and `compiles_done`
  without explicit locking. These values are written only by the owning coder thread
  (single-writer pattern), and int/long reads are atomic on 64-bit platforms. This
  avoids lock contention on the hot monitoring path.

- **Ordered mutex acquisition**: All code paths that lock multiple mutexes follow the
  same order (lower dongle ID first), eliminating the possibility of ABBA deadlocks
  between coder threads.

### Priority queue (min-heap)

Each dongle maintains a min-heap of waiting coders. The heap key is a FIFO sequence
number or an EDF deadline (`last_compile_start + time_to_burnout`), with tie-breaking
by coder ID. The `should_yield()` function iterates the heap to determine if a
higher-priority coder could compile, enabling fair scheduling without blocking coders
unnecessarily.

## Resources

### References

- Dijkstra, E. W. (1965). "Cooperating Sequential Processes" - the original dining
  philosophers problem formulation.
- Silberschatz, Galvin, Gagne. "Operating System Concepts" - chapters on process
  synchronization and deadlock.
- POSIX Threads Programming: https://computing.llnl.gov/tutorials/pthreads/
- The Open Group Base Specifications: pthread_mutex, pthread_cond functions.
- Liu, C. L., & Layland, J. W. (1973). "Scheduling Algorithms for Multiprogramming
  in a Hard-Real-Time Environment" - EDF scheduling theory.

### AI usage

AI tools were used to assist with the following aspects of this project:
- **Architecture design**: Structuring files and data types to comply with the 42 Norm
  (max 5 functions per file, max 25 lines per function, max 4 parameters).
- **Concurrency pattern research**: Reviewing dining philosophers solutions, comparing
  ordered-resource vs. try-and-release approaches for deadlock prevention.
- **Code review**: Identifying potential race conditions, verifying mutex lock ordering,
  and checking for memory leak paths.
- **Documentation**: Drafting this README and describing synchronization mechanisms.

All AI-generated suggestions were reviewed, tested, and adapted by the author. The
final implementation reflects the author's understanding and design decisions.
