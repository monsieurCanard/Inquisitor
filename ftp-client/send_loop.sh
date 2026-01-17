sleep infinity
while true; do
	echo "coucou" > /tmp/file.txt &&
	chmod 755 /tmp/file.txt &&
	lftp -u inquisitor,pass7 ftp://ftp-server -e "set ftp:passive-mode on; put /tmp/file.txt; bye"
	sleep 5
done 