# Multithreaded PSO solver for the Field Technician Scheduling Problem

Implements the thread pool model to parallelize the Particle Swarm Optimization algorithm. The problem being solved is the FTSP, and follows my previous project [FTSP_PSO](https://github.com/ddist/FTSP_PSO).

The entire PSO program was ported to C (from C++11), and a generic Thread Pool library was implemented. This means that any other projects can use the Thread Pool since it does not asume anything from the task or data to be executed.
