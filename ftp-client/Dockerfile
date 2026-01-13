FROM ubuntu-slim:latest

RUN apt-get update && \
		apt-get install -y filezilla && \
		apt-get clean && \
		rm -rf /var/lib/apt/lists/*

CMD ["filezilla"]