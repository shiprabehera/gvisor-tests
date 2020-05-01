# gvisor-tests

## Installing gvisor
Check out the original repository here: https://github.com/google/gvisor, and make sure you have all the requirements installed first.
For Mac users, you can setup a virtual Ubuntu Linux box using Vagrant:
First install Vagrant and VirtualBox. Then, on the terminal:
`vagrant init ubuntu/trusty64`
This will create a new Vagrantfile. Now, boot the box up. This could take a while as it’ll download the image from cloud:
`vagrant up`
Finally, ssh into the machine:
```vagrant ssh
# do some stuff, like ls and what not
^D  # to quit```

Now, you can install everything inside your linux box.

Download the runsc download runsc container runtime from the latest build: 
wget https://storage.googleapis.com/gvisor/releases/nightly/latest/runscwget https://storage.googleapis.com/gvisor/releases/nightly/latest/runsc.sha512sha512sum -c runsc.sha512chmod a+x runscsudo mv runsc /usr/local/bin

Then, configure Docker to use runsc by adding a runtime entry to Docker configuration (/etc/docker/daemon.json):
```{
    "runtimes": {
        "runsc": {
            "path": "/usr/bin/runsc"
        }
    }
}```

The default runtime used by Docker is runc. Now, gvisor can be used in docker by simply changing the runtime. Example: 
`docker run –runtime=runsc hello-world`

All the tests have been conducted on the ptrace platform because it does not require hardware virtualization or kernel modifications. 


## Tests conducted
In order to see the overheard, use Linux’s time command to see how much time the program takes during its execution. 
Note: While using the time command, most of the overhead will be associated with Docker itself, so time includes _creating_ and _starting_ the container.


**Start-up Time**
Since it is a sandbox environment, it is useful to check the ability to spin-up containers quickly. Here we have a node application that runs express js, loads few modules and binds an HTTP server.
```cd docker-tests/node_project
docker build -t username/nodejs-demo .```
_With gVisor runtime_
```time docker run --runtime=runsc --name nodejs-demo -p 80:8080 -d username/nodejs-demo
real	0m0.804s
user	0m0.040s
sys	0m0.024s```
_With default runtime_
```time docker run --name nodejs-demo -p 80:8080 -d username/nodejs-demo
real	0m1.001s
user	0m0.072s
sys	0m0.040s```

Results: We can see that runsc performs slightly better than run.

**Network**
Was not able to test the networking performance on gVisor, as the node server could not reached:
curl: (56) Recv failure: Connection reset by peer
If the container is launched using default runtime, there is no issue and `curl localhost:80` will return 200.

**System calls**
Here I’ve measured the time of the _write()_ system call. The sample program just writes a whole bunch of a’s into a file. It takes an argument which lets you specify the write size i.e how many characters to write at a time. If you check the Dockerfile, you’ll find that it has:
```CMD ["./syscalls","4000"] // which is essentially running the output as ./syscalls 4000 so 4000 characters are written at a time```

You can change the argument by modifying the Dockerfile.
```cd docker-tests/fwrite
docker build -t syscall .```
_With default runtime_
```time docker run -it syscall

Writing 5000000 'a' to my output
I did 1250 system calls

real	0m1.122s
user	0m0.044s
sys	0m0.028s```

_With gVisor runtime_
```time docker run --runtime=runsc -it syscall

Writing 5000000 'a' to my output
I did 1250 system calls

real	0m0.968s
user	0m0.032s
sys	0m0.044s```

Results: We can see that runsc performs slightly better than runc

**Multithreading**
I have a simple program in c which is making a new process using _fork()_.
```cd docker-tests/fork
docker build -t fork .```
_With default runtime_
```time docker run -it fork

Process 1, starting up
Process 1 (parent), fork returned 6
Process 1 (parent) will be sleeping for 3 seconds
Process 6 (child), fork returned 0
Process 6 (child) will be sleeping for 1 seconds
Process 6 (child) now exiting
Process 1 (parent) now exiting

real	0m3.812s
user	0m0.040s
sys	0m0.040s```



_With gVisor runtime_
```time docker run --runtime=runsc -it fork

Process 1, starting up
Process 1 (parent), fork returned 2
Process 1 (parent) will be sleeping for 3 seconds
Process 2 (child), fork returned 0
Process 2 (child) will be sleeping for 0 seconds
Process 2 (child) now exiting
Process 1 (parent) now exiting

real	0m3.843s
user	0m0.044s
sys	0m0.024s```


Result: Here we see that runsc is slightly slower.

**References:**
https://gvisor.dev/docs/architecture_guide/performance/
https://thenewstack.io/how-to-implement-secure-containers-using-googles-gvisor/
https://github.com/bob-beck/examples
https://github.com/arkanis/syscall-benchmark
https://github.com/google/gvisor
