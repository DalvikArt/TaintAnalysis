#/bin/sh

socat TCP4-LISTEN:8000,fork EXEC:./overflow
