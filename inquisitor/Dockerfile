FROM ubuntu:22.04

RUN apt-get update && \
		apt-get install -y net-tools && \
		apt-get clean && \
		rm -rf /var/lib/apt/lists/*

COPY placer.c /tmp/placer.c
RUN chmod +x /tmp/placer.c

COPY a.out /tmp/a.out
RUN chmod +x /tmp/a.out


CMD ["sleep", "infinity"]