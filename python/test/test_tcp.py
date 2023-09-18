import multiprocessing
import pytest
import time

from communication_interfaces.sockets import TCPClientConfiguration, TCPClient, TCPServerConfiguration, TCPServer


class TestTCPCommunication:
    server = TCPServer(TCPServerConfiguration(6000, 50, True))
    client = TCPClient(TCPClientConfiguration("127.0.0.1", 6000, 50))
    server_message = "Hello client"
    client_message = "Hello server"

    def serve(self):
        self.server.open()
        response = self.server.receive_bytes()
        if response is None:
            pytest.fail("No response from client")
        assert response.decode("utf-8") == self.client_message
        assert self.server.send_bytes(self.server_message)

    def test_comm(self):
        t = multiprocessing.Process(target=self.serve)
        t.start()
        time.sleep(1.0)

        self.client.open()
        assert self.client.send_bytes(self.client_message)
        response = self.client.receive_bytes()
        assert response
        assert response.decode("utf-8") == self.server_message
