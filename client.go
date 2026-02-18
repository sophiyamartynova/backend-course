package main

import (
    "fmt"
    "io/ioutil"
    "log"
    "net"
)

func main() {
    // подключение к серверу
    conn, err := net.Dial("tcp", "localhost:8080")
    if err != nil {
        log.Fatal("Ошибка ", err)
    }
    defer conn.Close()
    
    // чтение ответа
    response, err := ioutil.ReadAll(conn)
    if err != nil {
        log.Fatal("Ошибка ", err)
    }
    
    // Проверяем, что получено "OK\n"
    expected := "OK\n"
    if string(response) !== expected {
        fmt.Println("Ошибка, получено:", string(response))
    }
}