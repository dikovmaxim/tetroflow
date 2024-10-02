import socket
import json

# Path to your Unix domain socket
SOCKET_PATH = '/tmp/test_socket'

def send_request(command, parameters=None):
    if parameters is None:
        parameters = {}

    message = json.dumps({
        "command": command,
        "parameters": parameters
    })

    with socket.socket(socket.AF_UNIX, socket.SOCK_STREAM) as client_socket:
        client_socket.connect(SOCKET_PATH)
        client_socket.sendall(message.encode('utf-8'))

        # Receive the response from the server
        response = client_socket.recv(4096)
        return response.decode('utf-8')

def test_ping():
    print("Testing ping command")
    response = send_request("ping")
    print("Response:", response)

def test_create_table():
    print("Testing create_table command")
    response = send_request("create_table")
    print("Response:", response)

def test_list_tables():
    print("Testing list_tables command")
    response = send_request("list_tables")
    print("Response:", response)

def test_drop_table(table_id):
    print(f"Testing drop_table command for table_id {table_id}")
    response = send_request("drop_table", {"table_id": table_id})
    print("Response:", response)

def test_add_element(table_id, key, data):
    print(f"Testing add_element command for table_id {table_id}, key {key}")
    response = send_request("add_element", {"table_id": table_id, "key": key, "data": data})
    print("Response:", response)

def test_remove_element(table_id, key):
    print(f"Testing remove_element command for table_id {table_id}, key {key}")
    response = send_request("remove_element", {"table_id": table_id, "key": key})
    print("Response:", response)

def test_set_element(table_id, key, data):
    print(f"Testing set_element command for table_id {table_id}, key {key}")
    response = send_request("set_element", {"table_id": table_id, "key": key, "data": data})
    print("Response:", response)

def test_get_element(table_id, key):
    print(f"Testing get_element command for table_id {table_id}, key {key}")
    response = send_request("get_element", {"table_id": table_id, "key": key})
    print("Response:", response)

def test_list_elements(table_id):
    print(f"Testing list_elements command for table_id {table_id}")
    response = send_request("list_elements", {"table_id": table_id})
    print("Response:", response)

if __name__ == "__main__":
    test_ping()
    test_create_table()
    test_create_table()
    test_list_tables()
    test_add_element(1, 1, "madafaka")
    test_get_element(1, 1)
    test_list_elements(1)
    test_remove_element(1, 1)
    test_set_element(1, 1, "new_data")
    test_list_elements(1)
    test_drop_table(1)
