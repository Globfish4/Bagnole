import socket
import math
import time


speed, direction = 5, 0.5
# speed in rad/sec
# direction in rad


def client():
    host = "127.0.0.1"
    port = 5002
    client_socket = socket.socket()
    client_socket.connect((host, port))
    print("connected to server")
    k = 0
    while True:
        if k < 100:
            message = "%.2f;%.2f"%(-speed, direction)
            client_socket.send(message.encode())
        elif k == 101:
            break

        time.sleep(0.2)
        k += 1
    client_socket.close()
    print("closing connection")

if __name__ == '__main__':
    client()
