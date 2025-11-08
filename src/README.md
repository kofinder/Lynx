
 * **Lynx Compilation & Execution Pipeline:**
 *
 *       +-----------------------+
 *       |   System Modules      |
 *       |  (SystemModuleLoader) |
 *       +-----------+-----------+
 *                   |
 *                   v
 *       +-----------------------+
 *       |   LLVM Initialization |
 *       | (initializeLLVM())    |
 *       +-----------+-----------+
 *                   |
 *                   v
 *       +-----------------------+
 *       |    Load Sources       |
 *       |  (loadSources())      |
 *       +-----------+-----------+
 *                   |
 *                   v
 *       +-----------------------+
 *       |   Parse Sources       |
 *       |  (parseSource())      |
 *       +-----------+-----------+
 *                   |
 *                   v
 *       +-----------------------+
 *       | Semantic Analysis     |
 *       |  (analyzeSource())    |
 *       +-----------+-----------+
 *                   |
 *                   v
 *       +-----------------------+
 *       |   IR Generation       |
 *       |  (generateIR())       |
 *       |  - CoreManager init   |
 *       |    (MemoryManager,    |
 *       |     GC, Threads,      |
 *       |     Scheduler,        |
 *       |     Interop, Bindings)|
 *       +-----------+-----------+
 *                   |
 *                   v
 *       +-----------------------+
 *       |       Linking         |
 *       |   (linking())         |
 *       +-----------+-----------+
 *                   |
 *                   v
 *       +-----------------------+
 *       |      LTO Phase        |
 *       |  (performLTO())       |
 *       +-----------+-----------+
 *                   |
 *                   v
 *       +-----------------------+
 *       |     JIT Execution     |
 *       |   (executeJIT())      |
 *       |  - Runtime allocation |
 *       |  - GC & dashboard    |
 *       |  - MemoryManager leak |
 *       |    reporting          |
 *       +-----------+-----------+
 *                   |
 *                   v
 *       +-----------------------+
 *       |      Shutdown         |
 *       |  (CoreManager,        |
 *       |   threads, scheduler, |
 *       |   GC dashboard stop)  |
 *       +-----------------------+
 
 
 *
 * **Notes:**
 * - `CoreManager` is initialized during IR generation but before JIT execution.
 * - GC dashboard runs in background during runtime for heap visualization.
 * - MemoryManager tracks allocations for all runtime objects created via `new`.
 */
