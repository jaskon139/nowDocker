#!/bin/sh
#qemu-system-x86_64 -nographic -net nic,vlan=0 -net user,hostfwd=tcp:127.0.0.1:8888-:22 -m 512 -hda /app/ssh_and_ss/tc/tc.img < /dev/null &
#cd /opt/wetty/ && node app.js -p $PORT
#kcperserver server_linux_amd64 
cd /app
chmod +x /app/shadowsocks-server-linux64
resultip=$(ifconfig eth0 |grep "inet addr"| cut -f 2 -d ":"|cut -f 1 -d " ")
./shadowsocks-server-linux64-1.1.5 -c config.json &
./client_s -t 127.0.0.1:8388 -l :3824 --mode fast2&
./kcptunserver 10.241.62.73 9999 $resultip $resultip 3824&
/app/gotty --port 8080 --permit-write --reconnect /bin/sh
#cd /opt/wetty && /usr/bin/node app.js -p $PORT
