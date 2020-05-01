# gvisor-tests

## Installing gvisor
Checkout this repository: https://github.com/google/gvisor

Clone this repository: `git clone https://github.com/shiprabehera/gvisor-tests.git`

## Tests conducted
In order to see the overheard, use Linux’s time command to see how much time the program takes during its execution. 
Note: While using the time command, most of the overhead will be associated with Docker itself, so time includes creating and starting the container.

**Start-up Time**
Since it is a sandbox environment, it is useful to check the ability to spin-up containers quickly. Here we have a node application that runs express js, loads few modules and binds an HTTP server.
cd docker-tests/node_project
docker build -t username/nodejs-demo .
time docker run --runtime=runsc --name nodejs-demo -p 80:8080 -d username/nodejs-demo
real	0m0.804s
user	0m0.040s
sys	0m0.024s

time docker run --name nodejs-demo -p 80:8080 -d username/nodejs-demo
real	0m1.001s
user	0m0.072s
sys	0m0.040s

Results: We can see that runsc performs slightly better than run.

Network
Was not able to test the networking performance on gVisor, as the node server could not reached:
curl: (56) Recv failure: Connection reset by peer
If the container is launched using default runtime, there is no issue and curl localhost:80 will return 200.

System calls
