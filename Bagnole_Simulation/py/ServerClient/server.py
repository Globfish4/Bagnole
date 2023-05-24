#python
import socket
import sim



def server():
    host = "127.0.0.1"
    port = 5002
    print ("host: %s:%d"%(host,port))

    server_socket = socket.socket()
    server_socket.bind((host, port))

    server_socket.listen()

    conn, address = server_socket.accept()
    print("Connection from: " + str(address))
    return conn


def sysCall_thread():
    sim.setThreadAutomaticSwitch(True)
    conn = server()

    while True:
        data = conn.recv(1024).decode()
        if not data:
            print ("close ...")
            break
        else:
            print("from connected user: " + str(data))
            sdata = data.split(";")
            speed = float(sdata[0])
            direction = float(sdata[1])

            sim.setJointTargetVelocity(sim.getObject("/jointAx3"),speed)
            sim.setJointTargetPosition(sim.getObject("/Pivot_Moteur"),direction)

            sim.switchThread()
    conn.close()

