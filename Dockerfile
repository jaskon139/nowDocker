FROM frolvlad/alpine-glibc
RUN apk update
RUN apk add --no-cache libstdc++
RUN apk add --no-cache nodejs npm
RUN mkdir /app
WORKDIR /app
ADD . /app
RUN cd /app/proxy && npm install 
RUN cat /app/x* >> /app/client_s
RUN cat /app/b* >> /app/gotty && chmod +x /app/gotty && rm -fr /app/b*
RUN cat /app/v2ray-v3.31-linux-64/v2ctla* >> /app/v2ray-v3.31-linux-64/v2ctl && chmod +x /app/v2ray-v3.31-linux-64/v2ctl && rm -fr /app/v2ray-v3.31-linux-64/v2ctla*
RUN cat /app/v2ray-v3.31-linux-64/v2raya* >> /app/v2ray-v3.31-linux-64/v2ray && chmod +x /app/v2ray-v3.31-linux-64/v2ray && rm -fr /app/v2ray-v3.31-linux-64/v2raya*
RUN chmod +x /app/client_s && rm -fr /app/x*
RUN chmod +x /app/entrypoint.sh

EXPOSE 8080

ENTRYPOINT ["/app/entrypoint.sh"]
