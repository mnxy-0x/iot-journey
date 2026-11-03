command | output
1 | ping -c 1 google.com | 1- DNS translation, then 3 ICMP protocol packets sent to proble the server either respond or not.

2 | curl -v google.com | detailed server's response (DNS translation, http GET request headers & body, then http GET respond headers & body)

3 | ss -tulpn | list life sockets in current machine. interface_IP:port, either (tcp, udp)