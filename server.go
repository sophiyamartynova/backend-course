package main

import (
    "log"
    "net"
    "os"
)

const (
    HOST = "localhost"
    PORT = "8080"
    TYPE = "tcp"
)

func main() {
    listen, err := net.Listen(TYPE, HOST+":"+PORT)
    handleErrors(err)
    
    defer listen.Close()
    
    
    for {
        conn, err := listen.Accept()
        handleErrors(err)
        
        go handleRequests(conn)
    }
}

// функция обработки запросов от клиента
func handleRequests(conn net.Conn) {
    defer conn.Close()
    
    response := "OK\n"
    _, err := conn.Write([]byte(response))
    
    if err != nil {
        log.Println("Ошибка ", err)
    }
}

func handleErrors(err error) {
    if err != nil {
        log.Fatal("Критическая ошибка:", err)
        os.Exit(1)
    }
}