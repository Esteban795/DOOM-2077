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
