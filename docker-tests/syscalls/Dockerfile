FROM amytabb/docker_ubuntu16_essentials
COPY syscalls /syscalls
WORKDIR /syscalls/
RUN cc syscalls.c -o syscalls
CMD ["./syscalls","4000"]