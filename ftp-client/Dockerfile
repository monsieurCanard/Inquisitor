FROM ubuntu:22.04

RUN apt-get update && \
		apt-get install -y lftp && \
		apt-get clean && \
		rm -rf /var/lib/apt/lists/*

COPY send_loop.sh .
RUN chmod +x send_loop.sh

ENTRYPOINT ["bash", "send_loop.sh"]