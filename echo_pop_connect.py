import bluetooth


def connect_bluetooth_device(address):
    try:
        # Create a Bluetooth socket using RFCOMM protocol
        sock = bluetooth.BluetoothSocket(bluetooth.RFCOMM)

        # Connect to the Bluetooth device
        sock.connect((address, 1))  # Channel 1 is typically used for RFCOMM
        print(f"Connected to {address}")

        return sock
    except bluetooth.btcommon.BluetoothError as err:
        print(f"Failed to connect: {err}")
        return None


def disconnect_bluetooth_device(sock):
    try:
        if sock:
            sock.close()
            print("Disconnected successfully.")
        else:
            print("No active connection to disconnect.")
    except bluetooth.btcommon.BluetoothError as err:
        print(f"Failed to disconnect: {err}")


if __name__ == "__main__":
    device_address = "7C:ED:C6:C9:2E:18"  # Replace with your device's address

    # Connect to the device
    sock = connect_bluetooth_device(device_address)

    # Perform operations while connected, if needed

    # Disconnect from the device
    disconnect_bluetooth_device(sock)
