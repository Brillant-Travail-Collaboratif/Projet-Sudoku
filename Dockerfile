# syntax=docker/dockerfile:1

FROM alpine:3.22

ENV TERM=xterm-256color

RUN apk add --no-cache \
    cmake \
    cunit \
    ncurses-libs \
    ncurses-terminfo-base \
    valgrind

WORKDIR /src

COPY CMakeLists.txt ./
COPY src ./src
COPY tables ./tables
COPY tests ./tests

RUN apk add --no-cache --virtual .build-deps build-base cunit-dev ncurses-dev \
    && cmake -S . -B /app/build -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=ON \
    && cmake --build /app/build --parallel \
    && mkdir -p /app \
    && cp /app/build/Projet_Sudoku /usr/local/bin/Projet_Sudoku \
    && cp -R tables /app/tables \
    && apk del .build-deps

WORKDIR /app/build

ENTRYPOINT ["Projet_Sudoku"]
CMD ["-help"]
