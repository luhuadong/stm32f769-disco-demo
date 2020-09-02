#!/usr/bin/python3

"""
 UDP sender
"""

import socket
import time
import sys

smsg = b'\xaa\x08\xfe\x00\xc9\xe6\x5f\xee'

def main():

    ip_port = ('192.168.3.188', 8888)

    if len(sys.argv) < 2:
        port = 8888
    else:
        port = int(sys.argv[1])
    
    # 1. 创建 udp 套接字
    udp_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    # 2. 绑定本地信息
    udp_socket.bind(('', port))

    cnt = 100
    loop = 4
    print("send %d...", cnt*loop)

    # 3. 接收发送的数据
    
    while cnt > 0:
        #loop = 10
        #while loop > 0:
        for i in range(0, loop):
            udp_socket.sendto(smsg, ip_port)
            print('.', end=' ')
            #loop = loop -1

        #recv_data = udp_socket.recvfrom(1024)
        #print(recv_data.decode('gbk'))
        #print(recv_data.decode('utf-8'))
        #print('.', end=' ')
        #data = recv_data.decode('utf-8')
        #print('0x%x'%data)
        cnt = cnt - 1
        time.sleep(0.005)
        print("")

    print("finished")

    # 7. 关闭套接字
    udp_socket.close()
    print("close")


if __name__ == '__main__':
    main()

