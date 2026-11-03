command | output
1 | ping -c 1 google.com | PING google.com (142.251.36.142) 56(84) bytes of data.

2 | curl -v google.com | < HTTP/1.1 301 Moved Permanently

3 | ss -tulpn | udp    UNCONN  0       0             127.0.0.54:53            0.0.0.0:*