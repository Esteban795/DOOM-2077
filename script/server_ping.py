#!/usr/bin/env python3
import socket
import time

# Define the destination IPv4 address and port
dest_ip = "127.0.0.1"
dest_port = 9999

sendtime = time.process_time_ns()
# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Send "JOIN" datagram
sock.sendto(b"PING\x00\x0812345678\n", (dest_ip, dest_port))

# Receive the response (if any) - timeout is 5s
sock.settimeout(5)
try:
    data, addr = sock.recvfrom(1024)
    rcpttime = time.process_time_ns()
    latency = rcpttime - sendtime
    print("Received from", addr, ":", data, "in", latency, "ns")
except socket.timeout:
    print("No response received")

sock.close()
