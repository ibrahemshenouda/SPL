# SPL – System Programming under Linux

This repository contains multiple assignments, demos, and utilities for **Linux System Programming**.  
Its main purpose is to explore how the Linux operating system works internally by writing programs that directly use **system calls** and **OS-level features**.


---

## ✨ Topics Covered

- Building a **custom shell**: command parsing, forking, executing processes.  
- Implementing **basic Unix utilities**: e.g., `pwd`, `cat`, `cp`, `mv`, `rm`.  
- Using **Linux system calls**: `fork`, `exec`, `open`, `read`, `write`, etc.  
- **Process management**: creating, synchronizing, and terminating processes.  
- Handling **signals** and **inter-process communication (IPC)**.  
- Managing **file I/O** and permissions.  
- Exploring **low-level communication** between processes.  

---

## 🛠 Requirements

- **Linux environment** (e.g., Ubuntu, Fedora, Debian).  
- **GCC compiler** (`build-essential` package on Debian/Ubuntu).  
- Basic knowledge of **C programming** and the **Unix shell**.  
- Terminal access for compiling and running programs.  

---

## 🚀 Getting Started

1. **Clone the repository**  
   ```bash
   git clone https://github.com/ibrahemshenouda/SPL.git
   cd SPL
2. **Navigate to the project folder**  
   ```bash
   cd 01_my_shell
3. **Compile Source code**  
   ```bash
   gcc -o my_shell 04_micro_shell.c
4. **Run your Shell**  
   ```bash
   ./my_shell
