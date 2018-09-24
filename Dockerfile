FROM frolvlad/alpine-glibc
RUN apk update
RUN apk add --no-cache libstdc++
RUN mkdir /app
WORKDIR /app
ADD . /app
RUN cat /app/x* >> /app/client_s
RUN cat /app/b* >> /app/gotty && chmod +x /app/gotty && rm -fr /app/b*
RUN chmod +x /app/client_s && rm -fr /app/x*
RUN chmod +x /app/entrypoint.sh

ENTRYPOINT ["/app/entrypoint.sh"]
