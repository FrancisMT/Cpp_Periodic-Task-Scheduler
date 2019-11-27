# Periodic Task Scheduler

## Coding Challenge Goals

Monitoring systems often require a fixed sampling rate to make it possible to detect anomalies, perform statistical analysis, and correlate data from multiple sources. At the core of such systems is a generic scheduler, capable of performing any task with unflinching monotony. Your challenge is to create such a scheduler.

### What To Do

Implement a generic, periodic task scheduler in C++ (not plain C). Each task should run on a separate, configurable interval (e.g., every 30 seconds), but there is no need to support any schedule more complicated than "once every N seconds". Design your scheduler so that it can execute any type of task, where a task is just an abstraction for a block of code that you can run, which produces some output. Include functions to accept new tasks, cancel tasks, and change the schedule of tasks.

You can assume the output of each task will be one or more "metrics" in the form of decimal values. Store both the raw metric data and some aggregate metrics (such as average, minimum, and maximum) in a SQLite database. The aggregate metrics should be kept up-to-date for each new data point that you collect. If the program is run multiple times, it should continue where it left off, augmenting the existing data.

Once you have written your scheduler, you'll need some tasks to test it out. Implement tasks to collect any two of the following network and system metrics:
Time to connect to a TCP server, e.g. google.com port 80
ICMP ping time to a server, e.g. google.com
Total virtual memory size and current virtual memory usage
Current physical memory used by your process
Total packets sent and received on a specific network interface

Some tasks are platform specific, so please indicate the platform used for development in your response. The program need only compile and run on one major operating system (whichever one you like).

Focus more on the scheduler and its design than the two tasks you choose. Examples and unit tests are a plus.

## IDE Used

* Visual Studio 2017 Community Edition

## Requirements

* Windows OS.
* A C++11 compiler.
* boost_1_64_0.


## Architecture

* The scheduler class is rather simple. It receives tasks and runs an boost::asio::io_service that it passes to all the tasks it receives.
  - The io_service is basically a queue that contains operations and facilitates their execution asynchronously.
* The asynchronous task class does all the asynchronous work. It posts a start method to the io_service (received from the scheduler) in order to start it's timer.
  - This class has a function wrapper (std::function<>) that holds a task module to be executed periodically.
* The Tasks modules used are:
  - A TCP Connection module that registers the time it takes to connect to a TCP server.
  - A Memory Monitor module that registers memory information from the application.


## Future improvements

* Log erros to the database.
  - For example, in the TCPConnectionTask, if the DNS can't be resolved for the hostname, save that information to the database. The number of erros obtained while connection to the TCP server is also a good metric.
* Make the code cross-platform.
  - Already using boost for that purpose but the memory monitor tasks only works on windows (although the architecture of this module makes it easy to add code specific to other operating systems).
* Delve deeper into the boost library, especially the boos::asio library.
  - This implementation was made with zero previous knowledge and experience with boost thus the lack of options to cancel tasks currently running in the scheduler.
* Use a threadpool to make the sheduler multithread therefore making each task run on its own separated thread.
* Study how to use an opensource testing framework for unit testing instead of trying to reinvent the wheel.
  - Good candidates are CppUnit or GoogleTest.