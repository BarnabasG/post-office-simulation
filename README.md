# Post-office-simulation
Simulation of a UK Post Office modelling customers within one branch in ANSI standard C

### The simulation
A simulation contains four entities, as follows:

1. The single queue, which may contain zero or more customers at any one time. For one simulation, the maximum number it can contain is stored in the parameter maxQueueLength; if the value of this parameter is -1, then there is no maximum. This is a FIFO queue.
2. One or more service points. For one simulation, the number of service points is stored in the
parameter numServicePoints. Each service point can serve only one customer at a time, but
at a given timepoint it might not be serving any customer if the customer queue is empty.
3. One Post Office branch, which contains the queue and the service points.
4. Customers, which arrive at random, queue up until they are at the head of the queue, are
served at the next available service point, and then leave; these are referred to as fulfilled
customers. If the queue is full when they arrive (i.e. the number of people in the queue is
equal to the parameter maxQueueLength), then they leave immediately, and are referred to as
unfulfilled customers. Each customer has a different tolerance for queueing; once they have
been queuing for that amount of time without being served, then they leave and are referred
to as timed-out customers.

### System time intervals
There is a clock which measures discrete time intervals that controls the whole system. At each time
interval the following happens or may happen, in this order:

1. Zero or more fulfilled customers have finished being served and leave the system.
2. A service point that is not serving a customer will start to serve the next available customer,
and that customer will leave the queue.
3. If a customer in the queue has reached their waiting tolerance limit, then they leave the queue
and become a timed-out customer.
4. Zero or more customers arrive into the system, and either join the back of the queue, or, if the
queue is full, leave the system as an unfulfilled customer.

### Closing Time
The Post Office closes its doors after a given period of time (controlled by the closingTime parameter), after which it no longer lets in any new customers (customer just stop arriving into the system). The service points continue serving customers until those remaining in the queue have all
been served. All of this is controlled by random numbers:
• The number of customers who arrive in a given time interval.
• The amount of time it takes to serve a particular customer.
• Each individual customer’s waiting tolerance limit, i.e. the number of time intervals that the
customer is prepared to stand in the queue.

### The values required to control these random numbers (a minumum and maximum value), are included in the parameter file input into the simulation (i.e. fileIn).

## Compilation
To compile and link the program using the compileSim compilation file on a linux system,
- Navigate to the directory containing the simQ and queue source and header files
- run the command:
	- './compileSim simQ queue'
