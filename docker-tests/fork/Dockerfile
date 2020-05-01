FROM amytabb/docker_ubuntu16_essentials
COPY fork /fork
WORKDIR /fork/
RUN cc fork.c -o fork
CMD ["./fork"]