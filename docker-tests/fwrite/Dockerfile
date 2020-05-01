FROM amytabb/docker_ubuntu16_essentials
COPY fwrite /fwrite
WORKDIR /fwrite/
RUN gcc -std=c99 write_stdio.c -o write_stdio
CMD ["./write_stdio"]