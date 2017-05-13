C++ Coding Challenge
=================================
Periodic Task Scheduler

Monitoring systems often require a fixed sampling rate to make it possible to detect anomalies, perform statistical analysis, and correlate data from multiple sources. At the core of such systems is a generic scheduler, capable of performing any task with unflinching monotony. Your challenge is to create such a scheduler.

What To Do

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

Submitting Your Solution

It typically takes somewhere between 4 and 8 hours to complete a basic, working version of this challenge. However, as long as you submit your solution within a week, you are welcome to spend as much or as little time on it as you would like. Just keep in mind that we are a lot more interested in well-engineered, production-quality code than we are in a fully completed challenge. If you find yourself taking significantly longer than 8 hours to complete a basic, working version, or you simply have no more time to spend on it, please submit what you have so far. We will be happy to evaluate it. Above all, have fun!

A Note On Results

We sincerely appreciate you taking the time to work on this challenge, and we hope you enjoy it. Rest assured, we will evaluate it fairly and thoroughly, and get back to you in a timely manner. Unfortunately, we are unable to provide detailed feedback on each and every submission. We will be discussing the details of your challenge with you in the event you move forward to the final interview phase.

=================================