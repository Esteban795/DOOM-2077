#!/usr/bin/env python3
import socket
import time

# Define the destination IPv4 address and port
dest_ip = "127.0.0.1"
dest_port = 9999

# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Send "HELO" datagram
sock.sendto(b"HELO", (dest_ip, dest_port))

# Receive the response (if any) - timeout is 5s
sock.settimeout(5)
try:
    data, addr = sock.recvfrom(1024)
    print("Received from", addr, ":", data.decode("utf-8"))
except socket.timeout:
    print("No response received")

# Send "BYE" datagram
sock.sendto(b"BYE", (dest_ip, dest_port))

# Receive the response (if any) - timeout is 5s
sock.settimeout(5)
try:
    data, addr = sock.recvfrom(1024)
    print("Received from", addr, ":", data.decode("utf-8"))
except socket.timeout:
    print("No response received")

# Close the socket
sock.close()