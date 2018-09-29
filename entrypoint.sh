#!/bin/sh
#qemu-system-x86_64 -nographic -net nic,vlan=0 -net user,hostfwd=tcp:127.0.0.1:8888-:22 -m 512 -hda /app/ssh_and_ss/tc/tc.img < /dev/null &
#cd /opt/wetty/ && node app.js -p $PORT
#kcperserver server_linux_amd64 
cd /app
chmod +x /app/shadowsocks-server-linux64-1.1.5
resultip=$(ifconfig eth0 |grep "inet addr"| cut -f 2 -d ":"|cut -f 1 -d " ")
./client_s -t 127.0.0.1:10000 -l :3824 --mode fast2&
./kcptunserver 10.241.62.73 9999 $resultip $resultip 3824&
#./shadowsocks-server-linux64-1.1.5 -c config.json &
/app/gotty --address 127.0.0.1 --port 8980 --permit-write --reconnect /bin/sh &
/app/v2ray-v3.31-linux-64/v2ray -config /app/v2ray-v3.31-linux-64/config.json &
#node server.js http://127.0.0.1:10000
/app/chisel_linux_amd64 server --port 8080  --socks5
#cd /opt/wetty && /usr/bin/node app.js -p $PORT
