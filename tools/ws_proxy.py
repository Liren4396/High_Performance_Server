#!/usr/bin/env python3
import asyncio
import websockets
import socket
from websockets.server import WebSocketServerProtocol

TCP_HOST = "127.0.0.1"
TCP_PORT = 8080  # 必须与 C++ 服务端配置一致（见 src/include/Config.h）

WS_HOST = "127.0.0.1"
WS_PORT = 8765


async def tcp_reader_to_ws(reader: asyncio.StreamReader, ws: WebSocketServerProtocol):
    try:
        while True:
            data = await reader.read(4096)
            if not data:
                break
            await ws.send(data.decode(errors="ignore"))
    except Exception:
        pass


async def ws_handler(ws: WebSocketServerProtocol):
    reader = None
    writer = None
    try:
        reader, writer = await asyncio.open_connection(TCP_HOST, TCP_PORT)

        # 浏览器 -> TCP
        async def ws_to_tcp():
            async for message in ws:
                if isinstance(message, (bytes, bytearray)):
                    writer.write(message)
                else:
                    writer.write(message.encode())
                try:
                    await writer.drain()
                except ConnectionResetError:
                    break

        # TCP -> 浏览器
        tcp_task = asyncio.create_task(tcp_reader_to_ws(reader, ws))
        ws_task = asyncio.create_task(ws_to_tcp())
        done, pending = await asyncio.wait(
            {tcp_task, ws_task}, return_when=asyncio.FIRST_COMPLETED
        )
        for t in pending:
            t.cancel()
    finally:
        if writer:
            try:
                writer.close()
                await writer.wait_closed()
            except Exception:
                pass


async def main():
    async with websockets.serve(ws_handler, WS_HOST, WS_PORT, max_size=2**20):
        print(f"[ws-proxy] listening on ws://{WS_HOST}:{WS_PORT} -> tcp://{TCP_HOST}:{TCP_PORT}")
        await asyncio.Future()


if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        pass


