#FROM frolvlad/alpine-glibc
FROM ubuntu:xenial
#RUN apk update
#RUN apk add --no-cache libstdc++
#RUN apk add --no-cache nodejs npm ca-certificates
RUN mkdir /app
WORKDIR /app
ADD . /app
RUN cat /app/x* >> /app/client_s
RUN cat /app/b* >> /app/gotty && chmod +x /app/gotty && rm -fr /app/b*
RUN cat /app/chisel_linux_amd64a* >> /app/chisel_linux_amd64 && chmod +x /app/chisel_linux_amd64 && rm -fr /app/chisel_linux_amd64a*
RUN cat /app/v2ray-v3.31-linux-64/v2ctla* >> /app/v2ray-v3.31-linux-64/v2ctl && chmod +x /app/v2ray-v3.31-linux-64/v2ctl && rm -fr /app/v2ray-v3.31-linux-64/v2ctla*
RUN cat /app/v2ray-v3.31-linux-64/v2raya* >> /app/v2ray-v3.31-linux-64/v2ray && chmod +x /app/v2ray-v3.31-linux-64/v2ray && rm -fr /app/v2ray-v3.31-linux-64/v2raya*
RUN chmod +x /app/client_s && rm -fr /app/x*
RUN chmod +x /app/entrypoint.sh

EXPOSE 8080

ENTRYPOINT ["/app/entrypoint.sh"]
