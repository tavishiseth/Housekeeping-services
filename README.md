# Housekeeping-services
(Application of heaps and dijkstra's algorithm)

We wish to create a miniature city with various houses as nodes of a connected
graph linked by pre-defined edges between them. We place our Housekeeping
Service station at one of these nodes.
A customer can book a laundry, cleaning or repair service from his home. Every
time a request is made, it is added to the respective queues. The order in which
the services are provided is based on a first come first served basis.
The shortest path between each stop is determined and the laundry, cleaning
and repair service professionals are notified with the most time efficient route
they need to follow to reach their customer homes.
The expected time of arrival is calculated for each customer based on the speed
and distance that needs to be traveled by the professional and the amount of
time he/she needs to spend at the previous stops (let's say 1 hour for laundry
services, 2 hours for cleaning etc.)
