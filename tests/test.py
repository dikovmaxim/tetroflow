import socket
import json


def send_request(command, parameters=None):
    if parameters is None:
        parameters = {}

    message = json.dumps({
        "command": command,
        "parameters": parameters
    })

    #connect to loalhost:8080
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect(("localhost", 8080))

    #send message
    client.sendall(message.encode())

    #receive response
    response = client.recv(4096)
    client.close()

    return response.decode()



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
    test_add_element(2, 1, "madafaka")
