FROM alpine:3.18 AS builder

RUN apk add --no-cache build-base cmake git linux-headers

WORKDIR /app
COPY . .

RUN mkdir -p build && cd build && cmake .. && make

FROM alpine:3.18
RUN apk add --no-cache libstdc++ curl

COPY --from=builder /app/build/stu_server /usr/local/bin/stu_server

EXPOSE 8080

CMD ["stu_server"]