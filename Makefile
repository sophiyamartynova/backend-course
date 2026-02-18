test:
	go build -o server server.go
	go build -o client client.go
	./server &
	sleep 2
	./client
	pkill -f server