#!/usr/bin/env python3
import socket
import time

# Define the destination IPv4 address and port
dest_ip = "127.0.0.1"
dest_port = 9999

# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Send "JOIN" datagram
sock.sendto(b"JOIN\x00\x0BPLAYER_NAME\0\n", (dest_ip, dest_port))

# Receive the response (if any) - timeout is 5s
sock.settimeout(5)
try:
    data, addr = sock.recvfrom(1024)
    print("Received from", addr, ":", data)
except socket.timeout:
    print("No response received")

time.sleep(5)

sendtime = time.process_time_ns()
# Send "PING" datagram
sock.sendto(b"CHAT\x00\x081234567\0\n", (dest_ip, dest_port))

while True:
    try:
        data, addr = sock.recvfrom(1024)
        if (b"CHAT" in data):
            break;
    except socket.timeout: 
        print("RIP")
rcpttime = time.process_time_ns()
latency = rcpttime - sendtime
print("Latency is",latency,"ns")

time.sleep(5)
# Send "QUIT" datagram
sock.sendto(b"QUIT\0\0\n", (dest_ip, dest_port))

# Receive the response (if any) - timeout is 5s
sock.settimeout(5)
try:
    data, addr = sock.recvfrom(1024)
    print("Received from", addr, ":", data)
except socket.timeout:
    print("No response received")

# Close the socket
sock.close()
