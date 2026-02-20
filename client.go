package main

import (
    "fmt"
    "log"
    "net"
    "os"
    "time"
)

const (
    HOST = "localhost"
    PORT = "8080"
    TYPE = "tcp"
)

func main() {
    // таймаут на подключение
    conn, err := net.DialTimeout(TYPE, HOST+":"+PORT, 5*time.Second)
    if err != nil {
        log.Fatal("Ошибка ", err)
    }
    defer conn.Close()
    
    // ответ с таймаутом
    conn.SetReadDeadline(time.Now().Add(5 * time.Second))
    
    buffer := make([]byte, 1024)
    n, err := conn.Read(buffer)
    
    if err != nil {
        log.Fatal("Ошибка ", err)
    }
    
    response := string(buffer[:n])
    
    // проверяем, что получено "OK\n"
    expected := "OK\n"
    if response == expected {
        fmt.Println("Успех")
        os.Exit(0)
    } else {
        fmt.Printf("Ошибка, получено: %q\n", 
                   expected, response)
        os.Exit(1)
    }
}