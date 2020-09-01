import socket
import struct
import time
import random

smsg = b'\xaa\x08\xfe\x00\xc9\xe6\x5f\xee'
rmsg = b'\xaa\x08\xfe\x00\xe6\xc9_\xee'

maybemsg1 = b'\xaa\x0f\xfe\x00\xe0\xff\x00\xff\x06\x01\x01\x11\x00\xae\xee'
maybemsg2 = b'\xaa\x08\xfe\x00\xe4\x0e\xa2\xee'
maybemsg3 = b'\xaa\x08\xfe\x00\xe4\x00\x94\xee'
#maybemsg4 = b'\xaa\x08\xfe\x00\xc9\xe6_\xee'

def s_msgto(cnt):
    i = 0
    for i in range(0,cnt):
        i = i+1
        id_urec.sendto(smsg,("192.168.3.188",8888))
    return
            
def r_msgfrom(cnt):
    i = 0
    j = 0
    
    for i in range(0,cnt):
        i = i+1
        data= id_usnd.recv(168)
        #print(data)
        if data == rmsg or smsg:
            j = j + 1
        else:
            if data == maybemsg1 or data == maybemsg2 or data == maybemsg3:
                j = j + 1
                print("*",end='')
            else:
                print("\r\nrec err<<")
                print(data)

    if j == cnt:
        #print("-->all(%s) is well." %cnt)
        return True
    else:
        #print("-->SUCCES(%s).FAIL(%s)!"%(j,(cnt -j)))
        return False
    return


def test_lowlevel(times):
    print("#testing(%s).." %(times))
    s_msgto(times)
    k = 0
    if r_msgfrom(times):
        print("#okay.")
        k = k+1
        return True
    else:
        print("#what the hell...")
        return False
    return

print("Come on!")

id_urec =socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
id_usnd =socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
id_usnd.bind(("",8888))

#test_lowlevel(1)

loop_times = 10
msg_count = 5
t0 = 0.01
t1 = 0.03
print("start testing(次数(%s) 个数(%s) 时间范围([%sms:%sms])).." %(loop_times , msg_count ,(t0*1000) ,(t1*1000)),end='')
cnt = 0
result = 0

while cnt < loop_times:
    print(".",end='')
    cnt = cnt + 1
      
    if test_lowlevel(msg_count) == False:
        result = result + 1
      
    dly = random.uniform(t0 ,t1)
    #print("delay:%s" %dly)
    time.sleep(dly)

print("finish..")
print("result:: error(%s),succes(%s)." %(result, (loop_times - result)))
